/*
 * Copyright (C) 2008, Morgan Quigley and Willow Garage, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the names of Stanford University or Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
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
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/PointCloud2.h"
#include "velodyne_pointcloud/point_types.h"
#include "velodyne_pointcloud/rawdata.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>


/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
// %Tag(CALLBACK)%
//void velodyneCallback(const sensor_msgs::PointCloud2::ConstPtr& msg)
void velodyneCallback(const pcl::PointCloud<pcl::PointXYZI>::ConstPtr& msg)
{
  static int count=0;
  ROS_INFO("%s",msg->header.frame_id.c_str());
  //ROS_INFO("%ld",msg->header.stamp.sec);// + (double)msg->header.stamp.nsec/1000000000.);

  pcl::PointCloud<pcl::PointXYZI> cloud;
  
  cloud.width    = 5;
  cloud.height   = 1;
  cloud.is_dense = false;
  cloud.points.resize (cloud.width * cloud.height);

  for (size_t i = 0; i < cloud.points.size (); ++i)
  {
    cloud.points[i].x = 1024 * rand () / (RAND_MAX + 1.0f);
    cloud.points[i].y = 1024 * rand () / (RAND_MAX + 1.0f);
    cloud.points[i].z = 1024 * rand () / (RAND_MAX + 1.0f);
  }

  /*  ros::Time stamp;
  stamp = msg->header.stamp;
  double now = ros::Time::now().toSec();
  */
  ROS_INFO("get data %d",msg->points.size());

  std::stringstream filename;
  filename << "/tmp/range" << std::setw( 6 ) << std::setfill( '0' ) << count << ".pcd";
  //pcl::io::savePCDFileASCII (filename.str(), msg);
  pcl::io::savePCDFileASCII ("/tmp/range.pcd", msg);
  count++;
  
  /*  ROS_INFO("height  %d",msg->height);
  ROS_INFO("width %d",msg->width);
  ROS_INFO("row_step %d",msg->row_step);
  ROS_INFO("point_step %d",msg->point_step);
  ROS_INFO("field1 %s %d %d %d",
	   msg->fields[0].name.c_str(),
	   msg->fields[0].offset,
	   msg->fields[0].datatype,
	   msg->fields[0].count);
  ROS_INFO("field2 %s %d %d %d",
	   msg->fields[1].name.c_str(),
	   msg->fields[1].offset,
	   msg->fields[1].datatype,
	   msg->fields[1].count);

  ROS_INFO("field3 %s %d %d %d",
	   msg->fields[2].name.c_str(),
	   msg->fields[2].offset,
	   msg->fields[2].datatype,
	   msg->fields[2].count);
  ROS_INFO("field4 %s %d %d %d",
	   msg->fields[3].name.c_str(),
	   msg->fields[3].offset,
	   msg->fields[3].datatype,
	   msg->fields[3].count);
  */


}
// %EndTag(CALLBACK)%

int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line. For programmatic
   * remappings you can use a different version of init() which takes remappings
   * directly, but for most command-line programs, passing argc and argv is the easiest
   * way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "listener_velodyne");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
// %Tag(SUBSCRIBER)%
  ros::Subscriber sub = n.subscribe("velodyne_points", 1000, velodyneCallback);
// %EndTag(SUBSCRIBER)%

  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
   */
// %Tag(SPIN)%
  ros::spin();
// %EndTag(SPIN)%

  return 0;
}
// %EndTag(FULLTEXT)%
