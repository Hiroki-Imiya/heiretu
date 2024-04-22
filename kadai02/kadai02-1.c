#include<stdio.h>

// 配列の要素数
#define N 4

//行列積を計算する関数
void matrix_product(double a[N][N], double b[N][N], double c[N][N]){
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            c[i][j] = 0;
            for(int k=0; k<N; k++){
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main(void){
    //行列の宣言
    double a[N][N];

    //行列にダミーデータを代入
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            a[i][j] = i+j;
        }
    }

    //行列の表示
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            printf("%f ", a[i][j]);
        }
        printf("\n");
    }

    //行列積の計算後の行列
    double c[N][N];

    //行列積の計算
    matrix_product(a, a, c);

    //結果の表示
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            printf("%f ", c[i][j]);
        }
        printf("\n");
    }

    return 0;
}