#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <ros/ros.h>
#include "trigger_rec/Trigger.h"
#include <boost/circular_buffer.hpp>


boost::circular_buffer<std::pair<double, trigger_rec::Trigger > > trigger_buffer(100);

void triggerCallback(const trigger_rec::Trigger::ConstPtr &msg){
  trigger_buffer.push_front(std::pair<double, trigger_rec::Trigger >(ros::Time::now().toSec(), *msg));
}

int search_trigger(int mask,double time,double dm, double dp, int prev_id){
  for(int i=0; i<trigger_buffer.size(); i++){
    if(trigger_buffer[i].second.triggerOut & mask &&
       trigger_buffer[i].first < time+dp &&
       trigger_buffer[i].first > time+dm &&
       i!=prev_id )return i;
  }
  return -1;
}

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  double pt_image=msg->header.stamp.toSec();
  static int id_prev;

  int id=search_trigger(0x01,pt_image, -0.05, -0.01, 0);
  
  int tt_image=trigger_buffer[id].second.offsetTime;

  if(id>=0)printf("%f %d %f\n",pt_image,tt_image, trigger_buffer[id].first);

  id_prev=id;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_time");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/camera1/image_raw", 1, imageCallback); 
  ros::Subscriber trigger_sub=nh.subscribe("trigger",1000,triggerCallback);
 
  ros::spin();
}
