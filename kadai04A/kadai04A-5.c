#include<stdio.h>
//画像を生成するためのライブラリ
#include<ppexp.h>

//使用するプロセス数-Dオプションで指定
//#define PNUM 2

//各プロセスが計算する行数
#define HEI_PNUM (HEI/PNUM)

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

    //自身のプロセス番号を取得
    int myrank = T_GetMyNum();

    //結果を保存するファイルを指定
    FILE *fp=fopen("kadai04A-5.txt","a");

    //グレー画像情報
    uchar g[HEI_PNUM][WID];

    //結果表示用の画像情報
    uchar result[HEI][WID];

    //for文用の変数(y軸,x軸)
    int y,x,i;

    //受信したプロセス数
    int recv_count=0;

    //実行時間計測用の変数
    double start,end;

    //計算が完了したかどうか
    int flag=0;

    //10回実行時間を保存する配列
    double time[10];
    //計測回数
    int cnt=0;
    //計算した行数
    int cnt_row;
    //計算する行番号
    int row;

    //10回計算を繰り返す
    while(cnt<10){
        //計算した行数を初期化
        cnt_row=0;
        //計算する行番号を初期化
        row = myrank;
        //他のプロセスと同期
        T_Barrier();

        //プロセス0のみ計測開始
        if(myrank==0){
            start = T_GetTime();
        }
        //初期化
        flag=0;


        //計算が完了するまで繰り返す
        while(flag==0){
            //他のプロセスと同期
            T_Barrier();

            //Mandelbrot集合の描画
            for(x=0;x<WID;x++){
                //実部
                double a = MIN_A + RANGE_A*(double)x/(double)WID;
                //虚部
                double b = MIN_B + RANGE_B*(double)row/(double)HEI;
                //zの実部
                double zr = 0.0;
                //zの虚部
                double zi = 0.0;
                //zの絶対値の二乗
                double z = 0.0;
                //繰り返し回数
                int n = 0;

                //zの絶対値の２乗が4より小さいかつ繰り返し回数が最大繰り返し回数より小さい間繰り返す
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
                //グレー画像情報の設定
                g[cnt_row][x] = (U-n)*255/U;
            }

            //計算した行数を更新
            cnt_row++;
            //次に計算する行を更新
            row+=PNUM;
            //次に計算する行が最大の行を超えた場合
            if(row>=HEI){
                //計算が完了したことを示すフラグを立てる
                flag=1;
            }
        }

        //他のプロセスと同期
        T_Barrier();

        //プロセス0のみ計測終了
        if(myrank==0){
            end = T_GetTime();
            //計測時間を保存
            time[cnt]=end-start;
        }

        //計測回数を更新
        cnt++;
    }

    //計測時間の平均を計算
    double sum=0;
    for(i=0;i<10;i++){
        sum+=time[i];
    }
    double ave = sum/10;

    //計測時間の表示
    if(myrank==0){
        printf("%d %f\n",PNUM,ave);
        fprintf(fp,"%d %f\n",PNUM,ave);
    } 

    //プロセス0のみ実行
    if(myrank==0){
        //行番号を初期化
        i=myrank;
        cnt_row=0;
        //行番号が最大値未満の間繰り返す
        while(i<HEI){

            //グレー画像を結果表示用の画像情報にコピー
            for(x=0;x<WID;x++){
                result[i][x]=g[cnt_row][x];
            }

            //行番号を更新
            i+=PNUM;
            cnt_row++;
        }

        //他のプロセスからの結果を受信
        for(i=1;i<PNUM;i++){
            //受信したデータを格納する配列
            uchar recv_d[HEI_PNUM][WID];
            //プロセスiからデータを受信
            T_Recv(i,&recv_d,HEI_PNUM*WID*sizeof(uchar));
            //結果表示用の画像情報にグレー画像情報をコピー
            //行番号を初期化
            row=i;
            cnt_row=0;
            //行番号が最大値未満の間繰り返す
            while(row<HEI){
                //グレー画像を結果表示用の画像情報にコピー
                for(x=0;x<WID;x++){
                    result[row][x]=recv_d[cnt_row][x];
                }
                //行番号を更新
                row+=PNUM;
                cnt_row++;
            }
        }

        //グレー画像の表示(ファイル名=grey.xmp)
        generate_pgm(&result[0][0],WID,HEI,"grey-5.xmp");
    }else{
        //プロセス0に配列を送信
        T_Send(0,&g[0][0],HEI_PNUM*WID*sizeof(uchar));
    }

    return;
}

int main(void){
    //T_Runで並列呼び出し
    T_Run(PNUM,f); 
    return 0;
}