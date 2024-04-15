#include<stdio.h>

#define N 4

#define PRONUM 2

void f(void){
    int i;

    //出力先のファイルをオープン
    FILE *fp=fopen("kadai01-2.txt","a");

    //実行時間を計測する用の変数
    double s_Time=0,e_Time=0;

    //プロセス番号の取得
    int myrank=T_GetMyNum();

    if(myrank==0){
        //プロセス0のみ実行
        //n個の整数を格納する配列を宣言
        int a[N];
        //n個の整数を格納する配列に値を代入
        for(i=0;i<N;i++){
            a[i]=i;
        }

        //現在の時間を開始時間に設定
        s_Time=T_GetTime();

        //配列をプロセス1に送信
        T_Send(1,a,N*sizeof(int));

        //配列をプロセス1から受信
        T_Recv(1,a,N*sizeof(int));

        //現在の時間を終了時間に設定
        e_Time=T_GetTime();

        //経過時間を出力
        fprintf(fp,"Time=%f | N : %d\n",e_Time-s_Time,N);

    }else if(myrank==1){
        //プロセス1のみ実行
        //n個の整数を格納する配列を宣言
        int b[N];
        
        //配列をプロセス0から受信
        T_Recv(0,b,N*sizeof(int));
        //受信した配列をプロセス0に送信
        T_Send(0,b,N*sizeof(int));


    }
    return;
}

int main(int argc, char *argv[]){
    //関数fをPRONUMの数並列に呼び出す
    T_Run(PRONUM,f);
    return 0;
}