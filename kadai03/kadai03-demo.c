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


	//行列Aを表示
	if(my_rank == 0){
		printf("行列A\n");
	}

	//他のプロセスと同期
	T_Barrier();

	cnt=0;
	while(cnt<PNUM){
		//他のプロセスと同期
		T_Barrier();

		if(my_rank == cnt){
			//行列Aを表示
			for(i=0;i<ROW_PNUM;i++){
				for(j=0;j<ROW;j++){
					printf("%f ",a[i][j]);
				}
				printf("      %d\n",my_rank);
			}
		}else{
			//1秒待機
			sleep(1);
		}
		cnt++;
	}

	//行列Bを宣言
	double b[ROW];

	cnt=0;

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

	//行列Cを表示
	if(my_rank==0){
		printf("行列C\n");
	}

	cnt=0;

	//PNUM数だけループ
	while(cnt<PNUM){
		//他のプロセスと同期
		T_Barrier();

		//自身のプロセス番号と同じ場合
		if(my_rank==cnt){
			//行列Cを表示
			for(i=0;i<ROW_PNUM;i++){
				for(j=0;j<ROW;j++){
					printf("%f ",c[i][j]);
				}
				printf("      %d\n",my_rank);
			}
		}else{
			//1秒待機
			sleep(1);
		}
		cnt++;
	}

}

int main(void){

    //PNUM数だけプロセスを生成
    T_Run(PNUM, f);

    return 0;
}