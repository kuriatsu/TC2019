#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "geometry_msgs/Point32.h"
#include "geometry_msgs/TwistStamped.h"
#include "sensor_msgs/PointCloud.h"
#include <vector>
#include <iostream>
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud.h"
#include "laser_geometry/laser_geometry.h"
#include <stdlib.h>
#include "visualization_msgs/Marker.h"
#include </usr/local/include/opencv2/core/core.hpp>
#include </usr/local/include/opencv2/highgui/highgui.hpp>
#include </usr/local/include/opencv2/imgproc/imgproc.hpp>
//#include "CloudClusterArray.msg"

laser_geometry::LaserProjection projector2_;
using namespace std;
ros::Publisher vis_redzone,vis_outerzone,cloud_pub,twist_cmd_out;

class cluster{
public:
    vector<geometry_msgs::Point32> points;
};

ros::Publisher emergency_stop;
bool safe = true;
bool decel = false;
float dis_closest_global = 10.0;
float safe_x = 0.80;
float error_x = 0.20;
float safe_y = 0.30;
float decel_x = 2.00;
float decel_y = 0.60;
bool new_vel = false;

void segment_clusters(vector<geometry_msgs::Point32> data,vector<cluster>& data_clusters)
{
	int j;
	float x_threshold = 0.05;
	float y_threshold = 0.05;
	cluster new_cluster;
	for (int i=0;i<data.size();i++)
	{
		if (i==0)
		{
			data_clusters.push_back(new_cluster);
			j = 0;
		}
		else
		{
			if ((abs(data[i].x-data[i-1].x) < x_threshold) && (abs(data[i].y-data[i-1].y) < y_threshold));
			else
			{
				data_clusters.push_back(new_cluster);
				j++;
			}
		}
		data_clusters[j].points.push_back(data[i]);
	}
}

void publish_regions()
{
    visualization_msgs::Marker marker;
    marker.header.frame_id = "/velodyne";
    marker.header.stamp = ros::Time();
    marker.id = 0;
    marker.type = visualization_msgs::Marker::CUBE;
    marker.action = visualization_msgs::Marker::ADD;
    marker.pose.position.x = ((safe_x-error_x)/2) + error_x;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;
    marker.scale.x = safe_x-error_x;
    marker.scale.y = safe_y*2;
    marker.scale.z = 0.1;
    marker.color.a = 0.5; // Don't forget to set the alpha!
    marker.color.r = 1.0;
    marker.color.g = 0.0;
    marker.color.b = 0.0;
    vis_redzone.publish(marker);
    marker.pose.position.x = ((decel_x-error_x)/2) + error_x;
    marker.pose.position.z = 0.1;
    marker.scale.x = decel_x-error_x;
    marker.scale.y = decel_y*2;
    marker.scale.z = 0.1;
    marker.color.a = 0.2; // Don't forget to set the alpha!
    marker.color.r = 1.0;
    marker.color.g = 1.0;
    marker.color.b = 1.0;
    vis_outerzone.publish(marker);
}

/*void pub_clusters(vector<geometry_msgs::Point32> data)
{
	vector<clusters>data_clusters;
	vector<geometry_msgs::Point32> relevant_points;
	geometry_msgs::Point32 point;
	float x_minthresh = 0.15;
	float x_thresh = 3.0;
	float y_thresh = 3.0;
	for (int i=0;i<in_scan.points.size();i++)
		  {
			point.x = in_scan.points[i].x;
			point.y = in_scan.points[i].y;
			point.z = 0;
			if ((point.x < x_thresh) && (point.x > x_minthresh) && (point.y < y_thresh))
				{
					relevant_points.push_back(point);
				}
		  }
	segment_clusters(relevant_points,data_clusters);
	autoware_msgs::CloudClusterArray cloud_clusters;
	for (int i=0;i<data_clusters.size();i++)
	{
		autoware_msgs::CloudCluster cluster;
		cluster.cloud.points.resize(data_clusters[i].points.size());

	}

} */

void laser_callback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
	sensor_msgs::PointCloud in_scan;
	projector2_.projectLaser(*msg, in_scan);
	in_scan.header.stamp = ros::Time::now();
	in_scan.header.frame_id = "/velodyne";
	cloud_pub.publish(in_scan);
	publish_regions();
	int minsize_object = 3;
	bool obstacle=false;
	bool soon_obstacle = false;
	//cerr << "--close--";
	vector<geometry_msgs::Point32> unsafe_points;
	vector<geometry_msgs::Point32> relevant_points;
	geometry_msgs::Point32 point;
	for (int i=0;i<in_scan.points.size();i++)
	  {
		point.x = in_scan.points[i].x;
		point.y = in_scan.points[i].y;
		point.z = 0;
		if ((point.x < safe_x) && (point.x > error_x) && (abs(point.y) < safe_y))
			{
				unsafe_points.push_back(point);
			}
		else if ((point.x < decel_x) && (point.x > error_x) && (abs(point.y) < decel_y))
		    {
		    	relevant_points.push_back(point);
		    }
	  }
	//cerr << "--mid--";
	vector<cluster> redzone_clusters;
	segment_clusters(unsafe_points,redzone_clusters);
	for (int i=0;i<redzone_clusters.size();i++)
	 {
		if (redzone_clusters[i].points.size() > minsize_object)
			{
				//cerr << " obstacle detected ";
				obstacle = true;
				break;
			}
	 }
	if (obstacle == true) safe = false;
	else safe = true;
    vector<cluster> object_clusters;
	segment_clusters(relevant_points,object_clusters);
	float dis_closest_local = 10.0;
    using namespace cv;
	vector<Point2f>points;
	RotatedRect cluster_bb;
	//cerr << " ---far--- ";
	for (int i=0;i<object_clusters.size();i++)
	 {
		if (object_clusters[i].points.size() > minsize_object)
		  {
				soon_obstacle = true;
			    for (int j=0;j<object_clusters[i].points.size();j++)
			    	 {
			    		 Point2f point;
			    		 point.x = object_clusters[i].points[j].x;
			    	     point.y = object_clusters[i].points[j].y;
			             points.push_back(point);
			    	 }
			    cluster_bb = minAreaRect(Mat(points));
			    float distance = sqrt((cluster_bb.center.x*cluster_bb.center.x)+(cluster_bb.center.y*cluster_bb.center.y));
				if (distance < dis_closest_local) dis_closest_local = distance;
		  }
	 }
	//cerr << "--end--";
	if (soon_obstacle == true)
		{
			decel=true;
			//cerr << " decel ";
			dis_closest_global = dis_closest_local;
		}
	else decel=false;
}

void vel_callback(const geometry_msgs::TwistStampedConstPtr& in_vel)
{
	new_vel = true;
	geometry_msgs::TwistStamped out_vel;
	if ((safe==false) || (decel==true))
	  {
		 out_vel.header = in_vel->header;
		 if (safe == false)
		  {
			 out_vel.twist.linear.x = 0.0;
			 out_vel.twist.angular.z = 0.0;
		  }
		 else if (decel == true)
		 {
			 out_vel.twist.linear.x = (dis_closest_global/3.0)*in_vel->twist.linear.x;
			 out_vel.twist.angular.z = (dis_closest_global/3.0)*in_vel->twist.angular.z;
		 }
	  }
	else out_vel = *in_vel;
	twist_cmd_out.publish(out_vel);
}

int main(int argc,char **argv)
{
	 ros::init(argc,argv,"safe_follow");
	 ros::NodeHandle nh;
	 ros::Subscriber safe_sub = nh.subscribe("/scan",2,laser_callback); // subscribe to urg node's topic
	 ros::Subscriber twist_cmd_in = nh.subscribe("/twist_cmd",2,vel_callback);
	 vis_redzone = nh.advertise<visualization_msgs::Marker>("/vis_red",1);
	 vis_outerzone = nh.advertise<visualization_msgs::Marker>("/viz_white",1);
	 cloud_pub = nh.advertise<sensor_msgs::PointCloud>("/2dpoints",1);
	 twist_cmd_out =  nh.advertise<geometry_msgs::TwistStamped>("/twist_cmd_safe",1);
	 int maxvel_count = 10;
	 int i=0;
	 ros::Rate r(40);
	 while (ros::ok())
	 {
        i++;
        ros::spinOnce();
        if (i==maxvel_count)
        {
            i=0;
            if (new_vel== true) new_vel = false;
            else
              {
                geometry_msgs::TwistStamped out_vel;
                out_vel.header.stamp = ros::Time::now();
                out_vel.header.frame_id = "/velodyne";
                out_vel.twist.linear.x = 0.0;
                out_vel.twist.angular.z = 0.0;
                twist_cmd_out.publish(out_vel);
             }
        }
        r.sleep();
	 }
	 return 0;
}
