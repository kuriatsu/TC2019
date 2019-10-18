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

/*シリアル通信用*/
int g_ser_port;
struct termios g_oldtio; //元のターミナルの設定
int   g_err_count;
int g_option;

double get_time(void){
  struct timeval current;
  
  gettimeofday(&current, NULL); 
   return  current.tv_sec + current.tv_usec/1000000.0;

}

/*----------------serial_connect------------------*/
/*ポートをオープンして 通信の準備をする*/
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
  
  tcgetattr(g_ser_port, &g_oldtio); /* 現在のシリアルポートの設定を待避させる*/
  bzero(&newtio, sizeof(newtio)); /* 新しいポートの設定の構造体をクリアする */

   
/* 
  BAUDRATE: ボーレートの設定．cfsetispeed と cfsetospeed も使用できる．
  CRTSCTS : 出力のハードウェアフロー制御 (必要な結線が全てされているケー
            ブルを使う場合のみ．Serial-HOWTO の7章を参照のこと)
  CS7     : 7n1 (7 ビット，ノンパリティ，ストップビット 1)
  CLOCAL  : ローカル接続，モデム制御なし
  CREAD   : 受信文字(receiving characters)を有効にする．
*/
   
  newtio.c_cflag = BAUDRATE |  CS8 | CLOCAL | CREAD | CRTSCTS  ;
  
  newtio.c_iflag = IGNPAR;   //パリティエラーのデータは無視する
  newtio.c_oflag = 1;        //Raw モードでの出力
  
  newtio.c_cc[VTIME]    = 0;     /* キャラクタ間タイマを使わない */
  newtio.c_cc[VMIN]     = 1;     /* 1文字来るまで，読み込みをブロックする */

  //モデムラインをクリアし，ポートの設定を有効にする
  tcflush(g_ser_port, TCIFLUSH);
  if(tcsetattr(g_ser_port,TCSANOW,&newtio)){
    fprintf(stdout,"ser set err");
    return 0;
  }

  /*チェック
    tcgetattr(ser_port,&newtio); 
    printf("%s\n",newtio.c_cflag&BAUDRATE?"ok":"failure");*/
  return 1;
}


/*----------------PBS_close------------------*/
int serial_close(void)
{
  //設定を元に戻す
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
