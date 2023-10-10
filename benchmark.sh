#!/bin/bash
#Js=(16 64 128 256 512 1024 2048)
Js=(4 8 32)
K=12092
L=9184
I=28818
it=20
wp=5
Bname="/scratch/zgh23/sparse_ten/nell-2-zero.tns"
for J in ${Js[@]}; do
    Dname="/home/zgh23/code/ctf/D-nell-2-$J.txt"
    Cname="/home/zgh23/code/ctf/C-nell-2-$J.txt"
    mpirun -n 1 ./bin/mymttkrp -tensor $Bname -dims $K,$L,$I -iter $it -warmup $wp -mttkrpLDim $J -mode 2 -matrixD $Dname -matrixC $Cname
done