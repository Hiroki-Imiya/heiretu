#include<stdio.h>

#define N 4

#define PRONUM 1

void f(void){
    int i;

    int cnt=0;

    //出力先のファイルをオープン
    FILE *fp=fopen("kadai01-6.txt","a");

    //実行時間を計測する用の変数
    double s_Time=0,e_Time=0;

    //プロセス番号の取得
    int myrank=T_GetMyNum();


    //n個の整数を格納する配列aを宣言
    int a[N];

    //n個の整数を格納する配列bを宣言
    int b[N];
    //n個の整数を格納する配列aに値を代入
    for(i=0;i<N;i++){
        a[i]=i;
    }

    //現在の時間を開始時間に設定
    s_Time=T_GetTime();

    //以下を100回繰り返す

    while(cnt<100){
        //memcpyを用いて配列aを配列bにコピー
        memcpy(b,a,sizeof(int)*N);

        //menmcpyを用いて配列bを配列aにコピー
        memcpy(a,b,sizeof(int)*N);

        cnt++;
    }

    //現在の時間を終了時間に設定
    e_Time=T_GetTime();

    //経過時間を出力
    fprintf(fp,"Time=%f | N : %d\n",e_Time-s_Time,N);
    return;
}

int main(int argc, char *argv[]){
    //関数fをPRONUMの数並列に呼び出す
    T_Run(PRONUM,f);
    return 0;
}