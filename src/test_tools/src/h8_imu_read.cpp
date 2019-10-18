#include <ros/ros.h>
#include "test_tools/IMUPacket.h"

void imuCallback(const test_tools::IMUPacket::ConstPtr& msg){
  printf("%f\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
	 msg->header.stamp.toSec(),
	 (short)msg->wx,(short)msg->wy,(short)msg->wz,
	 (short)msg->ax,(short)msg->ay,(short)msg->az,(short)msg->wheel);
}

int main(int argc, char** argv){
  ros::init(argc, argv, "h8_imu_read");
  ros::NodeHandle n;

  ros::Subscriber imu_sub = n.subscribe("imu_wheel", 1000, imuCallback);

  ros::spin();
}
