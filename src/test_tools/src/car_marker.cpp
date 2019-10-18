#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <tf/transform_broadcaster.h>
#include "tf/message_filter.h"

int main( int argc, char** argv )
{
  ros::init(argc, argv, "car_marker");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>(argv[2], 1);
  char file_name[300];
  sprintf(file_name,"file://%s",argv[1]);

  while (ros::ok())
  {
    visualization_msgs::Marker marker;
    
    marker.header.frame_id = argv[2];
    marker.header.stamp = ros::Time::now();
    marker.ns = argv[2];
    marker.id = 0;
    marker.type = visualization_msgs::Marker::MESH_RESOURCE;
    marker.mesh_resource = file_name;
    marker.mesh_use_embedded_materials=true;
    marker.action = visualization_msgs::Marker::ADD;
    marker.pose.position.x = 0.5;
    marker.pose.position.y = 0;
    marker.pose.position.z = -1.8;
    marker.frame_locked=true;
  
    tf::Quaternion q;
    q.setRPY(M_PI/2,0,M_PI);
    marker.pose.orientation.x = q.x();
    marker.pose.orientation.y = q.y();
    marker.pose.orientation.z = q.z();
    marker.pose.orientation.w = q.w();
 
 
    marker.scale.x = 1.0;
    marker.scale.y = 1.0;
    marker.scale.z = 1.0;
    marker.color.r = 0.0f;
    marker.color.g = 0.0f;
    marker.color.b = 0.0f;
    marker.color.a = 0.0;
    //  marker.lifetime = ros::Duration(0.1);

    marker_pub.publish(marker);
    printf("p\n");

    r.sleep();
    ros::spinOnce();
  }

}
// %EndTag(FULLTEXT)%
