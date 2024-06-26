#include<stdio.h>
//画像を生成するためのライブラリ
#include<ppexp.h>

//使用するプロセス数-Dオプションで指定
#define PNUM 16

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
    FILE *fp=fopen("kadai04A-7.txt","a");

    //他のプロセスが計算する行数
    int HEI_PNUM;

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

    //10回計測する
    while(cnt<10){
        //プロセス番号が0の場合
        if(myrank==0){
            //他のプロセスが計算する行数を計算
            if(HEI%(PNUM-1)==0){

                HEI_PNUM=HEI/(PNUM-1);
                //他のプロセスに計算する行数を送信
                for(i=1;i<PNUM;i++){
                    T_Send(i,&HEI_PNUM,sizeof(int));
                }

            }else{
                //余りを格納
                int remainder=HEI%(PNUM-1);
                HEI_PNUM=HEI/(PNUM-1);
                //他のプロセスに計算する行数を送信
                for(i=1;i<PNUM;i++){
                    //最後のプロセスには余りを加えた値を送信
                    if(i==PNUM-1){
                        int send_num=HEI_PNUM+remainder;
                        T_Send(i,&send_num,sizeof(int));
                    }else{
                        T_Send(i,&HEI_PNUM,sizeof(int));
                    }
                }
            }

            //他のプロセスと同期
            T_Barrier();

            //計測開始
            start = T_GetTime();

            //他のプロセスと同期
            T_Barrier();

            //計測終了
            end = T_GetTime();

            //計測時間を保存
            time[cnt]=end-start;

            //計測回数を更新
            cnt++;
            
        }else{
            //プロセス番号が0以外の場合、他のプロセスから計算する行数を受信
            T_Recv(0,&HEI_PNUM,sizeof(int));

            //グレー画像情報
            uchar g[HEI_PNUM][WID];

            //他のプロセスと同期
            T_Barrier();

            //Mandelbrot集合の描画
            for(y=0;y<HEI_PNUM;y++){
                for(x=0;x<WID;x++){
                    //実部
                    double a = MIN_A + RANGE_A*(double)x/(double)WID;
                    //虚部
                    double b = MIN_B + RANGE_B*(double)(y+(myrank-1)*HEI_PNUM)/(double)HEI;
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

                    //グレー画像の生成
                    g[y][x]=(U-n)*255/U;
                }
            }

            //他のプロセスと同期
            T_Barrier();

            //計測回数が10回目出ない場合，計測回数を更新しcontinue
            if(cnt<9){
                cnt++;
                continue;
            }

            cnt++;

            //プロセス0に計算する行数を送信
            T_Send(0,&HEI_PNUM,sizeof(int));

            //プロセス0以外の場合、プロセス0にグレー画像情報を送信
            T_Send(0,&g[0][0],HEI_PNUM*WID*sizeof(uchar));

        }
    }

    //プロセス0のみ実行
    if(myrank==0){
        //計測時間の平均を計算
        double sum=0;
        for(i=0;i<10;i++){
            sum+=time[i];
        }
        double ave = sum/10;

        //計測時間の平均をファイルに保存と表示
        fprintf(fp,"%d,%f\n",PNUM,ave);
        printf("%d,%f\n",PNUM,ave);

        //結果を挿入するための関数
        int cnt_y=0;

        //他のプロセスからの結果を受信
        for(i=1;i<PNUM;i++){
            
            //他のプロセスから計算する行数を受信
            T_Recv(i,&HEI_PNUM,sizeof(int));

            //受信用のバッファ
            uchar recv[HEI_PNUM][WID];

            //受信
            T_Recv(i,&recv[0][0],HEI_PNUM*WID*sizeof(uchar));

            //結果表示用の画像情報に受信したグレー画像情報をコピー
            for(y=cnt_y;y<cnt_y+HEI_PNUM;y++){
                for(x=0;x<WID;x++){
                    result[y][x]=recv[y-cnt_y][x];
                }
            }
            cnt_y+=HEI_PNUM;
        }

        //グレー画像の表示(ファイル名=grey.xmp)
        generate_pgm(&result[0][0],WID,HEI,"grey-7.xmp");
    }

    return;
}

int main(void){
    //T_Runで並列呼び出し
    T_Run(PNUM,f); 
    return 0;
}