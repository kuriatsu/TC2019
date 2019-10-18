#include<stdio.h>

int main(int argc ,char* argv[]){
  FILE *src,*dst;
  double x,y,z;

  src = fopen(argv[1],"r");
  dst = fopen(argv[2],"w");

  while(fscanf(src,"%lf %lf %lf",&x,&y,&z)!=EOF){
    if(z > -400 && z < 1900)
      fprintf(dst,"%f %f %f\n",x,y,z);
  }
  fclose(src);
  fclose(dst);
}
