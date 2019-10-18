#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <ros/ros.h>
#include<cv.h>
#include<cxcore.h>
#include<highgui.h>

cv_bridge::CvImageConstPtr a_image;
sensor_msgs::CameraInfoConstPtr a_cameraInfo;
image_transport::Publisher pub; 

void imageCallback(const sensor_msgs::ImageConstPtr& msg)//,
//		   const sensor_msgs::CameraIncoConstPtr& info_msg)
{
  
  cv::Mat roi(cv_bridge::toCvShare(msg, "bgr8")->image, cv::Rect(0, 700, 1920, 400));	/*トリミング */
   // cv::Mat roi(cv_bridge::toCvShare(msg, "bgr8")->image, cv::Rect(0, 0, 1920, 1440));	/*トリミング */
   cv::Mat dst_img1,dst_img2;//2(roi.rows*0.5, roi.cols*0.5, roi.type());
  
   cv::resize(roi, dst_img1, cv::Size(), 0.5, 0.5);
   cv::Canny(dst_img1,dst_img2,20,100);
  // INTER_CUBIC（バイキュービック補間）でのサイズ変更
  // cv::resize(src_img, dst_img2, dst_img2.size(), cv::INTER_CUBIC);

  sensor_msgs::ImagePtr outmsg = cv_bridge::CvImage(std_msgs::Header(), "mono8",dst_img2).toImageMsg();
  //outmsg->header=msg->header;
  pub.publish(outmsg);


#if 0  
  static tf::TransformListener listenerCenter_;
  listenerCenter_.lookupTransform( "camera",   "japan_7", ros::Time(0), centerTransform);  
    
  double u,v;
  u=-1000;
  v=-1000;
  
  //tranform this point
  tf::Point tfPt(0,0,0);
  tf::Point tmpPt = centerTransform * tfPt;
  pcl::PointXYZ transformedPt;
  transformedPt.x = tmpPt.m_floats[0];
  transformedPt.y = tmpPt.m_floats[1];
  transformedPt.z = tmpPt.m_floats[2];
  
  cv::Point3d xyz(transformedPt.x, transformedPt.y, transformedPt.z);
  cv::Point2d point2d;  
  point2d = camModelCenter.project3dToPixel(xyz);
  
  CvScalar s;
  if (point2d.y < imageCenter.height &&  point2d.x < imageCenter.width && point2d.y >= 0 && point2d.x >= 0) {
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
#endif

  /* 結果表示 */
  try
  {
    cv::imshow("view",dst_img2); 
    printf("hoge\n");
    cv::waitKey(30);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_filter");
  ros::NodeHandle nh;
  cv::namedWindow("view");
  cv::startWindowThread();

  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("/camera1/image_color", 1, imageCallback); 
  //image_transport::CameraSubscriber image_sub;
  
  pub = it.advertise("/camera1/canny", 1);
  //image_sub = it.subscribeCamera("/camera/image_rect_color", 1, imageCallback);


  ros::spin();
  cv::destroyWindow("view");
}
