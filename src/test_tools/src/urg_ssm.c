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

#include<ssm_common.h>

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

/*PBS serial connect speed*/
#define BAUDRATE B57600

#define BUF_SIZE 4624
volatile int STOP=0; 

/*------------global variables------------------*/
/*���ꥢ���̿�*/
int ser_port;
struct termios oldtio; //���Υ����ߥʥ������


typedef struct {
  unsigned short wx;
  unsigned short wy;
  unsigned short wz;  
  unsigned short gx;
  unsigned short gy;
  unsigned short gz;  
  unsigned short wheel;
}IMU_Wheel;

/*-------------function prototypes------------------------*/
int URG_connect(char*);
int URG_close(void);


/*----------------PBS_connect------------------*/
/*�ݡ��Ȥ򥪡��ץ󤷤ơ�PBS �Ȥ��̿��ν����򤹤�*/
int URG_connect(char* port)
{
  struct termios newtio;
  
  ser_port = open(port,O_RDWR/*|O_NONBLOCK*/);
  
  if(ser_port < 0){
    fprintf(stderr,"Can't open serial port.\n");
    return 0;
  }  
  tcgetattr(ser_port,&oldtio); /* ���ߤΥ��ꥢ��ݡ��Ȥ���������򤵤���*/
  bzero(&newtio, sizeof(newtio)); /* �������ݡ��Ȥ�����ι�¤�Τ򥯥ꥢ���� */

  newtio.c_cflag = BAUDRATE |  CS8 | CLOCAL | CREAD;// | CRTSCTS  ;
  newtio.c_iflag = IGNPAR;   //�ѥ�ƥ����顼�Υǡ�����̵�뤹��
  newtio.c_oflag = 1;        //Raw �⡼�ɤǤν���
  newtio.c_cc[VTIME]    = 20;     /* ����饯���֥����ޤ�Ȥ�ʤ� */
  newtio.c_cc[VMIN]     = 0;     /* 1ʸ�����ޤǡ��ɤ߹��ߤ�֥�å����� */
  
  //��ǥ�饤��򥯥ꥢ�����ݡ��Ȥ������ͭ���ˤ���
  tcflush(ser_port, TCIFLUSH);
  if(tcsetattr(ser_port,TCSANOW,&newtio)){
    fprintf(stderr,"ser set err");
    return 0;
  }
  
   /*�����å�
    tcgetattr(ser_port,&newtio); 
    printf("%s\n",newtio.c_cflag&BAUDRATE?"ok":"failure");*/
  return 1;
}


/*----------------PBS_close------------------*/
int URG_close(void)
{
  //����򸵤��᤹
  tcsetattr(ser_port,TCSANOW,&oldtio);
  close(ser_port);
  return 1;
}


int  main(int argc ,char *argv[])
{
  unsigned char b,dat[18];
  unsigned short sens[7];
  IMU_Wheel imu;
  int i;
  SSM_sid  imu_sid;
  int count,edge_count,prev_edge_count,edge_diff;
  unsigned short pc,cc;
  unsigned short state;

  initSSM();
  imu_sid=createSSM("IMU_Wheel",0,sizeof(IMU_Wheel),5,1/500.0);
  state=0;

  cc=0;
  count=0;
  edge_count=0;
  prev_edge_count=0;
  /*init URG*/
  URG_connect("/dev/ttyUSB0");
  while(1){
    read(ser_port,&b,1);
    //printf("%c\n",b);
    if(b==':'){
      for(i=0;i<14;i++)read(ser_port,dat+i,1);
      for(i=0;i<3;i++){
	sens[i]=(dat[i*4+0]<<8)+dat[i*4+1];
	sens[i+3]=(dat[i*4+2]<<8)+dat[i*4+3];
      }
      sens[6]=(dat[12]<<8)+dat[13];

      imu.wx = sens[0];
      imu.wy = sens[1];
      imu.wz = sens[2];
      imu.gx = sens[3];
      imu.gy = sens[4];
      imu.gz = sens[5];
      imu.wheel= sens[6];
      writeSSM(imu_sid,(char*)&imu,gettimeSSM());

      pc = cc;
      cc=sens[6]&0x3fff;
      if(sens[6]&0x4000){//OVF�Ǥ����
	if(state != (sens[6]&0x8000)){//edge
	  if(cc>=0x2000){//OVF�Τޤ��˥��å�
	    edge_count = count+cc;  
	    count+=0x4000;
	  }else{//OVF�θ�˥��å�
	    count+=0x4000;
	    edge_count = count+cc;
	  }
	  edge_diff = edge_count-prev_edge_count;
	  prev_edge_count =edge_count;
	  state = sens[6]&0x8000;
	}else{
	  count+=0x4000;
	}
      }else{//OVF�Ǥʤ����
	if(state != (sens[6]&0x8000)){//edge
	  edge_count = count+cc;
	  edge_diff = edge_count-prev_edge_count;
	  prev_edge_count =edge_count;
	  state = sens[6]&0x8000;
	}
      }
      printf("%d %d %d %d %d %d %d \n",
	     (short)sens[0],(short)sens[1],(short)sens[2],
	     (short)sens[3],(short)sens[4],(short)sens[5],
	     //(sens[6]&0x8000)>>15,(sens[6]&0x4000)>>14,
	     (unsigned short)(sens[6]));
	     //edge_count,edge_diff);
	     }
  }
  URG_close();
  
  return 1;
} 
