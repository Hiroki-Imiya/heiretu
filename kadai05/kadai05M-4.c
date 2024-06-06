#include<stdio.h>
//MPIを使用するためのヘッダファイル
#include<mpi.h>
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


int main(int argc,char *argv[]){

    //MPIの初期化
    MPI_Init(&argc,&argv);

    //自身のプロセス番号を取得
    int myrank;
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

    //プロセス数を取得
    int PNUM;
    MPI_Comm_size(MPI_COMM_WORLD,&PNUM);

    //結果を保存するファイルを指定
    FILE *fp=fopen("kadai05M-4.txt","a");

    //結果表示用の画像情報
    uchar result[HEI][WID];

    //for文用の変数(y軸,x軸)
    int y,x,i;

    //実行時間計測用の変数
    double start,end;

    //10回実行時間を保存する配列
    double time[10];
    //計測回数
    int cnt=0;

    //次に計算する行
    int nextHEI_PNUM;

    //すでに計算した行数
    int coled_HEI;

    //Recvのステータスを取得するための変数
    MPI_Status status;

    //10回計測する
    //while(cnt<10){
        //次に計算する行を初期化
        nextHEI_PNUM=0;
        //計算した行数を初期化
        coled_HEI=0;
        //プロセス番号が0の場合
        if(myrank==0){

            for(i=1;i<PNUM;i++){
                //他のプロセスに計算する行を送信
                MPI_Send(&nextHEI_PNUM,1,MPI_INT,i,0,MPI_COMM_WORLD);
                //次に計算する行を更新
                nextHEI_PNUM++;
            }

            //次に計算する行数がある間以下を繰り返す
            while(coled_HEI<HEI){
                //MPI_Recvで受信するプロセス番号
                int source;
                //MPI_Recvで受信する行数
                int HEI_PNUM;
                //MPI_Recvで受信
                MPI_Recv(&HEI_PNUM,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
                //受信したプロセス番号を取得
                source=status.MPI_SOURCE;
                //MPI_Recvで結果を受信
                MPI_Recv(&result[HEI_PNUM][0],WID,MPI_UNSIGNED_CHAR,source,0,MPI_COMM_WORLD,&status);

                //他のプロセスに計算する行を送信
                MPI_Send(&nextHEI_PNUM,1,MPI_INT,source,0,MPI_COMM_WORLD);
                //次に計算する行を更新
                nextHEI_PNUM++;
                //計算した行数を更新
                coled_HEI++;
            }

            //計算終了を通知
            for(i=1;i<PNUM;i++){
                MPI_Send(&nextHEI_PNUM,1,MPI_INT,i,0,MPI_COMM_WORLD);
            }

            //グレー画像の表示(ファイル名=grey.xmp)
            generate_pgm(&result[0][0],WID,HEI,"grey.xmp");
        }else{
            //他のプロセスの場合

            //計算する行
            int col;

            //計算した結果を保存する配列
            uchar tmp[WID];

            //計算する行を受信
            MPI_Recv(&col,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);

            //受信した行が計算できるまで以下を繰り返す
            while(col<HEI){
                ///Mandelbrot集合の描画
                for(x=0;x<WID;x++){
                    //実部
                    double a=MIN_A+RANGE_A/WID*x;
                    //虚部
                    double b=MIN_B+RANGE_B/HEI*col;
                    //zの実部
                    double zr = 0.0;
                    //zの虚部
                    double zi = 0.0;
                    //zの絶対値の二乗
                    double z = 0.0;
                    //繰り返し回数
                    int n = 0;

                    //収束判定
                    while(z<=4.0 && n<U){
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
                    tmp[x]=(U-n)*255/U;
                }

                //結果を送信
                MPI_Send(&col,1,MPI_INT,0,0,MPI_COMM_WORLD);

                MPI_Send(&tmp[0],WID,MPI_UNSIGNED_CHAR,0,0,MPI_COMM_WORLD);

                //次に計算する行を受信
                MPI_Recv(&col,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
            }
        }
    //}

    //MPIの終了
    MPI_Finalize();

    return 0;
}