#!/bin/sh
#kadai03.cをtccrunを用いてコンパイルし、実行するスクリプト
#-Dオプションを使用して,実行時にPNUMとROWの値を指定する
#PNUM:プロセス数,ROW:行数

tccrun -DPNUM=2 -DROW=4 kadai03.c
tccrun -DPNUM=2 -DROW=8 kadai03.c
tccrun -DPNUM=2 -DROW=16 kadai03.c
tccrun -DPNUM=2 -DROW=32 kadai03.c
tccrun -DPNUM=2 -DROW=64 kadai03.c
tccrun -DPNUM=2 -DROW=128 kadai03.c
tccrun -DPNUM=2 -DROW=256 kadai03.c
tccrun -DPNUM=2 -DROW=512 kadai03.c
tccrun -DPNUM=2 -DROW=1024 kadai03.c
tccrun -DPNUM=2 -DROW=2048 kadai03.c
tccrun -DPNUM=4 -DROW=4 kadai03.c
tccrun -DPNUM=4 -DROW=8 kadai03.c
tccrun -DPNUM=4 -DROW=16 kadai03.c
tccrun -DPNUM=4 -DROW=32 kadai03.c
tccrun -DPNUM=4 -DROW=64 kadai03.c
tccrun -DPNUM=4 -DROW=128 kadai03.c
tccrun -DPNUM=4 -DROW=256 kadai03.c
tccrun -DPNUM=4 -DROW=512 kadai03.c
tccrun -p -DPNUM=4 -DROW=1024 kadai03.c
tccrun -p -DPNUM=4 -DROW=2048 kadai03.c
tccrun -p -DPNUM=8 -DROW=4 kadai03.c
tccrun -p -DPNUM=8 -DROW=8 kadai03.c
tccrun -p -DPNUM=8 -DROW=16 kadai03.c
tccrun -p -DPNUM=8 -DROW=32 kadai03.c
tccrun -p -DPNUM=8 -DROW=64 kadai03.c
tccrun -p -DPNUM=8 -DROW=128 kadai03.c
tccrun -p -DPNUM=8 -DROW=256 kadai03.c
tccrun -p -DPNUM=8 -DROW=512 kadai03.c
tccrun -p -DPNUM=8 -DROW=1024 kadai03.c
tccrun -p -DPNUM=8 -DROW=2048 kadai03.c
tccrun -p -DPNUM=16 -DROW=4 kadai03.c
tccrun -p -DPNUM=16 -DROW=8 kadai03.c
tccrun -p -DPNUM=16 -DROW=16 kadai03.c  
tccrun -p -DPNUM=16 -DROW=32 kadai03.c
tccrun -p -DPNUM=16 -DROW=64 kadai03.c
tccrun -p -DPNUM=16 -DROW=128 kadai03.c
tccrun -p -DPNUM=16 -DROW=256 kadai03.c
tccrun -p -DPNUM=16 -DROW=512 kadai03.c
tccrun -p -DPNUM=16 -DROW=1024 kadai03.c
tccrun -p -DPNUM=16 -DROW=2048 kadai03.c

