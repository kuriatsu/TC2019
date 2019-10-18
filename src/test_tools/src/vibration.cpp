#include <sys/types.h>
#include <sys/stat.h>

/*low level I/O*/
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <sys/time.h>
#include <time.h>
/*serial*/
#include <sys/termios.h>
#include <pthread.h>
/*high level I/O*/
#include <stdio.h>
#include <math.h>

#include <ros/ros.h>
#include "test1/Vibration.h"

//#include <serial.h>

#define SECOND 1000000.0
//#define BAUDRATE B115200
#define BAUDRATE B38400
//B9600

/*���ꥢ���̿���*/
int g_ser_port;
struct termios g_oldtio; //���Υ����ߥʥ������
int   g_err_count;
int g_option;

double get_time(void){
  struct timeval current;
  
  gettimeofday(&current, NULL); 
   return  current.tv_sec + current.tv_usec/1000000.0;

}

/*----------------serial_connect------------------*/
/*�ݡ��Ȥ򥪡��ץ󤷤� �̿��ν����򤹤�*/
int serial_connect(int port)
{
  struct termios newtio;

  if(port == 0){
    g_ser_port = open( "/dev/ttyACM0",O_RDWR);
  }else{
    g_ser_port = open( "/dev/ttyS0",O_RDWR);
  }
   
  if(g_ser_port < 0){
    fprintf(stderr,"Can't open serial port.\n");
    return 0;
  }
  
  tcgetattr(g_ser_port, &g_oldtio); /* ���ߤΥ��ꥢ��ݡ��Ȥ���������򤵤���*/
  bzero(&newtio, sizeof(newtio)); /* �������ݡ��Ȥ�����ι�¤�Τ򥯥ꥢ���� */

   
/* 
  BAUDRATE: �ܡ��졼�Ȥ����ꡥcfsetispeed �� cfsetospeed ����ѤǤ��롥
  CRTSCTS : ���ϤΥϡ��ɥ������ե����� (ɬ�פʷ��������Ƥ���Ƥ��륱��
            �֥��Ȥ����Τߡ�Serial-HOWTO ��7�Ϥ򻲾ȤΤ���)
  CS7     : 7n1 (7 �ӥåȡ��Υ�ѥ�ƥ������ȥåץӥå� 1)
  CLOCAL  : ��������³����ǥ�����ʤ�
  CREAD   : ����ʸ��(receiving characters)��ͭ���ˤ��롥
*/
   
  newtio.c_cflag = BAUDRATE |  CS8 | CLOCAL | CREAD | CRTSCTS  ;
  
  newtio.c_iflag = IGNPAR;   //�ѥ�ƥ����顼�Υǡ�����̵�뤹��
  newtio.c_oflag = 1;        //Raw �⡼�ɤǤν���
  
  newtio.c_cc[VTIME]    = 0;     /* ����饯���֥����ޤ�Ȥ�ʤ� */
  newtio.c_cc[VMIN]     = 1;     /* 1ʸ�����ޤǡ��ɤ߹��ߤ�֥�å����� */

  //��ǥ�饤��򥯥ꥢ�����ݡ��Ȥ������ͭ���ˤ���
  tcflush(g_ser_port, TCIFLUSH);
  if(tcsetattr(g_ser_port,TCSANOW,&newtio)){
    fprintf(stdout,"ser set err");
    return 0;
  }

  /*�����å�
    tcgetattr(ser_port,&newtio); 
    printf("%s\n",newtio.c_cflag&BAUDRATE?"ok":"failure");*/
  return 1;
}


/*----------------PBS_close------------------*/
int serial_close(void)
{
  //����򸵤��᤹
  //  tcsetattr(ser_port,TCSANOW,&g_oldtio);
  close(g_ser_port);
  return 1;
}


int init_serial(void){
  g_err_count = 0;
  return serial_connect(0);
}

void finalize_serial(void){
  serial_close();
}


double pwm[8];

void VibrationCallback(const test1::Vibration::ConstPtr& msg){
  printf("");
  for(int i=0;i<8;i++){
    pwm[i]=(double)msg->pwm[i]/255.;
    printf("%f ",pwm[i]);
  }
  printf("\n");
}

/*main*/
int main(int argc, char *argv[]){
  static char buf[1000];
  int count,len;
  ros::init(argc, argv, "h8_imu");
  ros::NodeHandle n;
  ros::Subscriber vib_sub = n.subscribe("vibration", 1, VibrationCallback);
 

  if(!init_serial()){
    printf("stop.\n");
    return 0;
  }
  
  printf("start.\n");
  while(ros::ok()){ 
    len=sprintf(buf,"%4.3f %4.3f %4.3f %4.3f\r\n",pwm[0],pwm[1],pwm[2],pwm[3]);
    printf("%s <%d\n",buf,len);
    write(g_ser_port,buf,len+1);   
    usleep(20000);
    ros::spinOnce();
  }
  
  return 1;
}
