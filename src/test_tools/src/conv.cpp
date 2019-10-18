#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "ros/ros.h"
#include <tf/transform_listener.h>
#include <iostream>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream> 
#include<vector>

using namespace std;


typedef struct{
  double t;
  double x;
  double y;
  double z;
  double a;
  double b;
  double g;
  double q[4];
}Pose;

vector<Pose> xp;
vector<Pose> yp;

bool getCsv(const char *csv_file, vector<vector<std::string > >& data, const char delim)
{
  // �ե�����OPEN
  ifstream ifs(csv_file);
  if (!ifs.is_open()) return false;  // �ɤ߹��߼���
  
  // �ե�����READ
  string buf;                 // 1��ʬ�Хåե�
  while (std::getline(ifs, buf)) {
    vector<string> rec;     // 1��ʬ�٥���
    istringstream iss(buf); // ʸ���󥹥ȥ꡼��
    string field;           // 1��ʬʸ����
    
    // 1��ʬʸ�����1��ʬ�٥������ɲ�
    while (std::getline(iss, field, delim)) rec.push_back(field);

    // ����ʬ�٥����� data �٥������ɲ�
    if (rec.size() != 0) data.push_back(rec);
  }
  
  return true;  // �ɤ߹�������
}

/*
void  importData(string filename,vector<Pose> xarr)
{
  ifstream ifs( filename );
  stringstream hoge,huga;
  if( !ifs ) {
    cout << "Error:Input data file not found" << endl;
    return;
  }
 
 while( getline( ifs, str ) ){
   string token;
   stringstream ss, ss2;
   
   istringstream stream( str );
   
   while( getline( stream, token, ',' ) ) {
     ss << token;
     cout<<ss<<endl;
   }
   ss2 << token;
   cout<<ss2<<endl;
	

   tf::Quaternion q(0,0,0,0);
   double roll,pitch,yaw;
   tf::Matrix3x3 m(q);
   m.getRPY(roll,pitch,yaw);
   

//   tf::Transform transform;    
//  tf::Quaternion q;
//  transform.setOrigin( 0,0,0);
// q.setRPY(0,0,0);
//   transform.setRotation(q);

 }
 // cout >> hoge >> huga >> endl;
 return;
}
*/
int main(int argc, char* argv[]){
  // importData(argv[1],xp);
  vector<vector<std::string > > data;
  int i=0;
  getCsv(argv[1],data,',');
 
  for(vector<vector<string> >::iterator it=data.begin();it!=data.end();it++){
    i=0;
    double t,x,y,z,q[4];
    for(vector<string >::iterator it2=it->begin(); it2!=it->end();it2++){
      if(i==2)t=strtod(it2->c_str(),0)/1000000000.0;
      if(i==5)x=strtod(it2->c_str(),0);
      if(i==6)y=strtod(it2->c_str(),0);
      if(i==7)z=strtod(it2->c_str(),0);
      if(i==8)q[0]=strtod(it2->c_str(),0);
      if(i==9)q[1]=strtod(it2->c_str(),0);
      if(i==10)q[2]=strtod(it2->c_str(),0);
      if(i==11)q[3]=strtod(it2->c_str(),0);
      i++;
    }

    tf::Quaternion qt(q[0],q[1],q[2],q[3]);
    double roll,pitch,yaw;
    tf::Matrix3x3 m(qt);
    m.getRPY(roll,pitch,yaw);
    printf("%f %f %f %f %f %f %f\n",t,x,y,z,roll,pitch,yaw);
  }
  
  return 1;
}

