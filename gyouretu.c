#include <stdio.h>
#include <math.h>
#define N 3
#define EPSILON 1.0E-5
#define TRUE 1
#define FALSE 0
float a[N][N]={
  {1,3,-1},
  {1,3,1},
  {3,-1,-2}
};
float x[N],b[N] = {1,0,1};
int sw;
int count=0;
void sweep();
void swap(float *wk1,float *wk2);
int main(void){
  int i,j;
  printf("==== 係数行列と定数 ===\n\n");
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      printf("%10.4f",a[i][j]);
    }
    printf("%10.4f\n\n",b[i]);
  }
  sw=TRUE;
  sweep();
  if(sw==TRUE){
    printf("\n\n 一次方程式の解 \n\n");
    for(i=0;i<N;i++){
      for(j=0;j<N;j++){
        printf("%10.4f",a[i][j]);
      }
      printf("%10.4f\n\n",b[i]);
    }
  }else{
    printf("解は求められませんでした。\n\n");
  }
  printf("掛け算と割り算の計算回数は%d回\n",count );
}

void swap(float *wk1,float *wk2){
  float w;
  w= *wk1;
  *wk1= *wk2;
  *wk2=w;
}
void sweep(void){
  int i,j,k,ik;
  float ak,aik;
  for(k=0; k<N; k++){
    ak=a[k][k];
    if(fabs(ak) <= EPSILON){
      ik=k+1;
      while((ik<N)&&(fabs(a[ik][k])<EPSILON)) ik++;
      if(ik<N){
	for(j=k; j<N; j++){
	  swap(&a[k][j],&a[ik][j]);
	}
        swap(&b[k],&b[ik]);
        ak=a[k][k];
      }else{
	printf("ピポットがゼロです！");
	sw=FALSE;
	goto owari;
      }
    }
    for(j=k; j<N; j++){
      a[k][j]=a[k][j]/ak;
      count++;
    }
    b[k]=b[k]/ak;
    count++;
    for(i=0;i<N;i++){
      if(i!=k){
	aik=a[i][k];
	for(j=k; j<N; j++){
	  a[i][j]=a[i][j]-aik*a[k][j];
	  count++;
	}
	b[i]=b[i]-b[k]*aik;
	count++;
      }
    }
  }
  for(k=0; k<N; k++){
    x[k]=b[k];
  }
 owari:;
}
