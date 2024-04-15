#include<stdio.h>

#define N 4

#define PRONUM 2

void f(void){
    int i;

    int cnt=0;

    //出力先のファイルをオープン
    FILE *fp=fopen("kadai01-5.txt","a");

    //実行時間を計測する用の変数
    double s_Time=0,e_Time=0;

    //プロセス番号の取得
    int myrank=T_GetMyNum();

    if(myrank==0){
        //プロセス0のみ実行
        
        //1個の整数を宣言(仮の値として1を代入)
        int a[1];
        a[0]=1;

        //現在の時間を開始時間に設定
        s_Time=T_GetTime();

        //以下をN回繰り返す

        while(cnt<N){
            //配列をプロセス1に送信
            T_Send(1,a,1*sizeof(int));

            //配列をプロセス1から受信
            T_Recv(1,a,1*sizeof(int));

            cnt++;
        }

        //現在の時間を終了時間に設定
        e_Time=T_GetTime();

        //経過時間を出力
        fprintf(fp,"Time=%f | N : %d\n",e_Time-s_Time,N);

    }else if(myrank==1){
        //プロセス1のみ実行
        //n個の整数を格納する配列を宣言
        int b[1];

        //以下を100回繰り返す
        while(cnt<N){
            //配列をプロセス0から受信
            T_Recv(0,b,1*sizeof(int));
            //受信した配列をプロセス0に送信
            T_Send(0,b,1*sizeof(int));

            cnt++;
        }
    }
    return;
}

int main(int argc, char *argv[]){
    //関数fをPRONUMの数並列に呼び出す
    T_Run(PRONUM,f);
    return 0;
}