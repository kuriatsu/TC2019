/*
 * Copyright (c) 2010, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

// %Tag(FULLTEXT)%
// %Tag(INCLUDES)%
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
// %EndTag(INCLUDES)%
#include <tf/transform_broadcaster.h>
#include "tf/message_filter.h"

// %Tag(INIT)%
int main( int argc, char** argv )
{
  ros::init(argc, argv, "car_marker");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("car", 1);

  while (ros::ok())
  {
    visualization_msgs::Marker marker;

    marker.header.frame_id = "/velodyne";
    marker.header.stamp = ros::Time::now();
    marker.ns = "car";
    marker.id = 0;
    marker.type = visualization_msgs::Marker::MESH_RESOURCE;
    //marker.mesh_resource = "file:///home/takeuchi/research_tools/ros/chair/models/Chair.dae";
    marker.mesh_resource = "file:///home/takeuchi/prius.dae";
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
