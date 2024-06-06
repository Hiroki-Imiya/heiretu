#include<stdio.h>
//MPIを使うためのライブラリをインクルード
#include<mpi.h>
#include<string.h>

#define TAG 10
#define SIZE 20

int main(int argc,char *argv[]){
    //通信データの格納場所(通信バッファ)
    char data[SIZE];
    //ランクを格納する変数を宣言
    int myrank;

    //受信関数の終了ステータスを格納する変数を宣言
    MPI_Status status;

    //MPIの初期化
    MPI_Init(&argc,&argv);
    //自身のランクを取得
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    
    if(myrank == 0){
        //ランク0の場合

        //通信バッファに文字列を書き込む
        sprintf(data,"Hello World!");
        //文字列を送信(送信バッファの先頭アドレス,送信データ数,送信データ型,送信先ランク,タグ,グループ)
        MPI_Send(data,SIZE,MPI_CHAR,1,TAG,MPI_COMM_WORLD);

    }else if(myrank == 1){
        //ランク1の場合

        //文字列を受信(受信バッファの先頭アドレス,受信データ数,受信データ型,送信元ランク,タグ,グループ,終了ステータス変数)
        MPI_Recv(data,SIZE,MPI_CHAR,0,TAG,MPI_COMM_WORLD,&status);
        //受信した文字列を表示
        printf("Rank%d: rank0 says, \"%s\"!!\n", myrank, data);
    }

    //MPIの終了
    MPI_Finalize();
    return 0;
}