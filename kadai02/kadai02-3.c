#include<stdio.h>
#include<stdlib.h>

// 配列の要素数
int N=4;

//プロセスの数
#define P 2

//一つのプロセスが計算する行列の要素数
int M=0;

//行列積を計算する関数

void matrix_product(double a[M][N], double b[N][N], double c[M][N]){
    //for文用の変数宣言
    int i;
    int j;
    int k;

    //行列積を計算
    for(i=0; i<M; i++){
        for(j=0; j<N; j++){
            c[i][j]=0;
            for(k=0; k<N; k++){
                c[i][j] += a[i][k]*b[k][j];
            }
        }
    }
}

//並列で呼び出す関数
void f(void){
    //行列の宣言
    double a[N][N];

    //一つのプロセスが計算する行列の要素数を計算
    M=N/P;

    //for文用の変数宣言
    int i;
    int j;
    int k;

    //結果表示用のファイルをオープン
    FILE *fp=fopen("kadai02-3.txt","a");

    //計測時間用の変数
    double s_time=0, e_time=0;

    //自身のプロセス番号を取得
    int my_rank = T_GetMyNum();

    //行列にダミーデータを代入
    for(i=0; i<N; i++){
        for(j=0; j<N; j++){
            //1～10のランダムな値を代入
            a[i][j] = rand()%10+1;
        }
    }

    //計算後の行列を格納する配列
    double c[N][N];

    //一つのプロセスが計算後に格納する行列
    double tmp[M][N];

    //計測時間を10回格納するための配列
    double time[10];

    //計測した回数
    int time_cnt=0;

    while(time_cnt<10){
        if(my_rank==0){
            //プロセス0の処理
            s_time = T_GetTime();

            //行列積を計算
            matrix_product(a, a, tmp);

            //計算結果をcに格納
            for(i=0; i<M; i++){
                for(j=0; j<N; j++){
                    c[i][j] = tmp[i][j];
                }
            }

            int cnt=0;

            //他のプロセス全員から計算結果を受信
            while(cnt<P-1){
                //受信用の配列
                double tmp2[M][N];

                //プロセス番号を受信
                int from = T_Recv(T_ANY_NUM,tmp2, sizeof(double)*M*N);

                //受信した行列をcに格納
                for(i=0; i<M; i++){
                    for(j=0; j<N; j++){
                        c[from*M+i][j] = tmp2[i][j];
                    }
                }
                cnt++;
            }

            //計測時間を取得
            e_time = T_GetTime();

            //計測時間を配列に格納
            time[time_cnt] = e_time-s_time;

            //計測回数をカウント
            time_cnt++;

        }else {

            //プロセス1~P-1の処理
            //行列積を計算
            matrix_product(&a[M*my_rank], a, tmp);

            //プロセス0に計算結果を送信
            T_Send(0, tmp, sizeof(double)*M*N);
        }
    }

    //計測時間の平均を計算
    double sum_time=0;
    for(i=0; i<10; i++){
        sum_time += time[i];
    }
    double ave_time = sum_time/10;

    //計測時間の平均を出力
    printf("%-4d %f\n", N, ave_time);

    //計測時間の平均をファイルに書き込む
    fprintf(fp, "%d %-4d %f\n", P,N, ave_time);

    return;
}

int main(void){
    while(N<=2048){
        //関数fをPの数並列で呼び出す
        T_Run(P, f);
        N*=2;
    }
    return 0;
}