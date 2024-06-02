#include<stdio.h>
#include<stdlib.h>

#define N 8

//配列を表示する
void printArray(int a[],int size,const char *str){
    printf("%s",str);
    for(int i=0; i<size; i++){
        printf("%4d ", a[i]);
    }
    printf("\n");
}

//配列を初期化する関数
void initArray(int a[],int size){
    for(int i=0; i<size; i++){
        //0以上100未満の乱数を生成
        a[i] =(int)((rand()/(RAND_MAX+1.0))*100);
    }
}

//a[]+b[]の結果をc[]に格納する
__global__ void addArray(int *a ,int *b,int *c){
    //一意識別番号の取得
    int id=blockIdx.x * blockDim.x+threadIdx.x;

    if(id<N){
        c[id]=a[id]+b[id];
    }
}

int main(void){
    //ホスト上に確保した日あ列
    int a[N],b[N],c[N];

    //デバイス上に確保した配列へのポインタ
    int *adev,*bdev,*cdev;

    //デバイス上に配列領域を確保し，その領域の先頭アドレスを取得する
    cudaMalloc(&adev, N * sizeof(int));
    cudaMalloc(&bdev, N * sizeof(int));
    cudaMalloc(&cdev, N * sizeof(int));

    //ホスト上の配列に初期値を設定する
    initArray(a, N);
    initArray(b, N);
    printArray(a,N,"a");
    printArray(b,N,"b");

    //a,bの内容をデバイスメモリ上の配列にコピーする
    cudaMemcpy(adev, a, N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(bdev, b, N * sizeof(int), cudaMemcpyHostToDevice);

    //カーネル関数の変数呼び出し
    addArray<<<4,4>>>(adev,bdev,cdev);
    cudaDeviceSynchronize();

    //計算結果(cdevが指す領域)をホスト側にコピーする
    cudaMemcpy(c, cdev, N * sizeof(int), cudaMemcpyDeviceToHost);
    printArray(c, N, "c");

    //デバイス上に確保した配列領域を解放する
    cudaFree(adev);
    cudaFree(bdev);
    cudaFree(cdev);

    return 0;
}