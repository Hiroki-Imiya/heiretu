#include<stdio.h>
#include<stdlib.h>

//行列の要素数
//#define ROW 4
//プロセス数
//#define PNUM 2
//以上の値はコンパイル時に指定する -Dオプションで指定する

//各プロセスが担当する行列の行数
#define ROW_PNUM (ROW / PNUM)

//並列に呼び出す関数
void f(void){

	//while文用の変数
	int cnt;

    //計測時間を書き込むファイル(追加書き込みモードで開く)
    FILE *fp=fopen("kadai03.txt","a");


    //計測回数の変数
    int cnt_time=0;

	//for文用の変数宣言
	int i;
	int j;

	//計測用の変数
	double start,end;

	//計測時間を10回保存するための配列
	double time[10];

	//自身のプロセス番号を取得
	int my_rank=T_GetMyNum();

	//計算後の行列
	double c[ROW_PNUM][ROW];

	//行列を宣言
	double a[ROW_PNUM][ROW];

	//初期化(1～10をランダムで代入)
	for(i = 0; i < ROW_PNUM; i++){
		for(j = 0; j < ROW; j++){
			a[i][j] = rand() % 10 + 1;
		}
	}

	//他のプロセスと同期
	T_Barrier();

	//行列Bを宣言
	double b[ROW];

    //計測回数が10回になるまでループ
    while(cnt_time<10){
        cnt=0;

        //計測開始(プロセス0のみ)
        if(my_rank==0){
            start=T_GetTime();
        }

        //cntがROW未満の間ループ
        while(cnt < ROW){

            //他のプロセスに一列を送信
            for(i=0;i<PNUM;i++){
                //自身のプロセス番号と違う場合
                if(i != my_rank){
                    //送信用の配列
                    double send[ROW_PNUM];

                    //行列Aの対応する列を代入
                    for(j=0;j<ROW_PNUM;j++){
                        send[j]=a[j][cnt];
                    }

                    //他のプロセスに送信
                    T_Send(i,send,ROW_PNUM*sizeof(double));
                }
            }

            int pro_cnt=0;

            //プロセス数だけループ
            while(pro_cnt < PNUM){

                //プロセス番号が自身のプロセス番号と違う場合
                if(pro_cnt != my_rank){

                    //受信用の配列
                    double recv[ROW_PNUM];

                    //他のプロセスから受信
                    int from=T_Recv(T_ANY_NUM,recv,ROW_PNUM*sizeof(double));

                    //行列の対応する行に代入
                    for(i = 0;i<ROW_PNUM;i++){
                        b[i+from*ROW_PNUM]=recv[i];
                    }
                }else{
                    //自身のプロセス番号の場合
                    //行列Bに行列Aの対応する列を代入
                    for(i=0;i<ROW_PNUM;i++){
                        b[i+my_rank*ROW_PNUM]=a[i][cnt];
                    }
                }

                //プロセス番号をインクリメント
                pro_cnt++;
            }

            //行列Aと行列Bを掛けて行列Cに代入
            for(i=0;i<ROW_PNUM;i++){
                for(j=0;j<ROW;j++){
                    c[i][cnt]+=a[i][j]*b[j];
                }
            }

            //cntをインクリメント
            cnt++;
        }

        //他のプロセスと同期
        T_Barrier();

        //計測終了(プロセス0のみ)
        if(my_rank==0){
            end=T_GetTime();

            //計測時間を配列に代入
            time[cnt_time]=end-start;
            
        }

        //計測回数をインクリメント
        cnt_time++;
    }

    //計測時間の平均をプロセス0のみ表示(ファイルにも書き込む)
    if(my_rank==0){
        double sum=0;
        for(i=0;i<10;i++){
            sum+=time[i];
        }
        printf("%d %d %f\n",PNUM,ROW,sum/10);
        fprintf(fp,"%d %d %f\n",PNUM,ROW,sum/10);
    }

    //ファイルを閉じる
    fclose(fp);

    return;

}

int main(void){

    //PNUM数だけプロセスを生成
    T_Run(PNUM, f);

    return 0;
}