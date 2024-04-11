#include<stdio.h>

#define N 4

#define PRONUM 2

void f(void){
    int i;

    int cnt=0;

    //出力先のファイルをオープン
    FILE *fp=fopen("kadai01-4.txt","a");

    //送信命令の実行時間を計測する用の変数
    double Send_s_Time=0,Send_e_Time=0;

    //受信命令の実行時間を計測する用の変数
    double Recv_s_Time=0,Recv_e_Time=0;

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


        //以下を100回繰り返す

        while(cnt<100){
            //現在の時間を開始時間に設定
            Send_s_Time=T_GetTime();
            //配列をプロセス1に送信
            T_Send(1,a,N);
            //現在の時間を終了時間に設定
            Send_e_Time=T_GetTime();

            //現在の時間を開始時間に設定
            Recv_s_Time=T_GetTime();
            //配列をプロセス1から受信
            T_Recv(1,a,N);
            //現在の時間を終了時間に設定
            Recv_e_Time=T_GetTime();

            //自身のプロセス番号と送信命令の実行時間、受信命令の実行時間を出力
            fprintf(fp,"myrank=%d | Send Time=%f | Recv Time=%f\n",myrank,Send_e_Time-Send_s_Time,Recv_e_Time-Recv_s_Time);

            cnt++;
        }

    }else if(myrank==1){
        //プロセス1のみ実行
        //n個の整数を格納する配列を宣言
        int b[N];

        //以下を100回繰り返す
        while(cnt<100){

            //現在の時間を開始時間に設定
            Recv_s_Time=T_GetTime();
            //配列をプロセス0から受信
            T_Recv(0,b,N);
            //現在の時間を終了時間に設定
            Recv_e_Time=T_GetTime();

            //現在の時間を開始時間に設定
            Send_s_Time=T_GetTime();
            //受信した配列をプロセス0に送信
            T_Send(0,b,N);
            //現在の時間を終了時間に設定
            Send_e_Time=T_GetTime();

            //自身のプロセス番号と送信命令の実行時間、受信命令の実行時間を出力
            fprintf(fp,"myrank=%d | Send Time=%f | Recv Time=%f\n",myrank,Send_e_Time-Send_s_Time,Recv_e_Time-Recv_s_Time);

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