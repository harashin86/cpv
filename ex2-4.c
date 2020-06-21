#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define NUM_IMG 2
#define BLOCK_SIZE 16
unsigned char header[NUM_IMG][54];
unsigned char imgin[NUM_IMG][3][512][512];
unsigned char imgout[NUM_IMG][3][512][512];
int width[NUM_IMG];
int height[NUM_IMG];
int bite[NUM_IMG];
void get_data(int num);
void rgb_to_ybr(int num);
void processing(int num);
void ybr_to_rgb(int num);
void put_data(int num);
void cp(int num);
void red_frame(int num);
void matching(void) {
  /* code */
}
int main(void){
  for(int i=0;i<NUM_IMG;i++){//複数の画像の読み込み
    get_data(i);
    rgb_to_ybr(i);
    cp(i);
  }
  for (int i = 0; i < NUM_IMG; i++) {
    if (i==0) {
      /*1度だけ実行*/
      processing(i);//
    }
    ybr_to_rgb(i);
  }
  for(int i=0;i<NUM_IMG;i++){//複数の画像の書き出し
    put_data(i);
  }
  return 0;
}
void processing(int num){
  int h=0,v=0;
  printf("パラメータ入力\n" );
  printf("対象ブロックの左上端画素の位置を入力して下さい\n");
  printf("---水平：");
  scanf("%d",&h );
  printf("---垂直：");
  scanf("%d",&v );
  int target[h][v]
  /*類似画素保存*/
  for (int i = 0; i < BLOCK_SIZE; i++) {
    for (int j = 0; j < BLOCK_SIZE; j++) {
      target[j][i]=imgout[num][0][h+j][v+i];
    }
  }
  mating(void);
  int min=0;
  int sum=0;
  int min_p{0,0}
  for (int i = 0; i < height-BLOCK_SIZE-1; i++) {
    for (int j = 0; j < width-BLOCK_SIZE-1; j++) {
      /* 差分絶対値和*/
      for (int k = 0; k < BLOCK_SIZE; k++) {
        for (int l = 0; l < BLOCK_SIZE; l++) {
        sum+=abs(target[l][k]-imgin[num][0][j+l][i+k])
        //sum+=abs(imgin[num][0][h][v]-imgin[num][0][j+l][i+k])
        }
      }
      if (sum<min) {
        /* 類似度更新 */
        min=sum;
        /*参照ブロックの基準点*/
        min_p[0]=j
        min_p[1]=i
      }
    }
  }
  red_frame(num,h,v)
}
int matching(void) {
  /* 類似画像検索*/
  int min=0;
  int sum=0;
  int min_p{0,0}
  for (int i = 0; i < height-BLOCK_SIZE-1; i++) {
    for (int j = 0; j < width-BLOCK_SIZE-1; j++) {
      /* 差分絶対値和*/
      for (int k = 0; k < BLOCK_SIZE; k++) {
        for (int l = 0; l < BLOCK_SIZE; l++) {
        sum+=abs(target[l][k]-imgin[num][0][j+l][i+k])
        //sum+=abs(imgin[num][0][h][v]-imgin[num][0][j+l][i+k])
        }
      }
      if (sum<min) {
        /* 類似度更新 */
        min=sum;
        /*参照ブロックの基準点*/
        min_p[0]=j
        min_p[1]=i
      }
    }
  }
}
void red_frame(int num,int hori,int verti) {
  /*赤枠生成*/
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      if(i==0||i==15||j==0||j==15) {
        /*ライン引き*/
        imgout[num][0][hori+j][verti+i]=82;
        imgout[num][1][hori+j][verti+i]=90;
        imgout[num][2][hori+j][verti+i]=240;
      }
    }
  }
}
void get_data(int num){
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
    header[num][i]=fgetc(fp);
  }
  /* int filesize= header[num][2]+header[num][3]*pow(16,2)+header[num][4]*pow(16,4)+header[num][5]*pow(16,6);
  int offset= header[num][10]+header[num][11]*pow(16,2)+header[num][12]*pow(16,4)+header[num][13]*pow(16,6);
   int bits[num]=header[num][28]+header[num][29]*pow(16,2);
  bite[num]=filesize-offset-width*height*(bits/8);*/
  width[num]= header[num][18]+header[num][19]*pow(16,2)+header[num][20]*pow(16,4)+header[num][21]*pow(16,6);
  height[num]= header[num][22]+header[num][23]*pow(16,2)+header[num][24]*pow(16,4)+header[num][25]*pow(16,6);

for(i=height[num]-1;i>-1;i--){
    for(j=0;width[num]>j;j++){
      imgin[num][2][j][i]=fgetc(fp);
      imgin[num][1][j][i]=fgetc(fp);
      imgin[num][0][j][i]=fgetc(fp);
    }
  }
  /*printf("＜ファイルサイズ＞\n\n%d バイト\n",filesize[num]);
  printf("＜オフセット＞\n%d バイト\n",offset[num]);
  printf("＜画像の幅＞\n%d 画素\n",width[num]);
  printf("＜画像の高さ＞\n%d ライン\n",height[num]);
  printf("＜１画素当たりのビット数＞\n%d ビット\n",bits[num]);
  printf("＜挿入ビット数＞\n%d バイト\n",bite[num]);*/
  printf("---width: %d\n",width[num]);
  printf("---height: %d\n",height[num]);
  fclose(fp);
  printf("ファイルをクローズしました\n");
}
void rgb_to_ybr(int num){
  int i=0,j=0,k=0;
  double arry[3]={0,0,0};
  double offset=0;
  for(i=0;i<height[num];i++){
    for(j=0;j<width[num];j++){
      arry[0]=0.2990*imgin[num][0][j][i]+0.5870*imgin[num][1][j][i]+0.1140*imgin[num][2][j][i];//色空間変換
      arry[1]=(-0.1687*imgin[num][0][j][i])+(-0.3313*imgin[num][1][j][i])+0.5000*imgin[num][2][j][i];
      arry[2]=0.5000*imgin[num][0][j][i]+(-0.4187*imgin[num][1][j][i])+(-0.0813*imgin[num][2][j][i]);
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
	imgin[num][k][j][i]=arry[k];
      }
    }
  }
}
void cp(int num){
  for(int i=0;i<height[num];i++){
    for(int j=0;j<width[num];j++){
      imgout[num][0][j][i]=imgin[num][0][j][i];
      imgout[num][1][j][i]=imgin[num][1][j][i];
      imgout[num][2][j][i]=imgin[num][2][j][i];
    }
  }
}
void ybr_to_rgb(int num){
  int i=0,j=0,k=0;
  double arry[3]={0,0,0};
  double offset=0;
  for(i=0;i<height[num];i++){
    for(j=0;j<width[num];j++){//色空間変換
      arry[0]= (1.0000*imgout[num][0][j][i]) + 1.4020*(imgout[num][2][j][i]-128);
      arry[1]= (1.0000*imgout[num][0][j][i]) + (-0.3441*(imgout[num][1][j][i]-128)) + (-0.7141*(imgout[num][2][j][i]-128));
      arry[2]= (1.0000*imgout[num][0][j][i])+(1.7720*(imgout[num][1][j][i]-128));
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
	imgout[num][k][j][i]=arry[k];
      }
    }
  }
}
void put_data(int num){
  int i=0,j=0;
  char newfile[113];
  printf("出力ファイル名を入力して下さい.");
  scanf("%s",newfile);
  FILE *fp;
  fp=fopen(newfile,"wb");
  if(fp==NULL)
    {
      printf("ファイルをオープンできません\n");
    }
  printf("ファイルをオープンしました\n");
  for(i=0;i<54;i++){
    fputc(header[num][i],fp);
  }
  for(i=height[num]-1;i>-1;i--){
    for(j=0;j<width[num];j++){
      fputc(imgout[num][2][j][i],fp);
      fputc(imgout[num][1][j][i],fp);
      fputc(imgout[num][0][j][i],fp);
    }
  }
  fclose(fp);
  printf("ファイルをクローズしました.\n");
}
/*void gausian_filter(void){
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
      if(12<i<height[nun]-12||12<j<width[num]-12){//範囲内処理
	for(int k=0;k<25;k++){
	  for(int l=0;l<25;l++){
	    sum+=filter_gausian[l][k]*imgin[num][0][j-12+l][i-12+k];
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
*/
