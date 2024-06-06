#include<stdio.h>
#include<stdlib.h>
//MPIを使用するためのヘッダファイル
#include<mpi.h>

//行列の要素数
#define ROW 16

int main(int argc, char *argv[]){
    //MPIの初期化
    MPI_Init(&argc,&argv);

    //受信関数の終了ステータスを格納する変数を宣言
    MPI_Status status;

    //プロセス番号の取得
    int myrank;
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

    //プロセス数の取得
    int pnum;
    MPI_Comm_size(MPI_COMM_WORLD,&pnum);

    //各プロセスが担当する行列の行数
    int row_pnum=ROW/pnum;

    //行列の宣言
    double a[row_pnum][ROW];

    //行列の初期化
    for(int i=0;i<row_pnum;i++){
        for(int j=0;j<ROW;j++){
            a[i][j]=rand()%10+1;
        }
    }

    //行列Bの宣言
    double b[ROW];

    //行列Cの宣言
    double c[row_pnum][ROW];

    //行列Cの初期化
    for(int i=0;i<row_pnum;i++){
        for(int j=0;j<ROW;j++){
            c[i][j]=0;
        }
    }

    //行列aの表示
    printf("myrank=%d\n",myrank);
    for(int i=0;i<row_pnum;i++){
        for(int j=0;j<ROW;j++){
            printf("%f ",a[i][j]);
        }
        printf("\n");
    }

    //計算する列の番号
    int col=0;

    //colがROWになるまでループ
    while(col<ROW){

        //プロセス数の数だけループ
        for(int i=0;i<pnum;i++){

            //iとmyrankと違う場合送信
            if(i!=myrank){
                
                //送信専用の配列
                double send[row_pnum];

                //行列の列を送信専用の配列にコピー
                for(int j=0;j<row_pnum;j++){
                    send[j]=a[j][col];
                }

                //行列の列を送信
                MPI_Send(send,row_pnum,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
            }else{
                //自身のプロセス番号と同じ場合

                //aの列をbにコピー
                for(int j=0;j<row_pnum;j++){
                    b[j+myrank*row_pnum]=a[j][col];
                }

                //他のプロセスから行列の列を受信
                for(int j=0;j<pnum;j++){
                    if(j!=myrank){
                        //受信専用の配列
                        double recv[row_pnum];

                        //行列の列を受信専用の配列にコピー
                        MPI_Recv(recv,row_pnum,MPI_DOUBLE,j,0,MPI_COMM_WORLD,&status);

                        //行列の列を行列にコピー
                        for(int k=0;k<row_pnum;k++){
                            b[k+j*row_pnum]=recv[k];
                        }
                    }
                }
            }
        }

        //行列Bの表示
        printf("myrank=%d\n",myrank);
        for(int i=0;i<ROW;i++){
            printf("%f ",b[i]);
        }
        printf("\n");

         //行列Aと行列Bを掛けて行列Cに代入
        for(int i=0;i<row_pnum;i++){
            for(int j=0;j<ROW;j++){
                c[i][col]+=a[i][j]*b[j];
            }
        }

        //列の番号をインクリメント
        col++;
    }

    //他のプロセスと同期
    MPI_Barrier(MPI_COMM_WORLD);

    //プロセス0から順番に行列Cを出力
    for(int i=0;i<pnum;i++){
        if(myrank==i){
            printf("myrank=%d\n",myrank);
            for(int j=0;j<row_pnum;j++){
                for(int k=0;k<ROW;k++){
                    printf("%f ",c[j][k]);
                }
                printf("\n");
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    //MPIの終了処理
    MPI_Finalize();

    return 0;
}