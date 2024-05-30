#include<stdio.h>
//画像を生成するためのライブラリ
#include<ppexp.h>

//画像の横幅
#define WID 1024
//画像の縦幅
#define HEI 1024
//nの最大値
#define U 8000

//実部の最小値
#define MIN_A -5.0
//実部の最大値
#define MAX_A 5.0
//実部の振れ幅
#define RANGE_A (MAX_A-MIN_A)
//虚部の最小値
#define MIN_B -8.0
//虚部の最大値
#define MAX_B 8.0
//虚部の振れ幅
#define RANGE_B (MAX_B-MIN_B)

//並列用の関数
void f(void){

    //グレー画像情報
    uchar g[HEI][WID];

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
            
            //zの絶対値の２乗が4より小さいかつnが最大値より小さい間繰り返す
            while(z<4.0 && n<U){

                //次のzの実部を計算
                double zr_next = zr*zr - zi*zi - a;
                //次のzの虚部を計算
                double zi_next = 2.0*zr*zi - b;

                //zの実部を更新
                zr = zr_next;
                //zの虚部を更新
                zi = zi_next;
                //zの絶対値の二乗を更新
                z = zr*zr + zi*zi;

                //繰り返し回数を更新
                n++;
            }

            //グレー画像の生成
            g[y][x]=(U-n)*255/U;
        }
    }

    //グレー画像の表示(ファイル名=grey.xmp)
    generate_pgm(&g[0][0],WID,HEI,"grey-2.xmp");

    return;
}

int main(void){
    //T_Runで並列呼び出し
    T_Run(1,f); 
    return 0;
}