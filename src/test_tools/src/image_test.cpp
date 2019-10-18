#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

cv_bridge::CvImageConstPtr a_image;
sensor_msgs::CameraInfoConstPtr a_cameraInfo;

void imageCallback(const sensor_msgs::ImageConstPtr& msg,
		   const sensor_msgs::CameraIncoConstPtr& info_msg)
{
  try
  {
    cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
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
  //  image_transport::Subscriber sub = it.subscribe("/camera/image_rect_color", 1, imageCallback); 
  image_transport::CameraSubscriber image_sub;
  
  image_sub = it.subscribeCamera("/camera/image_rect_color", 1, imageCallback);


  ros::spin();
  cv::destroyWindow("view");
}
