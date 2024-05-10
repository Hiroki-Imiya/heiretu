#include<stdio.h>
//画像を生成するためのライブラリ
#include<ppexp.h>

//画像の横幅
#define WID 320
//画像の縦幅
#define HEI 240

//並列用の関数
void f(void){
    //グレー画像情報
    uchar g[HEI][WID];
    //カラー画像の赤成分
    uchar R[HEI][WID];
    //カラー画像の緑成分
    uchar G[HEI][WID];
    //カラー画像の青成分
    uchar B[HEI][WID];

    //for文用の変数(y軸,x軸)
    int y,x;

    for(y=0;y<HEI;y++){
        //左1/3の部分
        for(x=0;x<WID/3;x++){
            //グレー画像の生成
            g[y][x] = 0;

            //以下の組み合わせで赤色になる
            //カラー画像の赤成分の生成
            R[y][x] = 255;
            //カラー画像の緑成分の生成
            G[y][x] = 0;
            //カラー画像の青成分の生成
            B[y][x] = 0;
        }

        //中央1/3の部分
        for(x=WID/3;x<2*WID/3;x++){
            //グレー画像の生成
            g[y][x] = 0;

            //以下の組み合わせで白色になる
            //カラー画像の赤成分の生成
            R[y][x] = 255;
            //カラー画像の緑成分の生成
            G[y][x] = 255;
            //カラー画像の青成分の生成
            B[y][x] = 255;
        }

        //右1/3の部分
        for(x=2*WID/3;x<WID;x++){
            //グレー画像の生成(128=灰色)
            g[y][x] = 128;
            //以下の組み合わせで青色になる
            //カラー画像の赤成分の生成
            R[y][x] = 0;
            //カラー画像の緑成分の生成
            G[y][x] = 0;
            //カラー画像の青成分の生成
            B[y][x] = 255;
        }
    }

    //グレー画像の表示(ファイル名=grey.xmp)
    generate_pgm(&g[0][0],WID,HEI,"grey.xmp");
    //カラー画像の表示(ファイル名=color.xmp)
    generate_ppm(&R[0][0],&G[0][0],&B[0][0],WID,HEI,"color.xmp");

    return;
}

int main(void){
    //T_Runで並列呼び出し
    T_Run(1,f);
    
    return 0;
}