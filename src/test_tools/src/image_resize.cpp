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
  
  //   cv::Mat roi(cv_bridge::toCvShare(msg, "bgr8")->image, cv::Rect(0, 0, 1920, 1000));	/*トリミング */
  //cv::Mat roi(cv_bridge::toCvShare(msg, "bgr8")->image, cv::Rect(0, 0, 1920, 1440));	/*トリミング */
  cv::Mat dst_img1;//2(roi.rows*0.5, roi.cols*0.5, roi.type());
  
  cv::resize(cv_bridge::toCvShare(msg, "bgr8")->image, dst_img1, cv::Size(), 0.5, 0.5);
  // INTER_CUBIC（バイキュービック補間）でのサイズ変更
  // cv::resize(src_img, dst_img2, dst_img2.size(), cv::INTER_CUBIC);

  sensor_msgs::ImagePtr outmsg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", dst_img1).toImageMsg();
  pub.publish(outmsg);
  /* 結果表示 */
  try
  {
    cv::imshow("view",dst_img1); 
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
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  cv::namedWindow("view");
  cv::startWindowThread();

  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("/camera1/image_raw", 1, imageCallback); 
  //image_transport::CameraSubscriber image_sub;
  
  pub = it.advertise("camera/image_raw", 1);
  //image_sub = it.subscribeCamera("/camera/image_rect_color", 1, imageCallback);


  ros::spin();
  cv::destroyWindow("view");
}
