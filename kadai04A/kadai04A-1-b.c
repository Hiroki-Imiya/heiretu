#include<stdio.h>
//画像を生成するためのライブラリ
#include<ppexp.h>


//画像の横幅
#define WID 1024
//画像の縦幅
#define HEI 1024
//繰り返しの最大回数
#define U 8000

//実部の最小値
#define MIN_A 1.36769
//実部の最大値
#define MAX_A 1.36789
//実部の振れ幅
#define RANGE_A (MAX_A-MIN_A)
//虚部の最小値
#define MIN_B 0.00736
//虚部の最大値
#define MAX_B 0.00756
//虚部の振れ幅
#define RANGE_B (MAX_B-MIN_B)

//並列用の関数
void f(void){
    //グレー画像情報
    uchar g[HEI][WID];
    //カラー画像の赤成分
    //uchar R[HEI][WID];
    //カラー画像の緑成分
    //uchar G[HEI][WID];
    //カラー画像の青成分
    //uchar B[HEI][WID];

    //for文用の変数(y軸,x軸)
    int y,x;

    //Mandelbrot集合の描画
    for(y=0;y<HEI;y++){
        for(x=0;x<WID;x++){
            //実部
            double a = MIN_A + RANGE_A*(double)x/(double)WID;
            //虚部
            double b = MIN_B + RANGE_B*(double)y/(double)HEI;
            //zの実部
            double zr = 0.0;
            //zの虚部
            double zi = 0.0;
            //zの絶対値の二乗
            double z = 0.0;
            //繰り返し回数
            int n = 0;
            
            //zの絶対値の２乗が4より小さいかつ繰り返し回数が最大繰り返し回数より小さい間繰り返す
            while(z<4.0 && n<U){
                //zの更新
                double zr_next = zr*zr - zi*zi - a;
                double zi_next = 2.0*zr*zi - b;
                zr = zr_next;
                zi = zi_next;
                z = zr*zr + zi*zi;
                n++;
            }

            //グレー画像の生成
            g[y][x]=(U-n)*255/U;
        }
    }

    //グレー画像の表示(ファイル名=grey.xmp)
    generate_pgm(&g[0][0],WID,HEI,"grey-b.xmp");
    //カラー画像の表示(ファイル名=color.xmp)
    //generate_ppm(&R[0][0],&G[0][0],&B[0][0],WID,HEI,"color.xmp");

    return;
}

int main(void){
    //T_Runで並列呼び出し
    T_Run(1,f);
    
    return 0;
}