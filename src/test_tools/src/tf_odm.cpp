#include "ros/ros.h"
#include "std_msgs/String.h"
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include "tf/message_filter.h"
#include <iostream>
#include "nav_msgs/Odometry.h"


int main(int argc, char *argv[]){
  ros::init(argc, argv, "tf_odm");
  
  ros::NodeHandle n;
  ros::Publisher   m_motion_pub = n.advertise<nav_msgs::Odometry>("odm", 10);
  nav_msgs::Odometry odm;
  tf::TransformListener listener;
  ros::Rate loop_rate(10);
  odm.header.seq=0;

  while (ros::ok()){
    tf::StampedTransform transform;
    
    try{
      listener.waitForTransform("world","velodyne",ros::Time(0),ros::Duration(10));
      listener.lookupTransform("world","velodyne",ros::Time(0),transform);
    }catch(tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
    }

    odm.header.stamp = transform.stamp_;
    odm.header.frame_id = "world";
    odm.child_frame_id  = "odm";
    odm.header.seq++;
    odm.pose.pose.position.x = transform.getOrigin().x();
    odm.pose.pose.position.y = transform.getOrigin().y();
    odm.pose.pose.position.z = transform.getOrigin().z();
    odm.pose.pose.orientation.x = transform.getRotation().x();
    odm.pose.pose.orientation.y = transform.getRotation().y();
    odm.pose.pose.orientation.z = transform.getRotation().z();
    odm.pose.pose.orientation.w  = transform.getRotation().w();
    odm.twist.twist.linear.x = 0;
    odm.twist.twist.linear.y = 0;
    odm.twist.twist.linear.z = 0;
    odm.twist.twist.angular.x = 0;
    odm.twist.twist.angular.y = 0;
    odm.twist.twist.angular.z = 0;
    for(int i=0;i<6;i++){
      for(int j=0;j<6;j++){
	if(i==j){ 
	   odm.twist.covariance[i] = 1;
	   odm.pose.covariance[i] = 1;
	}else{
	   odm.twist.covariance[i] = 0;
	   odm.pose.covariance[i] = 0;
	}
      }
     }
    printf("%f %f %f %f %f %f %f %f\n",
	   transform.stamp_.toSec(),
	   odm.pose.pose.position.x,
	   odm.pose.pose.position.y,
	   odm.pose.pose.position.z,
	   odm.pose.pose.orientation.x,
	   odm.pose.pose.orientation.y,
	   odm.pose.pose.orientation.z,
	   odm.pose.pose.orientation.w);
    m_motion_pub.publish(odm);
    ros::spinOnce();
    loop_rate.sleep();
    
  }//  ros::spin();
   
}
