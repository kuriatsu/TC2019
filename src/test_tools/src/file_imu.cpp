/*-----------------------------------------
 * URG Simple Driver Program
 *               Eijiro Takeuchi
 *-----------------------------------------*/

/*-------------include files-----------------*/
#include <sys/types.h>
#include <sys/stat.h>

#include <math.h>
#include <sys/time.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include <ros/ros.h>
#include "test_tools/IMUPacket.h"

/*low level I/O*/
#include <fcntl.h>

/*serial*/
#include <sys/termios.h>

/*high level I/O*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*---------------defines--------------------*/

//#define DEBUG

#ifdef DEBUG
#define DB(x) x
#else
#define DB(x)
#endif

int  main(int argc ,char *argv[])
{
  //IMU_Wheel imu;
  int i;
  test_tools::IMUPacket imu;
  FILE *fp;
  ros::init(argc, argv, "h8_imu");
  ros::NodeHandle n;
  ros::Publisher imu_pub = n.advertise<test_tools::IMUPacket>("imu_wheel", 10);

  fp=fopen(argv[1],"r");

  while(ros::ok()){
    double time;
    int wx,wy,wz,ax,ay,az,wheel;
    if(fscanf(fp,"%lf %d %d %d %d %d %d %d",
	      &time, &wx,&wy, &wz, &ax, &ay, &az, &wheel)==EOF)break;
    imu.wx=wx;
    imu.wy=wy;
    imu.wz=wz;
    imu.ax=ax;
    imu.ay=ay;
    imu.az=az;
    imu.wheel=wheel;

    while(ros::Time::now().toSec() < time)usleep(1000);
    ros::Time now=ros::Time::now();
    
    now += ros::Duration(time - now.toSec());

    imu.header.stamp=now;

    imu_pub.publish(imu);
    ros::spinOnce();
  }

  return 1;
} 
