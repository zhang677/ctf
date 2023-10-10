#!/bin/bash
Ls=(4 8 16 32 64 128 256 512 1024 2048)
K=12092
I=9184
J=28818
it=20
wp=5
Bname="/scratch/zgh23/sparse_ten/nell-2-zero.tns"
for L in ${Ls[@]}; do
    Cname="/home/zgh23/code/ctf/C-nell-2-$L.txt"
    mpirun -n 1 ./bin/myttm -tensor $Bname -dims $K,$I,$J -iter $it -warmup $wp -ttmLDim $L -mode 1 -matrixC $Cname
done