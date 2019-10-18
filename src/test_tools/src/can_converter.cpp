#include <ros/ros.h>
#include "test1/CANPacket.h"



void chatterCallback(const test1::CANPacket::ConstPtr& msg)
{
  unsigned short w;
  static int enc_sum;
  short diff;
  static short steer,shift,speed2,speed3,brake;
  static char  speed;
  static short enc,enc_p,enc_diff;
  static short wheel1,wheel2,wheel3,wheel4;
  static short gyro,accy,accz;
  static char accx; 
  int changed=0;

  if(msg->id==0x24){
    w=msg->dat[0]*256+msg->dat[1];
    gyro=w;
    w=msg->dat[2]*256+msg->dat[3];
    accy=w;
    w=msg->dat[4]*256+msg->dat[5];
    accz=w;
    w=msg->dat[7];
    accx=w;
    changed=1;
  }
  if(msg->id==0x25){
    w=msg->dat[0]*4096+msg->dat[1]*16;
    steer=w;
    steer=steer/16;
    changed=1;
  }

  if(msg->id==0xaa){
    w=msg->dat[0]*256+msg->dat[1];
    wheel1=w;
    w=msg->dat[2]*256+msg->dat[3];
    wheel2=w;
    w=msg->dat[4]*256+msg->dat[5];
    wheel3=w;
    w=msg->dat[6]*256+msg->dat[7];
    wheel4=w;
    changed=1;
  }
  if(msg->id==182){
    w=msg->dat[0]*256+msg->dat[1];
    speed3=w;
    changed=1;
  }
  if(msg->id==0x127){
    shift=msg->dat[3];
    speed=msg->dat[4];
    changed=1;
  }
  if(msg->id==0x230){
    w=msg->dat[0]*256+msg->dat[1];
    enc_p=enc;
    enc=w;
    diff=enc-enc_p;
    enc_diff=diff;
    enc_sum+=diff;
    changed=1;
  }
  if(changed){
    printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
	   msg->time,steer,shift,speed,speed2,speed3,enc_sum,enc_diff,brake,
	   wheel1,wheel2,wheel3,wheel4,
	   accx,accy,accz,gyro);
  }  
}


int main (int argc, char *argv[]){
  test1::CANPacket candat;

  ros::init(argc, argv, "can_converter");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("can_raw", 1000, chatterCallback);

  ros::spin();
}
