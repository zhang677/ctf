#!/bin/bash
Tname=patents
J=$1
K=46
L=239172
I=239172
it=20
wp=5
Bname="/scratch/zgh23/sparse_ten/$Tname.tns"
Dname="/home/zgh23/code/ctf/D-$Tname-$J.txt"
Cname="/home/zgh23/code/ctf/C-$Tname-$J.txt"
mpirun -n 1 ./bin/mymttkrp -tensor $Bname -dims $K,$L,$I -iter $it -warmup $wp -mttkrpLDim $J -mode 2 -matrixD $Dname -matrixC $Cname