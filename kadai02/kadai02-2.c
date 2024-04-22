#include<stdio.h>
#include<stdlib.h>

//行列の要素数
int N=2;

//行列積を計算する関数
void matrix_product(double a[N][N], double b[N][N], double c[N][N]){
    //for文用の変数宣言
    int i;
    int j;
    int k;

    for(i=0; i<N; i++){
        for(j=0; j<N; j++){
            c[i][j] = 0;
            for(k=0; k<N; k++){
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

//並列で呼び出す関数
void f(void){
    //行列の宣言
    double a[N][N];

    //for文用の変数宣言
    int i;
    int j;
    int k;

    //結果表示用のファイルをオープン
    FILE *fp=fopen("kadai02-2.txt","a");

    //計測時間用の変数
    double s_time=0, e_time=0;

    //行列にダミーデータを代入
    for(i=0; i<N; i++){
        for(j=0; j<N; j++){
           //1～10のランダムな値を代入
            a[i][j] = rand()%10+1;
        }
    }

    //行列積の計算後の行列
    double c[N][N];

    //計測時間を10回格納するための配列
    double time[10];

    //計測した回数
    int time_cnt=0;

    while(time_cnt<10){
        //計測開始
        s_time = T_GetTime();

        //行列積の計算
        matrix_product(a, a, c);

        //計測終了
        e_time = T_GetTime();

        //計測時間を配列に格納
        time[time_cnt] = e_time-s_time;

        //計測回数をカウント
        time_cnt++;
    }

    //計測時間の平均を計算
    double sum_time=0;
    for(i=0; i<10; i++){
        sum_time += time[i];
    }
    double ave_time = sum_time/10;

    //結果の表示
    printf("%-4d %f\n", N, ave_time);
    fprintf(fp, "%-4d %f\n", N, ave_time);

    return;
}

int main(void){
    while(N<=2048){
        //関数fを1つ並列で呼び出す
        T_Run(1, f);
        N*=2;
    }
    return 0;
}