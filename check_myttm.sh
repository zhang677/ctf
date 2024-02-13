#!/bin/bash
K=12092
I=9184
J=28818
L=16
Tname=nell-2
Aname="/home/zgh23/code/ctf/ttm-A-$L.txt"
Cname="/home/zgh23/code/ctf/C-$Tname-$L.txt"
Bname="/scratch/zgh23/sparse_ten/$Tname-zero.tns"
mpirun -n 1 ./bin/myttm -tensor $Bname -dims $K,$I,$J -iter 1 -warmup 1 -ttmLDim $L -mode 0 -matrixA $Aname -matrixC $Cname
