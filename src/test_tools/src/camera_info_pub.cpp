#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>

#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
int main(int argc, char** argv) {
 //std::cout << "calling init" << std::endl;
  ros::init (argc, argv,"camera_info_pub");
  //std::cout << "called init\n" << std::endl;
  
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<sensor_msgs::CameraInfo >("/camera1/camera_info", 1);
  ros::Publisher pub2 = nh.advertise<sensor_msgs::CameraInfo >("/camera2/camera_info", 1);

  sensor_msgs::CameraInfo ci,ci2;

  tf::Matrix3x3 cam1_m,cam2_m,velodyne_m, cam1_vel_m,cam2_vel_m;
  cam1_m.setValue( 0.02307281280,   0.02226822791,   0.99948575344,
	      -0.99965516953,  -0.01202377795,   0.02334460970,
	      0.01253743785,  -0.99967972611,   0.02198312677); 
  
  cam2_m.setValue(0.02418809565,  -0.00881855014,   0.99966852966,
		  0.99926710242,  -0.02946232015,  -0.02443828377,
		  0.02966806449,   0.99952699056,   0.00809944963);

  velodyne_m.setValue( 0.9992386965, 	0.0118209266, 	0.0371792024,  
		       -0.0121574637, 	0.9998870301, 	0.0088387299, 
		       -0.0370705203, 	-0.0092840057, 	0.9992695251);
  cam1_vel_m=velodyne_m.inverse()*cam1_m;
  cam2_vel_m=velodyne_m.inverse()*cam2_m;
  /*0.0230.09286390567,-0.35875796823, -0.92879977135,
       	     0.15092212321, 0.92712406466, -0.34302110934,
	     0.98417417551, -0.10832215359, 0.14024087597);*/
  double roll,pitch,yaw;
  velodyne_m.getRPY(roll,pitch,yaw);
  printf("velodyne %f %f %f\n",yaw,pitch,roll);
  cam1_m.getRPY(roll,pitch,yaw);
  printf("cam1 %f %f %f\n",yaw,pitch,roll);
  cam2_m.getRPY(roll,pitch,yaw);
  printf("cam1 %f %f %f\n",yaw,pitch,roll);
  
  cam1_vel_m.getRPY(roll,pitch,yaw);
  printf("cam1_vel %f %f %f\n",yaw,pitch,roll);
  cam2_vel_m.getRPY(roll,pitch,yaw);
  printf("cam2_vel %f %f %f\n",yaw,pitch,roll);
 
  ci.header.stamp=ros::Time::now();
  ci.header.seq=0;
  ci.header.frame_id="camera1";
  ci.height = 1440;
  ci.width = 1920;
  ci.K[0*3+0]= 1150.96938467;
  ci.K[0*3+1]=0.0;
  ci.K[0*3+2]=988.511326762;
  ci.K[1*3+0]=0.0;
  ci.K[1*3+1]=1150.96938467;  
  ci.K[1*3+2]=692.803953253;  
  ci.K[2*3+0]=0;
  ci.K[2*3+1]=0;
  ci.K[2*3+2]=1;
  ci.R[0*3+0]=1;
  ci.R[0*3+1]=0.0;
  ci.R[0*3+2]=0;
  ci.R[1*3+0]=0.0;
  ci.R[1*3+1]=1;
  ci.R[1*3+2]=0;
  ci.R[2*3+0]=0;
  ci.R[2*3+1]=0;
  ci.R[2*3+2]=1;
  ci.P[0*4+0]= 1150.96938467;
  ci.P[0*4+1]=0.0;
  ci.P[0*4+2]=988.511326762;
  ci.P[0*4+3]=0;
  ci.P[1*4+0]=0.0;
  ci.P[1*4+1]=1150.96938467;  
  ci.P[1*4+2]=692.803953253;
  ci.P[1*4+3]=0;
  ci.P[2*4+0]=0;
  ci.P[2*4+1]=0;
  ci.P[2*4+2]=1;
  ci.P[2*4+3]=0;
  ci.roi.height=ci.height;
  ci.roi.width=ci.width;

  ci2.header.stamp=ros::Time::now();
  ci2.header.seq=0;
  ci2.header.frame_id="camera2";
  ci2.height = 1440;
  ci2.width = 1920;
  ci2.K[0*3+0]=3093.975;
  ci2.K[0*3+1]=0.0;
  ci2.K[0*3+2]=1064.8007;
  ci2.K[1*3+0]=0.0;
  ci2.K[1*3+1]=3093.975;
  ci2.K[1*3+2]=661.8430;  
  ci2.K[2*3+0]=0;
  ci2.K[2*3+1]=0;
  ci2.K[2*3+2]=1;
  ci2.R[0*3+0]=1;
  ci2.R[0*3+1]=0.0;
  ci2.R[0*3+2]=0;
  ci2.R[1*3+0]=0.0;
  ci2.R[1*3+1]=1;
  ci2.R[1*3+2]=0;
  ci2.R[2*3+0]=0;
  ci2.R[2*3+1]=0;
  ci2.R[2*3+2]=1;
  ci2.P[0*4+0]=3093.975;
  ci2.P[0*4+1]=0.0;
  ci2.P[0*4+2]=1064.8007;
  ci2.P[0*4+3]=0;
  ci2.P[1*4+0]=0.0;
  ci2.P[1*4+1]=3093.975;
  ci2.P[1*4+2]=661.8430;
  ci2.P[1*4+3]=0;
  ci2.P[2*4+0]=0;
  ci2.P[2*4+1]=0;
  ci2.P[2*4+2]=1;
  ci2.P[2*4+3]=0;
  ci2.roi.height=ci2.height;
  ci2.roi.width=ci2.width;

  while(ros::ok()){
    pub.publish(ci);
    pub2.publish(ci2);
    usleep(20000);
    ros::spinOnce();
  }

  return 0;
}
