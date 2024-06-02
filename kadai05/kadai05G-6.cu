#include<stdio.h>
#include<stdlib.h>

//行列のサイズ
#define N 16

//x方向のブロック内のスレッド数
#define BSX 2
//y方向のブロック内のスレッド数
#define BSY 2

//行列を表示する(N<=16のときのみ)
void printMatrix(float a[][N],const char *str){
#if (N<=16)
    printf("==== %s ====\n",str);
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            printf("%4.0f ", a[i][j]);
        }
        printf("\n");
    }
#endif
}

//行列を初期化する関数
void initMatrix(float a[][N]){
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            //0以上10未満の乱数を生成
            a[i][j] =(float)((rand()/(RAND_MAX+1.0))*10);
        }
    }
}

//行列積を計算するカーネル関数
__global__ void mulMatrix(float (*a)[N],float (*b)[N],float (*c)[N]){
    
    int xid=blockIdx.x * blockDim.x+threadIdx.x;
    int yid=blockIdx.y * blockDim.y+threadIdx.y;

    //担当部分の行列積を計算
    if(xid < N && yid < N){
        c[yid][xid]=0.0;
        for(int k=0;k<N;k++){
            c[yid][xid] += a[yid][k] * b[k][xid];
        }
    }
}

int main(void){
    //ホスト上に確保した2次元配列（行列）
    float a[N][N],b[N][N],c[N][N],cs[N][N];

    float (*adev)[N], (*bdev)[N], (*cdev)[N]; //デバイス上に確保した2次元配列を指すポインタ

    //ホスト上の配列に初期値を設定する
    initMatrix(a);
    initMatrix(b);
    printMatrix(a, "a");
    printMatrix(b, "b");

    ////// ここからGPU版行列積 ///////////////////////

    //デバイス上に配列領域を確保し，その領域の先頭アドレスを取得する
    cudaMalloc(&adev, sizeof(float)*N*N);
    cudaMalloc(&bdev, sizeof(float)*N*N);
    cudaMalloc(&cdev, sizeof(float)*N*N);

    //a, bの内容をデバイスメモリ上の配列にコピーする
    cudaMemcpy(adev, a, sizeof(float)*N*N, cudaMemcpyHostToDevice);
    cudaMemcpy(bdev, b, sizeof(float)*N*N, cudaMemcpyHostToDevice);

    //スレッドブロック数とブロック内スレッド数の設定
    dim3 bdim(BSX, BSY); //ブロック内スレッド数
    dim3 gdim(N/BSX, N/BSY); //スレッドブロック数

    //合計N*N個のスレッドになる．
    //カーネル関数の並列呼び出し
    printf("\nGPU上の行列積を開始します\n");
    mulMatrix<<<gdim, bdim>>>(adev, bdev, cdev);
    cudaDeviceSynchronize();

    //計算結果(cdevが指す領域)をホスト側にコピーする
    cudaMemcpy(c, cdev, sizeof(float)*N*N, cudaMemcpyDeviceToHost);
    printMatrix(c, "c");

    //デバイス上に確保した配列領域を解放する
    cudaFree(adev);
    cudaFree(bdev);
    cudaFree(cdev);

    ////// ここまでGPU版行列積 ////////////////////////////////////////

    ////// ここから， 確認のためのCPU版行列積 ///////////////////////
    printf("\nCPU上の行列積を開始します\n");
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            cs[i][j] = 0.0;
        }
    }
    for (int i=0; i<N; i++){
        for (int k=0; k<N; k++){
            for (int j=0; j<N; j++){
                cs[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    printMatrix(cs, "cs");

    ////// ここまでCPU版行列積 ////////////////////////////////////////

    return 0;
}