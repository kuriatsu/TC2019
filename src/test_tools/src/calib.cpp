#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include"algebra.h"
#include<unistd.h>
#include "ros/ros.h"
#include <tf/transform_listener.h>

#define EPS 0.00001
typedef struct{
  double t;
  double x;
  double y;
  double z;
  double a;
  double b;
  double g;
}Pose;


void set_sincos2(double a,double b,double g,double sc[3][3]){
  double sa,ca,sb,cb,sg,cg;
  double Rx[3][3],Ry[3][3],Rz[3][3],R[3][3],W[3][3];
  sa = sin(a);
  ca = cos(a);
  sb = sin(b);
  cb = cos(b);
  sg = sin(g);
  cg = cos(g);

  Rx[0][0]=1;
  Rx[0][1]=0;
  Rx[0][2]=0;
  Rx[1][0]=0;
  Rx[1][1]=ca;
  Rx[1][2]=-sa;
  Rx[2][0]=0;
  Rx[2][1]=sa;
  Rx[2][2]=ca;

  Ry[0][0]=cb;
  Ry[0][1]=0;
  Ry[0][2]=sb;
  Ry[1][0]=0;
  Ry[1][1]=1;
  Ry[1][2]=0;
  Ry[2][0]=-sb;
  Ry[2][1]=0;
  Ry[2][2]=cb;

  Rz[0][0]=cg;
  Rz[0][1]=-sg;
  Rz[0][2]=0;
  Rz[1][0]=sg;
  Rz[1][1]=cg;
  Rz[1][2]=0;
  Rz[2][0]=0;
  Rz[2][1]=0;
  Rz[2][2]=1;

  identity_matrix3d(W);
  mux_matrix3d(W,Rz,R);
  mux_matrix3d(R,Ry,W);
  mux_matrix3d(W,Rx,sc);

}

void set_sincos2_inv(double a,double b,double g,double sc[3][3]){
  double sa,ca,sb,cb,sg,cg;
  double Rx[3][3],Ry[3][3],Rz[3][3],R[3][3],W[3][3];
  sa = sin(a);
  ca = cos(a);
  sb = sin(b);
  cb = cos(b);
  sg = sin(g);
  cg = cos(g);

  Rx[0][0]=1;
  Rx[0][1]=0;
  Rx[0][2]=0;
  Rx[1][0]=0;
  Rx[1][1]=ca;
  Rx[1][2]=-sa;
  Rx[2][0]=0;
  Rx[2][1]=sa;
  Rx[2][2]=ca;

  Ry[0][0]=cb;
  Ry[0][1]=0;
  Ry[0][2]=sb;
  Ry[1][0]=0;
  Ry[1][1]=1;
  Ry[1][2]=0;
  Ry[2][0]=-sb;
  Ry[2][1]=0;
  Ry[2][2]=cb;

  Rz[0][0]=cg;
  Rz[0][1]=-sg;
  Rz[0][2]=0;
  Rz[1][0]=sg;
  Rz[1][1]=cg;
  Rz[1][2]=0;
  Rz[2][0]=0;
  Rz[2][1]=0;
  Rz[2][2]=1;

  identity_matrix3d(W);
  mux_matrix3d(W,Rx,R);
  mux_matrix3d(R,Ry,W);
  mux_matrix3d(W,Rz,sc);

}

void set_initial_pos(Pose *xp,int start,int num){
  Pose xi;
  int i;
  double sci[3][3],sc[3][3],W[3][3],x,y,z;

  tf::Transform transform,transform2,transform3;    
  tf::Quaternion q;
 
  xi=xp[start];

  transform.setOrigin(tf::Vector3(xi.x,xi.y,xi.z));
  q.setRPY(xi.a,xi.b,xi.g);
  transform.setRotation(q);
 
   set_sincos2_inv(-xi.a, -xi.b, -xi.g, sci);
   for(i=start;i<num;i++){
     transform2.setOrigin(tf::Vector3(xp[i].x,xp[i].y,xp[i].z));
     q.setRPY(xp[i].a,xp[i].b,xp[i].g);
     transform2.setRotation(q);
  
     transform3=transform.inverse()*transform2;

     tf::Quaternion qt(transform3.getRotation().x(),
		       transform3.getRotation().y(),
		       transform3.getRotation().z(),
		       transform3.getRotation().w());
     double roll,pitch,yaw;
     tf::Matrix3x3 m(qt);
     m.getRPY(roll,pitch,yaw);
          
     xp[i].x = transform3.getOrigin().x();//xp[i].x-xi.x;
     xp[i].y = transform3.getOrigin().y();//xp[i].y-xi.y;
     xp[i].z = transform3.getOrigin().z();//xp[i].z-xi.z;
     xp[i].a = roll;//xp[i].a-xp[0].a;//asin(W[2][1]);
     xp[i].b = pitch;//xp[i].b-xp[0].b;//atan2(W[2][2],-W[2][0]);
     xp[i].g = yaw;//xp[i].g-xp[0].g;//atan2(W[1][1],-W[0][1]);
  }
   //  printf("%f %f %f %f %f %f\n",
   //	 xp[start].x,xp[0].y,xp[0].z,xp[0].a,xp[0].b,xp[0].g);
  
}

void calc_sensor_pos(Pose *x,Pose *y,Pose *t,double scale,int num){
  double sc[3][3],sct[3][3];
  int i;
  double lx,ly,lz;

  tf::Transform tf_t,tf_x,tf_y,tf_w;
  tf::Quaternion q;

  tf_t.setOrigin(tf::Vector3(t->x,t->y,t->z));
  q.setRPY(t->a,t->b,t->g);
  tf_t.setRotation(q);

  set_sincos2_inv(-t->a, -t->b, -t->g, sct);
  
  for(i=0;i< num;i++){
    tf_x.setOrigin(tf::Vector3(x[i].x,x[i].y,x[i].z));
    q.setRPY(x[i].a,x[i].b,x[i].g);
    tf_x.setRotation(q);

    tf_y=tf_t.inverse() * tf_x * tf_t;
    
    tf::Quaternion qt(tf_y.getRotation().x(),
		      tf_y.getRotation().y(),
		      tf_y.getRotation().z(),
		      tf_y.getRotation().w());
     double roll,pitch,yaw;
     tf::Matrix3x3 m(qt);
     m.getRPY(roll,pitch,yaw);
          
     y[i].x = tf_y.getOrigin().x()*scale;//xp[i].x-xi.x;
     y[i].y = tf_y.getOrigin().y()*scale;//xp[i].y-xi.y;
     y[i].z = tf_y.getOrigin().z()*scale;//xp[i].z-xi.z;
     y[i].a = roll;//xp[i].a-xp[0].a;//asin(W[2][1]);
     y[i].b = pitch;//xp[i].b-xp[0].b;//atan2(W[2][2],-W[2][0]);
     y[i].g = yaw;//xp[i].g-xp[0].g;//atan2(W[1][1],-W[0][1]);

    /*
    set_sincos2(x[i].a, x[i].b, x[i].g, sc);
  
    lx = t->x*sc[0][0] + t->y*sc[0][1]+ t->z*sc[0][2] + x[i].x*scale-t->x;
    ly = t->x*sc[1][0] + t->y*sc[1][1]+ t->z*sc[1][2] + x[i].y*scale-t->y;
    lz = t->x*sc[2][0] + t->y*sc[2][1]+ t->z*sc[2][2] + x[i].z*scale-t->z;
    
    y[i].x = (lx*sct[0][0] + ly*sct[0][1]+ lz*sct[0][2]);
    y[i].y = (lx*sct[1][0] + ly*sct[1][1]+ lz*sct[1][2]);
    y[i].z = (lx*sct[2][0] + ly*sct[2][1]+ lz*sct[2][2]);
    y[i].a = t->a + x[i].a;
    y[i].b = t->b + x[i].b;
    y[i].g = t->g + x[i].g;
    */
}


}



double err(Pose pyd,Pose py){
  double err=
    (py.x-pyd.x)*(py.x-pyd.x)+
    (py.z-pyd.z)*(py.z-pyd.z)+
    (py.y-pyd.y)*(py.y-pyd.y)+
    (1*
     (py.a-pyd.a)*(py.a-pyd.a)+
     (py.b-pyd.b)*(py.b-pyd.b)+
     (py.g-pyd.g)*(py.g-pyd.g));
  return err;
}

int main(int argc, char *argv[]){
  FILE *fp,*fp2,*gp,*fpx,*fpy,*fpi;
  Pose px[1000];
  Pose py[1000];
  Pose pyd[1000];
  int skip[1000];
  Pose ta;
  int i,num,count=0,j,k;
  double v,w,x,y,z,a,b,g,t,q,ts,tas;
  double t2,x2,y2,z2,a2,b2,g2;
  gp=popen("/usr/bin/gnuplot","w");

  fpx=fopen(argv[1],"r");
  fpy=fopen(argv[2],"r");
  i=0;
  t2=0;
  j=0;
  while(fscanf(fpx,"%lf %lf %lf %lf %lf %lf %lf",
	       &t,&x,&y,&z,&a,&b,&g)!=EOF){
    if(t>t2){
      while(fscanf(fpy,"%lf %lf %lf %lf %lf %lf %lf",
		   &t2,&x2,&y2,&z2,&a2,&b2,&g2)!=EOF){
	if(t<=t2)break;
      }
    }
    j++;
    if(j<1000)continue;
    if(t +0.1 >= t2){//accept
      px[i].t=t;
      px[i].x=x;
      px[i].y=y;
      px[i].z=z;
      px[i].a=a;
      px[i].b=b;
      px[i].g=g;

      py[i].t=t2;
      py[i].x=x2;
      py[i].y=y2;
      py[i].z=z2;
      py[i].a=a2;
      py[i].b=b2;
      py[i].g=g2;
    
      i++;
      if(i>=800)break;
    }
  }
  num=i;
  /*
    set_initial_pos(px,0,num);
    set_initial_pos(py,0,num);
  */

  for(i=0;i<num;i+=num/4){
    set_initial_pos(px,i,i+num/4);
    set_initial_pos(py,i,i+num/4);
  }


  x=0;
  y=0;
  q=0;

  /*
    t.x=1;
    t.y=1;
    t.z=1;
    t.a=0.6;
    t.b=0.6;
    t.g=0.6;
    ts=0.2;
    for(i=0;i<1000;i++){
    v=0.1;
    w=(double)rand()/RAND_MAX-0.5;
    px[i].x=x;
    px[i].y=y;
    px[i].z=0;
    px[i].a=0;
    px[i].b=0;
    px[i].g=q;

    x+=v*cos(q);
    y+=v*sin(q);
    q+=w;

  }
  calc_sensor_pos(px,py,&t,ts,num)
 

  for(i=0;i<1000;i++){
    py[i].x+=((double)rand()/RAND_MAX-0.5)*0.1;
    py[i].y+=((double)rand()/RAND_MAX-0.5)*0.1;
  }
  */
  ta.x=0;
  ta.y=0;
  ta.z=0;
  ta.a=-1.57;
  ta.b=0;
  ta.g=-1.57;
  tas=1;
  fpi=fopen("initial","r");
  
  if(fpi!=NULL){
    fscanf(fpi,"%lf %lf %lf %lf %lf %lf %lf\n",
   	   &ta.x,&ta.y,&ta.z,&ta.a,&ta.b,&ta.g,&tas); 
    fclose(fpi);
  }
#if 1
  while(1){
    /*    
    double e,ee2[7][7],ee1[7],ee,tas2[7][7],tas1[7],H[7][7],Hinv[7][7],G[7];
    Pose ta2[7][7],ta1[7];

    for(i=0;i<7;i++){
      ta1[i]=ta;
      tas1[i]=tas;
      if(i==0)ta1[i].x+=EPS;
      if(i==1)ta1[i].y+=EPS;
      if(i==2)ta1[i].z+=EPS;
      if(i==3)ta1[i].a+=EPS;
      if(i==4)ta1[i].b+=EPS;
      if(i==5)ta1[i].g+=EPS;
      if(i==6)tas1[i]+=EPS;
      for(j=0;j<7;j++){
	ta2[i][j]=ta;
	tas2[i][j]=tas;
	if(i==0)ta2[i][j].x+=EPS;
	if(i==1)ta2[i][j].y+=EPS;
	if(i==2)ta2[i][j].z+=EPS;
	if(i==3)ta2[i][j].a+=EPS;
	if(i==4)ta2[i][j].b+=EPS;
	if(i==5)ta2[i][j].g+=EPS;
	if(i==6)tas2[i][j]+=EPS;

	if(j==0)ta2[i][j].x+=EPS;
	if(j==1)ta2[i][j].y+=EPS;
	if(j==2)ta2[i][j].z+=EPS;
	if(j==3)ta2[i][j].a+=EPS;
	if(j==4)ta2[i][j].b+=EPS;
	if(j==5)ta2[i][j].g+=EPS;
	if(j==6)tas2[i][j]+=EPS;
	ee2[i][j]=0;
      }
      ee1[i]=0;
    }
    e=0;
    calc_sensor_pos(px,pyd,&ta,tas,num);
    for(i=0;i<num;i++){
      ee =err(py[i],pyd[i]);
      if(ee<3.1*3.1){
	e+=ee;
	skip[i]=0;
      }else{
	skip[i]=1;
      }
    }
    
    for(j=0;j<7;j++){
      calc_sensor_pos(px,pyd,&ta1[j],tas1[j],num);
      for(i=0;i<num;i++){
	if(skip[i]==0)ee1[j]+=err(py[i],pyd[i]); 
      }
    }
    for(k=0;k<7;k++){
      for(j=0;j<7;j++){
	calc_sensor_pos(px,pyd,&ta2[k][j],tas2[k][j],num);
	for(i=0;i<num;i++){
	  if(skip[i]==0)ee2[k][j]+=err(py[i],pyd[i]); 
	}
      }
    }

    //calc Hessian
    for(k=0;k<7;k++){
      G[k]=(ee1[k]-e)/(EPS*num);
      for(j=0;j<7;j++){
	H[k][j]=(ee2[k][j]-ee1[k])/(EPS*EPS*num);
       	if(k==j)H[k][j]+=100.000000001;
      }
    }

    ginverse_matrix7d(H,Hinv);

    ta.x-=( Hinv[0][0]*G[0]+ Hinv[0][1]*G[1]+ Hinv[0][2]*G[2]+
	    Hinv[0][3]*G[3]+ Hinv[0][4]*G[4]+ Hinv[0][5]*G[5]+Hinv[0][6]*G[6]);
    ta.y-=( Hinv[1][0]*G[0]+ Hinv[1][1]*G[1]+ Hinv[1][2]*G[2]+
	    Hinv[1][3]*G[3]+ Hinv[1][4]*G[4]+ Hinv[1][5]*G[5]+Hinv[1][6]*G[6]);
    ta.z-=( Hinv[2][0]*G[0]+ Hinv[2][1]*G[1]+ Hinv[2][2]*G[2]+
	    Hinv[2][3]*G[3]+ Hinv[2][4]*G[4]+ Hinv[2][5]*G[5]+Hinv[2][6]*G[6]);
    ta.a-=( Hinv[3][0]*G[0]+ Hinv[3][1]*G[1]+ Hinv[3][2]*G[2]+
	    Hinv[3][3]*G[3]+ Hinv[3][4]*G[4]+ Hinv[3][5]*G[5]+Hinv[3][6]*G[6]);
    ta.b-=( Hinv[4][0]*G[0]+ Hinv[4][1]*G[1]+ Hinv[4][2]*G[2]+
	    Hinv[4][3]*G[3]+ Hinv[4][4]*G[4]+ Hinv[4][5]*G[5]+Hinv[4][6]*G[6]);
    ta.g-=( Hinv[5][0]*G[0]+ Hinv[5][1]*G[1]+ Hinv[5][2]*G[2]+
	    Hinv[5][3]*G[3]+ Hinv[5][4]*G[4]+ Hinv[5][5]*G[5]+Hinv[5][6]*G[6]);
    tas -=( Hinv[6][0]*G[0]+ Hinv[6][1]*G[1]+ Hinv[6][2]*G[2]+
	    Hinv[6][3]*G[3]+ Hinv[6][4]*G[4]+ Hinv[6][5]*G[5]+Hinv[6][6]*G[6]);

    */
    double e=0,ee,edx=0,edy=0,edz=0,eda=0,edb=0,edg=0,eds=0;
    calc_sensor_pos(px,pyd,&ta,tas,num);
    for(i=0;i<num;i++){
      ee =err(py[i],pyd[i]);
      if(ee<3.1*3.1){
	e+=ee;
	skip[i]=0;
      }else{
	skip[i]=1;
      }
    }

    ta.x+=EPS;
    calc_sensor_pos(px,pyd,&ta,tas,num);
    for(i=0;i<num;i++){
      if(skip[i]==0)edx+=err(py[i],pyd[i]);
      
    }
    ta.x-=EPS;

    ta.y+=EPS;
    calc_sensor_pos(px,pyd,&ta,tas,num);
    for(i=0;i<num;i++){
      if(skip[i]==0)edy+=err(py[i],pyd[i]);
    }
    ta.y-=EPS;

    ta.z+=EPS;
    calc_sensor_pos(px,pyd,&ta,tas,num);
    for(i=0;i<num;i++){
      if(skip[i]==0)edz+=err(py[i],pyd[i]);
    }
    ta.z-=EPS;

    ta.a+=EPS;
    calc_sensor_pos(px,pyd,&ta,tas,num);
    for(i=0;i<num;i++){
      if(skip[i]==0)eda+=err(py[i],pyd[i]);
    }
    ta.a-=EPS;

    ta.b+=EPS;
    calc_sensor_pos(px,pyd,&ta,tas,num);
    for(i=0;i<num;i++){
      if(skip[i]==0)edb+=err(py[i],pyd[i]);
    }
    ta.b-=EPS;

    ta.g+=EPS;
    calc_sensor_pos(px,pyd,&ta,tas,num);
    for(i=0;i<num;i++){
      if(skip[i]==0)edg+=err(py[i],pyd[i]);
    }
    ta.g-=EPS;

    tas+=EPS;
    calc_sensor_pos(px,pyd,&ta,tas,num);
    for(i=0;i<num;i++){
      if(skip[i]==0)eds+=err(py[i],pyd[i]);
    }
    tas-=EPS;
    
    ta.x-=0.0001*(edx-e)/(EPS*num);
    ta.y-=0.0001*(edy-e)/(EPS*num);
    ta.z-=0.0001*(edz-e)/(EPS*num);
    ta.a-=0.0001*(eda-e)/(EPS*num);
    ta.b-=0.0001*(edb-e)/(EPS*num);
    ta.g-=0.0001*(edg-e)/(EPS*num);
    tas -=0.0001*(eds-e)/(EPS*num);
   
    
    count++; 
    if(count %100==0){
      fp=fopen("out","w");
      fp2=fopen("link","w");
      for(i=0;i<num;i++){
	fprintf(fp,"%f %f %f %f %f %f %f %f %f\n",
		px[i].x,px[i].y,px[i].g,
		py[i].x,py[i].y,py[i].g,
		pyd[i].x,pyd[i].y,pyd[i].g);
	if(skip[i]==0)	
	  fprintf(fp2,"%f %f %f\n%f %f %f\n\n",
		  py[i].x,py[i].y,py[i].g,
		  pyd[i].x,pyd[i].y,pyd[i].g);
      }
      fclose(fp);
      fclose(fp2);
      fprintf(gp,"plot 'out' u 4:5 ,'out' u 7:8,'link' u 1:2 w l\n");
      fflush(gp);
      
      printf("%f %f %f %f %f %f %f %f\n",e, ta.x,ta.y,ta.z,ta.a,ta.b,ta.g,tas);

      fpi=fopen("initial","w");
      fprintf(fpi,"%f %f %f %f %f %f %f\n",
	      ta.x,ta.y,ta.z,ta.a,ta.b,ta.g,tas);
      fclose(fpi);

      usleep(100000);
    }
  }
#else
  double min_e=1000000000,min_ts;
  Pose min_t;

  while(1){
    double e=0,ee,edx=0,edy=0,edz=0,eda=0,edb=0,edg=0,eds=0;
   
    ta.x=(rand()/(double)RAND_MAX-0.5)*0;
    ta.y=(rand()/(double)RAND_MAX-0.5)*0;
    ta.z=(rand()/(double)RAND_MAX-0.5)*0;
    ta.a=(rand()/(double)RAND_MAX-0.5)*7;
    ta.b=(rand()/(double)RAND_MAX-0.5)*7;
    ta.g=(rand()/(double)RAND_MAX-0.5)*7;
    tas=0.08+(rand()/(double)RAND_MAX-0.5)*0.02;

    calc_sensor_pos(px,pyd,&ta,tas,num);
    
    for(i=0;i<num;i++){
      ee =err(py[i],pyd[i]);
      if(ee<10*10)e+=ee;
    }

    if(min_e>e){
      min_t=ta;
      min_ts=tas;
      min_e=e;
      
      count++; 
      if(count %100==0){
	fp=fopen("out","w");
	fp2=fopen("link","w");
	for(i=0;i<num;i++){
	  fprintf(fp,"%f %f %f %f %f %f %f %f %f\n",
		  px[i].x,px[i].y,px[i].g,
		  py[i].x,py[i].y,py[i].g,
		  pyd[i].x,pyd[i].y,pyd[i].g);
	  
	  fprintf(fp2,"%f %f %f\n%f %f %f\n\n",
		  py[i].x,py[i].y,py[i].g,
		  pyd[i].x,pyd[i].y,pyd[i].g);
	}
	fclose(fp);
	fclose(fp2);
	fprintf(gp,"plot 'out' u 4:5 ,'out' u 7:8,'link' u 1:2 w l\n");
	fflush(gp);
	
	printf("%f %f %f %f %f %f %f %f\n",e, ta.x,ta.y,ta.z,ta.a,ta.b,ta.g,tas);
	
	fpi=fopen("initial","w");
	fprintf(fpi,"%f %f %f %f %f %f %f\n",
		ta.x,ta.y,ta.z,ta.a,ta.b,ta.g,tas);
	fclose(fpi);

	usleep(100000);
      }
    }
  }
#endif

}
