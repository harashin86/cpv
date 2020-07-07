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
void red_frame(int num,int hori,int verti );
int main(void){
  for(int i=0;i<NUM_IMG;i++){//複数の画像の読み込み
    get_data(i);
    rgb_to_ybr(i);
    cp(i);
  }
  for(int i=0;i<NUM_IMG;i++){//マッチング処理
    processing(i);
  }
  for(int i=0;i<NUM_IMG;i++){//複数の画像の書き出し
    ybr_to_rgb(i);
  //  put_data(i);
  }
  return 0;
}
void processing(int num){
  int h=0,v=0;
  int array[2]={0,0};//左上端画素
  int target[BLOCK_SIZE][BLOCK_SIZE];
  int abssum[ height[num] ][ width[num] ];
  for(int i=0; i<height[num]-15; i++){
    for(int j=0; j<width[num]-15; j++){
      abssum[j][i]=0; //初期化
    }
  }
  if(num==0){/*一回目の処理（対象ブロック指定）*/
    printf("パラメータ入力\n" );
    printf("対象ブロックの左上端画素の位置を入力して下さい\n");
    printf("---水平：");
    scanf("%d",&h );
    printf("---垂直：");
    scanf("%d",&v );
    for (int i = 0; i < BLOCK_SIZE; i++) {
      for (int j = 0; j < BLOCK_SIZE; j++) {
        target[j][i]=imgout[num][0][h+j][v+i];
      }
    }
    array[0]=h;
    array[1]=v;
  }else{/* 2回目以降の処理（類似画像検索）*/
    int min=0;
    for(int i=0;i<height[num]-15;i++){
      for(int j=0; j<width[num]-15;j++){
	int sum=0;
	for (int k = 0; k < BLOCK_SIZE; k++) {
	  for (int l = 0; l < BLOCK_SIZE; l++) {
	    /* 差分絶対値和の計算*/
	    sum+=abs(target[l][k]-imgin[num][0][j+l][i+k]);
	  }
	}
	abssum[j][i]=sum;//sumの値の格納
	if (i==0&&j==0) {/* 類似ブロックの更新 */
	  /* 最初のsumの値をminに格納する */
	  min=sum;
	}
	if (sum<min) {
	  min=sum;
	  array[0]=j;
	  array[1]=i;
	}
      }
    }
    printf("＜マッチング結果＞\n");
    printf("マッチング位置:( %d , %d)\n",array[0],array[1] );
    printf("マッチング誤差:%d\n",min );
  }
  FILE *fp;
  printf("データの書き出しを行います");
  fp=fopen("ex2-7.txt","wb");
  if (fp==NULL){
    printf("ファイルをオープンできません.\n");
    exit (1);
  }
  printf("ファイルをオープンしました.\n");
  /* --- ファイルに書き出し ---*/
   for(int i=0;i<height[num]/16;i++){
     for(int j=0;j<width[num]/16;j++){
       fprintf(fp,"%d ",abssum[j][i]);
     }
     fprintf(fp,"\n");
   }
   printf("ファイルにデータを書き出しました.\n");
   /*--- ファイル・クローズ(書き出し用) ---*/
   fclose(fp);
  printf("ファイルをクローズしました.\n");
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
