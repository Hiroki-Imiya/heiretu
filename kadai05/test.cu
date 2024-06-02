#include<stdio.h>
#define N 10

//配列を表示する
void printArray(int *array, int size){
    for(int i=0; i<size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

//gpu上で実行する関数，__global__を付けるとgpu上で実行される関数を表す
__global__ void f(int k,int *p,int n){
    //自スレッドの識別番号を取得
    int myid = blockIdx.x * blockDim.x + threadIdx.x;

    //自分が担当する要素に処理を施す
    if(0<=myid && myid<n){
        p[myid] = p[myid] * k;
    }
}

int main(void){
    int i,a[N];

    //デバイス(gpu)上のメモリアドレス.配列aをgpu上にコピーする場合，
    //aのデバイス上の領域という意味でよくa_devという名前を使う
    int *a_dev;

    //配列a(cpu)の初期化
    for(i=0; i<N; i++){
        a[i] = i;
    }
    printf("before: ");printArray(a,N);

    //gpu上に整数10個分の領域を確保し，そのアドレスをa_devに代入
    cudaMalloc(&a_dev, N*sizeof(int));

    //配列aをgpu上にコピー
    cudaMemcpy(a_dev, a, N*sizeof(int), cudaMemcpyHostToDevice);

    //関数fをgpu上で並列に呼び出しする．1×NのN並列で呼び出す
    f<<<1,N>>>(100,a_dev,N);

    //gpu上での処理が終わるまで待つ
    cudaDeviceSynchronize();

    //gpu上の配列a_devをcpu上の配列aにコピー
    cudaMemcpy(a, a_dev, N*sizeof(int), cudaMemcpyDeviceToHost);

    printf("after: ");printArray(a,N);

    //gpu上で確保した領域を解放
    cudaFree(a_dev);

    return 0;
}
