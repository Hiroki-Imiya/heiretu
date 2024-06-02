#include<stdio.h>

// カーネル関数
__global__ void f(void){
    //一意識別番号を取得
    int myid = blockIdx.x * blockDim.x + threadIdx.x;

    //一意識別番号　その計算に用いた値を表示
    printf("myid = %-2d , bDim=(%d %d %d) , bIdx=(%d %d %d) , tIdx=(%d %d %d)\n",
        myid,
        blockDim.x, blockDim.y, blockDim.z,
        blockIdx.x, blockIdx.y, blockIdx.z,
        threadIdx.x, threadIdx.y, threadIdx.z
    );
}

int main(void){
    // カーネル関数の呼び出し 3ブロック4スレッド
    f<<<3,4>>>();
    // カーネル関数の終了を待つ
    cudaDeviceSynchronize();
    return 0;
}