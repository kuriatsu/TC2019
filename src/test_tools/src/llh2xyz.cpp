#include "ros/ros.h"
#include "sensor_msgs/NavSatFix.h"
#include <tf/transform_broadcaster.h>
#include"geo_pos_conv.hh"
geo_pos_conv geo;

void chatterCallback(const sensor_msgs::NavSatFix::ConstPtr& msg)
{
  static   tf::TransformBroadcaster br;
  
  geo.set_llh(msg->latitude*M_PI/180.0,
	      msg->longitude*M_PI/180.0,
	      msg->altitude);

  printf("%f %f %f\n",geo.x,geo.y,geo.z);
  tf::Transform transform;    
  tf::Quaternion q;

  transform.setOrigin( tf::Vector3(geo.x, geo.y, geo.z) );
  q.setRPY(0,0,0);
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, msg->header.stamp,  "japan_7", "gps"));
  
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "llh2xyz");
  geo.set_plane(7);
  

  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("fix", 1000, chatterCallback);
  ros::spin();

  return 0;
}
