#!/bin/bash
Tname=nips3
K=2482
I=2862
J=14036
it=20
wp=5
Bname="/scratch/zgh23/sparse_ten/$Tname-zero.tns"
Rname="/home/zgh23/code/ctf/ttm-$Tname.csv"
Ls=(4 8 16 32 64 128)
for L in ${Ls[@]}; do
    Cname="/home/zgh23/code/ctf/D-$Tname-4.txt"
    mpirun -n 1 ./bin/myttm_il -tensor $Bname -dims $K,$I,$J -iter $it -warmup $wp -ttmLDim $L -mode 1 -matrixC $Cname -result $Rname 
done