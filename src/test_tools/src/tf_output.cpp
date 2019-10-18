#include "ros/ros.h"
#include "std_msgs/String.h"
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include "tf/message_filter.h"
#include <iostream>


int main(int argc, char *argv[]){
  ros::init(argc, argv, "tf_output");
  
  ros::NodeHandle n;
  tf::TransformListener listener;
  ros::Rate loop_rate(10);

  while (ros::ok()){
    tf::StampedTransform transform;
    
    try{
      listener.waitForTransform("map","base_link",ros::Time(0),ros::Duration(10));
      listener.lookupTransform("map","base_link",ros::Time(0),transform);
    }catch(tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
    }
    tf::Transform offset;
    offset.setOrigin(tf::Vector3(1.7, 0, 1.5));
    offset.setRotation(tf::Quaternion(0,0,0,1));
    tf::Transform transform_=transform*offset;
    printf("%f %f %f %f %f %f %f %f %f %f %f \n",
	   transform.stamp_.toSec(),
	   transform_.getOrigin().x(),
	   transform_.getOrigin().y(),
	   transform_.getOrigin().z(),
	   transform_.getRotation().x(),
	   transform_.getRotation().y(),
	   transform_.getRotation().z(),
	   transform_.getRotation().w(),
	   transform.getOrigin().x(),
	   transform.getOrigin().y(),
	   transform.getOrigin().z()
	   );
    ros::spinOnce();
    loop_rate.sleep();    
  }//  ros::spin();
   
}
