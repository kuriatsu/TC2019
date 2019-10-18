#include <ros/ros.h>
#include "test_tools/IMUPacket.h"

void imuCallback(const test_tools::IMUPacket::ConstPtr& msg){
  static double prev_time;
  static int prev_count;
  short diff;

  double wx,wy,wz;
  double dt= msg->header.stamp.toSec()-prev_time;
  static double theta;
  static double x,y,v;
  static double woffset=0.0034;

  if(dt>0.1||dt<-0.1)dt=0;
  diff=(short)msg->wheel-prev_count;
  dt=0.003;
  if(dt>0.0001)v = (-75./300000.)*diff/dt;
  else v=0;



  wx= (0.03125*M_PI/180.0)*(short)msg->wx;
  wy= -(0.03125*M_PI/180.0)*(short)msg->wy;
  wz= (0.03125*M_PI/180.0)*(short)msg->wz;

  //  if(v<0.00001&&v>-0.00001)woffset=woffset*0.9999+wy*0.0001;
  wy-=woffset;
  
  x+= v*dt*cos(theta);
  y+= v*dt*sin(theta);
  theta += wy*dt;

  printf("%f %f %f %f %f %f %f %d %d %d %d %f %d %f\n",
	 msg->header.stamp.toSec(),
	 wx,wy,wz,x-1*sin(theta),y+1*cos(theta),theta, 
	 (short) msg->ax,(short)msg->ay,(short)msg->az,msg->wheel,
	 dt,diff,v);
  prev_count=msg->wheel;
  prev_time=msg->header.stamp.toSec();
}

int main(int argc, char** argv){
  ros::init(argc, argv, "h8_imu_read");
  ros::NodeHandle n;
 
  ros::Subscriber imu_sub = n.subscribe("imu_wheel", 1000, imuCallback);

  ros::spin();
}
