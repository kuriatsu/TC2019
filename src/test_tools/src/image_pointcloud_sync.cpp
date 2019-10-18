#include <ros/ros.h>
#include "velodyne_pointcloud/point_types.h"
#include "velodyne_pointcloud/rawdata.h"
#include <image_geometry/pinhole_camera_model.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_ros/transforms.h>
#include "sensor_msgs/PointCloud2.h"
#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>
#include <cv_bridge/cv_bridge.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <boost/circular_buffer.hpp>

#include <trigger_box/GetLastSyncboxTime.h>
#include <trigger_box/GetParameters.h>
#include <trigger_box/GetReferenceTime.h>
#include <trigger_box/GPSToROSTime.h>
#include <trigger_box/GPSToROSTime2.h>  // GPS時刻を文字列で指定する版
#include <trigger_box/GetTriggerTime.h>
#include <trigger_box/GetTriggerTime2.h>  // トリガーIDを文字列で指定する版

// 当処理が提供するサービス（基準同期カウンタを更新しない版）
#include <trigger_box/GPSToROSTime_NoUpdate.h>
#include <trigger_box/GPSToROSTime2_NoUpdate.h>  // GPS時刻を文字列で指定する版
#include <trigger_box/GetTriggerTime_NoUpdate.h>
#include <trigger_box/GetTriggerTime2_NoUpdate.h>  // トリガーIDを文字列で指定する版



#define NODE "rgb_pointcloud_publisher"
#define SINGLE_WIDTH 1280
#define SINGLE_HEIGHT 960
#define PI 3.14159265

//typedef pcl::PointXYZ VPoint;
//typedef pcl::PointCloud<VPoint> VPointCloud;

pcl::PointCloud<pcl::PointXYZRGB> rgbPc_;

//boost::circular_buffer<ParticleFilter::Particles> particles_buffer(100);

ros::Publisher velodyne_pub_;
ros::ServiceClient ref_srv;
ros::ServiceClient trg_srv;
ros::ServiceClient gps_srv;

bool center_ready = false;


image_geometry::PinholeCameraModel camModelCenter;

tf::StampedTransform centerTransform;
static IplImage imageCenter;

sensor_msgs::ImageConstPtr imgCenter;

//sensor_msgs::cv_bridge bridgeCenter;

long long int get_ref_time(){
  trigger_box::GetLastSyncboxTime srv;

  client.call(srv);

  return srv.response.lastSyncboxTime();
}  

double get_trg_time(int id, ros::Time time, long long int ref_time){
  trigger_box::GetTriggerTime srv;

  srv.request.triggerID=id;
  srv.request.rosTime= time;//ros::Time::Time(time);
  srv.request.triggerTimer=ref_time;

  trg_srv.call(srv);

  return srv.response.returnTime.toSec();
}

double get_gps_time(double time, long long int ref_time){
  trigger_box::GPSToROSTime srv;

  srv.request.gpsTime=ros::Time::Time(time);
  srv.request.triggerTimer=ref_time;

  trg_srv.call(srv);

  return srv.response.returnTime.toSec();
}

void callback(const pcl::PointCloud<velodyne_pointcloud::PointXYZIR>::ConstPtr& msg){
  static tf::TransformListener listenerCenter_;    

  try
  {   
    rgbPc_.points.resize(msg->points.size());
    if (center_ready == false ) //|| right_ready == false)
      return;
    printf("hoge\n");
      ros::Time veltime;    
    pcl_conversions::fromPCL(msg->header.stamp,veltime);    
    char filename_velodyne[100];//,filename_img[100];    
    cv::Mat cppMat =cv_bridge::toCvShare(imgCenter, "bgr8")->image;
    imageCenter=cppMat;
    printf("image copy\n");
    listenerCenter_.lookupTransform( "camera",   "velodyne", ros::Time(0), centerTransform);  
    printf("get transform\n");  
    
    for (size_t a = 0; a < msg->points.size(); ++a)
      {
	//velodyne_pointcloud::PointXYZIR 
	pcl::PointXYZ pt;
	pt.x= msg->points[a].x; //a point in the pointcloud
	pt.y= msg->points[a].y; //a point in the pointcloud
	pt.z= msg->points[a].z; //a point in the pointcloud

	//initialize each point to black
	rgbPc_.points[a].x = pt.x;
	rgbPc_.points[a].y = pt.y;
	rgbPc_.points[a].z = pt.z;
	uint32_t black = ((uint32_t)0 << 16 | (uint32_t)0 << 8 | (uint32_t)0);
	rgbPc_.points[a].rgb = *reinterpret_cast<float*>(&black);
                    
	double angle = getAngle(pt.x, pt.y);
	double u,v;

	u=-1000;
	v=-1000;
	if (angle < 30 && angle > 0)
	  {                 
	     //tranform this point
	     tf::Point tfPt(pt.x, pt.y, pt.z);
	     tf::Point tmpPt = centerTransform * tfPt;
	     pcl::PointXYZ transformedPt;
	     transformedPt.x = tmpPt.m_floats[0];
	     transformedPt.y = tmpPt.m_floats[1];
	     transformedPt.z = tmpPt.m_floats[2];
	     
	     //find rgb value for this point in center image                  
	     cv::Point3d xyz(transformedPt.x, transformedPt.y, transformedPt.z); //a Point3d of the point in the pointcloud
	     cv::Point2d point2d;  //will hold the 2d point in the image
	     point2d = camModelCenter.project3dToPixel(xyz);
	     
	     //get rgb from 2d point
	     CvScalar s;
	     if (point2d.y < imageCenter.height &&  point2d.x < imageCenter.width && point2d.y >= 0 && point2d.x >= 0) 
	       {
		 s=cvGet2D(&imageCenter, point2d.y, point2d.x); // get the (i,j) pixel value  
		 uint32_t b = s.val[0];
		 uint32_t g = s.val[1];
		 uint32_t r = s.val[2];
		 u=point2d.x;
		 v=point2d.y;		 
		 //give the new point the rgb value from the image and the original, untransformed x, y, z points
		 uint32_t rgb = ((uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b);
		 	 rgbPc_.points[a].rgb = *reinterpret_cast<float*>(&rgb);		      
	       }
	   }
  
	//fprintf(fp,"%f %f %f  %f %f %d \n",msg->points[a].x,msg->points[a].y, pt.z= msg->points[a].z,u,v,msg->points[a].ring);	
      }
    //    fclose(fp);
    rgbPc_.header.stamp = msg->header.stamp;
    rgbPc_.header.frame_id = "velodyne";
    
    //std::cout <<"publishing " << rgbPc_.points.size() << std::endl;
    center_ready = false;

    velodyne_pub_.publish(rgbPc_);

      //      cvSaveImage("test.bmp",&cppMat);//imageCenter);
      
      //      imwrite(filename_img,cppMat);//imageCenter);
  }
  catch (tf::TransformException ex)
  {
      // only log tf error once every 20 times
    ROS_WARN_THROTTLE(20, "%s", ex.what());
      return;   // skip this packet
  }    
}


void processImageCenter(const sensor_msgs::ImageConstPtr &msg, const sensor_msgs::CameraInfoConstPtr& info_msg)
{

//  std::cout << "got a center image" << std::endl;
  camModelCenter.fromCameraInfo(info_msg);
  imgCenter = msg;
  center_ready = true;
  
  cv::Mat cppMat =cv_bridge::toCvShare(imgCenter, "bgr8")->image;
  imageCenter=cppMat;
  char filename_img[100];    
  sprintf(filename_img,"/tmp/%.3f_camera.ppm",imgCenter->header.stamp.toSec());
  imwrite(filename_img,cppMat);//imageCenter);
  
  printf("hoga\n");
}


int main(int argc, char** argv) {
  //std::cout << "calling init" << std::endl;
  ros::init (argc, argv, NODE);
  //std::cout << "called init\n" << std::endl;  
  ros::NodeHandle nh;

  
  ref_srv = n.serviceClient<trigger_box::GetLastSyncboxTime>("get_last_syncbox_time");
  trg_srv = n.serviceClient<trigger_box::GetTriggerTime>("get_trigger_time");


  velodyne_pub_ = nh.advertise<pcl::PointCloud<pcl::PointXYZRGB> >("velodyne_points/rgb_pointcloud", 1);

  ros::Subscriber sub = nh.subscribe("velodyne_points/labeled_pointcloud", 10, callback);
  
  //subscribe to image
  static int qDepth_ = 10; // ROS topic queue size
  image_transport::ImageTransport it(nh);

  std::string topicCenter = nh.resolveName("/camera1/image_raw");
  image_transport::CameraSubscriber cameraCenter = it.subscribeCamera(topicCenter, qDepth_, &processImageCenter);




  printf("get\n");
  ros::spin();

  return 0;
}
