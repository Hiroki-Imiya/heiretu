#include<stdio.h>
//MPIを使用するためのヘッダファイル
#include<mpi.h>

#define N 4

int main(int argc, char *argv[]){
    //MPIの初期化
    MPI_Init(&argc,&argv);

    //受信関数の終了ステータスを格納する変数を宣言
    MPI_Status status;

    //プロセス番号の取得
    int myrank;
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

    if(myrank==0){
        //プロセス0のみ実行
        //n個の整数を格納する配列を宣言
        int a[N];
        //n個の整数を格納する配列に値を代入
        for(int i=0;i<N;i++){
            a[i]=i;
        }

        //現在の時間を開始時間に設定
        double s_Time=MPI_Wtime();

        //配列をプロセス1に送信
        MPI_Send(a,N,MPI_INT,1,0,MPI_COMM_WORLD);

        //配列をプロセス1から受信
        MPI_Recv(a,N,MPI_INT,1,0,MPI_COMM_WORLD,&status);

        //現在の時間を終了時間に設定
        double e_Time=MPI_Wtime();

        //経過時間を出力
        printf("%f,%d\n",e_Time-s_Time,N);

    }else if(myrank==1){
        //プロセス1のみ実行
        //n個の整数を格納する配列を宣言
        int b[N];
        
        //配列をプロセス0から受信
        MPI_Recv(b,N,MPI_INT,0,0,MPI_COMM_WORLD,&status);
        //受信した配列をプロセス0に送信
        MPI_Send(b,N,MPI_INT,0,0,MPI_COMM_WORLD);

    }

    //MPIの終了処理
    MPI_Finalize();

    return 0;
}