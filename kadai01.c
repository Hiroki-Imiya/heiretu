#include<stdio.h>

#define n 4

void f(void){
    int i;

    //プロセス番号の取得
    int myrank=T_GetMyNum();

    if(myrank==0){
        //プロセス0のみ実行
        //n個の整数を格納する配列を宣言
        int a[n];
        //n個の整数を格納する配列に値を代入
        for(i=0;i<n;i++){
            a[i]=i;
        }
        //配列をプロセス1に送信
        T_Send(1,a,n);

        //配列をプロセス1から受信
        T_Recv(1,a,n);

    }else if(myrank==1){
        //プロセス1のみ実行
        //n個の整数を格納する配列を宣言
        int b[n];
        //配列をプロセス0から受信
        T_Recv(0,b,n);
        //受信した配列をプロセス0に送信
        T_Send(0,b,n);
    }
    return;
}

int main(int argc, char *argv[]){
    //関数fを4つ並列に呼び出す
    T_Run(4,f);
    return 0;
}