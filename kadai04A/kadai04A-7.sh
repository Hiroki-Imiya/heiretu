#!/bin/sh

# このスクリプトは,kadai04A-7.cをプロセス数を変えながら実行するスクリプトである.

tccrun -DPNUM=2 kadai04A-7.c
tccrun -DPNUM=4 kadai04A-7.c
tccrun -DPNUM=8 -p kadai04A-7.c
tccrun -DPNUM=16 -p kadai04A-7.c
