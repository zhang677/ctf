#!/bin/bash
Tname=facebook
Ls=(4 8 16 32 64 128 256 512 1024 2048)
K=1504
I=42390
J=39986
it=20
wp=5
Bname="/scratch/zgh23/sparse_ten/$Tname-zero.tns"
for L in ${Ls[@]}; do
    Cname="/home/zgh23/code/ctf/C-$Tname-$L.txt"
    mpirun -n 1 ./bin/myttm -tensor $Bname -dims $K,$I,$J -iter $it -warmup $wp -ttmLDim $L -mode 1 -matrixC $Cname
done