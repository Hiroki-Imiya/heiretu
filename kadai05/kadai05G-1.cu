#include<stdio.h>

// カーネル関数
__global__ void f(void){
    printf("Hello, World!\n");
}

int main(void){
    // カーネル関数の呼び出し
    f<<<2,4>>>();
    // カーネル関数の終了を待つ
    cudaDeviceSynchronize();
    return 0;
}