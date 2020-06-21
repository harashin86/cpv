#include<stdio.h>
#include<stdlib.h>
#include<math.h>
unsigned char header[54];
unsigned char imgin[3][512][512];
unsigned char imgout[3][512][512];
int width;
int height;
int bite;
void get_data(void);
void rgb_to_ybr(void);
void processing(void);
void ybr_to_rgb(void);
void put_data(void);
void cp(void);
void gausian_filter(void);

int main(void){
  
  get_data();
  rgb_to_ybr();
  cp();
  processing();
  ybr_to_rgb();
  put_data();
  return 0;

}
void processing(){
  
}

void get_data(){
  FILE *fp;
  char file[114];
  int i,j;
  printf("入力ファイル名を入力して下さい:");
  scanf("%s",file);
  fp=fopen(file,"rb");
  if(fp==NULL){
    printf("ファイルをオープンできませんでした\n");
    exit(1);
  }
  printf("ファイルをオープンしました.\n");
  for(i=0;i<54;i++){
    header[i]=fgetc(fp);
  }
  int filesize= header[2]+header[3]*pow(16,2)+header[4]*pow(16,4)+header[5]*pow(16,6);
  int offset= header[10]+header[11]*pow(16,2)+header[12]*pow(16,4)+header[13]*pow(16,6);
  width= header[18]+header[19]*pow(16,2)+header[20]*pow(16,4)+header[21]*pow(16,6);
  height= header[22]+header[23]*pow(16,2)+header[24]*pow(16,4)+header[25]*pow(16,6);
  int bits=header[28]+header[29]*pow(16,2);
  bite=filesize-offset-width*height*(bits/8);
  for(i=height-1;i>-1;i--){
    for(j=0;width>j;j++){
      imgin[2][j][i]=fgetc(fp);
      imgin[1][j][i]=fgetc(fp);
      imgin[0][j][i]=fgetc(fp);
    }
  }
  printf("＜ファイルサイズ＞\n\n%d バイト\n",filesize); 
  printf("＜オフセット＞\n%d バイト\n",offset);
  printf("＜画像の幅＞\n%d 画素\n",width);
  printf("＜画像の高さ＞\n%d ライン\n",height);
  printf("＜１画素当たりのビット数＞\n%d ビット\n",bits);
  printf("＜挿入ビット数＞\n%d バイト\n",bite);
  fclose(fp);
  printf("ファイルをクローズしました\n");
}
void rgb_to_ybr(void){
  int i=0,j=0,k=0;
  double arry[3]={0,0,0};
  double offset=0; 
  for(i=0;i<height;i++){
    for(j=0;j<width;j++){
      arry[0]=0.2990*imgin[0][j][i]+0.5870*imgin[1][j][i]+0.1140*imgin[2][j][i];//色空間変換
      arry[1]=(-0.1687*imgin[0][j][i])+(-0.3313*imgin[1][j][i])+0.5000*imgin[2][j][i];
      arry[2]=0.5000*imgin[0][j][i]+(-0.4187*imgin[1][j][i])+(-0.0813*imgin[2][j][i]);
      for(k=0;k<3;k++){
	//四捨五入
	if (arry[k]>=0.0){
	  offset=0.5;
	}else{
	  offset=-0.5;
	}
	arry[k]=(int)(arry[k]+offset);      
	//１２８加算
	if(k==1||k==2){
	  arry[k]+=128;
	}  
	//範囲処理    
	if(arry[k]>255){
	  arry[k]=255;
	}else if(arry[k]<0){
	  arry[k]=0;
	}
	imgin[k][j][i]=arry[k];
      }
    } 
  }
}
void cp(void){
  for(int i=0;i<height;i++){
    for(int j=0;j<width;j++){
      imgout[0][j][i]=imgin[0][j][i];
      imgout[1][j][i]=imgin[1][j][i];
      imgout[2][j][i]=imgin[2][j][i];
    }
  }
}

void ybr_to_rgb(void){
  int i=0,j=0,k=0;
  double arry[3]={0,0,0};
  double offset=0;
  for(i=0;i<height;i++){
    for(j=0;j<width;j++){//色空間変換
      arry[0]= (1.0000*imgout[0][j][i]) + 1.4020*(imgout[2][j][i]-128);
      arry[1]= (1.0000*imgout[0][j][i]) + (-0.3441*(imgout[1][j][i]-128)) + (-0.7141*(imgout[2][j][i]-128));
      arry[2]= (1.0000*imgout[0][j][i])+(1.7720*(imgout[1][j][i]-128));
      for(k=0;k<3;k++){      
	if (arry[k]>=0.0){//四捨五入
	  offset=0.5;
	}else{
	  offset=-0.5;
	}
	arry[k]=(int)(arry[k]+offset);//バイト範囲処理
	if(arry[k]>255){
	  arry[k]=255;
	}else if(arry[k]<0){
	  arry[k]=0;
	}
	imgout[k][j][i]=arry[k];
      }
    }
  }
}
void put_data(void){
  int i=0,j=0;
  char newfile[113];
  printf("出力ファイル名を入力して下さい.");
  scanf("%s",newfile);
  FILE *fp;
  fp=fopen(newfile,"wb");
  printf("ファイルをオープンしました\n");
  for(i=0;i<54;i++){  
    fputc(header[i],fp);
  }
  for(i=height-1;i>-1;i--){ 
    for(j=0;j<width;j++){
      fputc(imgout[2][j][i],fp);
      fputc(imgout[1][j][i],fp);
      fputc(imgout[0][j][i],fp);
    }
  }
  fclose(fp);
  printf("ファイルをクローズしました.\n");
}
void gausian_filter(void){
  double filter_gausian[25][25]={{}};

  for(int i=-12;i<13;i++){//ガウシアンフィルタ生成
    for(int j=12;j>-13;j--){
      double x=(pow(j,2)+pow(i,2))/(-2*pow(2.5,2));  
      filter_gausian[12-j][12+i]=(1/(2*3.14*2.5*2.5))*exp(x);
    }
  }
  for(int i=0;i<height;i++){//フィルタ処理
    for(int j=0;j<width;j++){
      double sum=0;      
      if(12<i<height-12||12<j<width-12){//範囲内処理
	for(int k=0;k<25;k++){
	  for(int l=0;l<25;l++){
	    sum+=filter_gausian[l][k]*imgin[0][j-12+l][i-12+k];
	  }
	}
      }else{//範囲外
	for(int k=0;k<25;k++){
	  for(int l=0;l<25;l++){ 
	    int a=0,b=0;
	    if(k<12-i){//a判定
	      a=12-i+k+1;
	    }else if(k-12>height-i){
	      a=i+12-k+1;
	    }else{
	      a=i-12+k;
	    }
	    if(l<12-j){//b判定
	      b=12-i+k-1;
	    }else if(l-12>width-j){
	      b=j+12-l+1;
	    }else{
	      b=j-12+l;
	    }
	    sum+=filter_gausian[l][k]*imgin[0][b][a];
	  }
	}
      }
      if(sum>255){//y値範囲外処理
	sum=255;
      }else if(sum<0){
	sum=0;
      }
      imgout[0][j][i]=sum;//imgoutに格納
    }
  }
}
