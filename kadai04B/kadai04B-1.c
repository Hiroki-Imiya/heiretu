#include<stdio.h>

//連立方程式における未知数の数
#define N 4

//並列に呼び出す関数
void f(){

    //for文用の変数(i,j)
    int i,j;
    
    //連立方程式の係数行列
    double a[N][N] ;
    //連立方程式の右辺の定数項
    double b[N] ;

    //係数行列の初期化
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            a[i][j] = i+j+1;
        }
    }

    //右辺の定数項の初期化
    for(i=0;i<N;i++){
        b[i] = i+1;
    }

    //連立方程式の解を求める
    for(i=0;i<N;i++){
        //対角成分を1にする
        double d = a[i][i];
        for(j=0;j<N;j++){
            a[i][j] /= d;
        }
        b[i] /= d;

        //i行目の値を使ってi行目以外の値を更新
        for(j=0;j<N;j++){
            if(i!=j){
                double c = a[j][i];
                for(int k=0;k<N;k++){
                    a[j][k] -= c*a[i][k];
                }
                b[j] -= c*b[i];
            }
        }
    }

    //解の表示
    for(i=0;i<N;i++){
        printf("x[%d] = %f\n",i,b[i]);
    }

    return;

}

int main(void){
    //T_Runで並列呼び出し
    //T_Run(1,f);
    f();
    return 0;
}