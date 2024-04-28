#include<stdio.h>

//行列の要素数
#define ROW 3

//プロセス数
#define PROCESS 3

//行列の積を計算する関数
void matrix_product(int a[ROW][ROW], int b[ROW][ROW], int c[ROW][ROW]){
  int i, j, k;
  for(i = 0; i < ROW; i++){
    for(j = 0; j < ROW; j++){
      c[i][j] = 0;
      for(k = 0; k < ROW; k++){
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}

//並列に呼び出す関数
void f(){

}

int main(){

    //PROCESS数だけプロセスを生成
    T_RUN(PROCESS, f);

    return 0;
}