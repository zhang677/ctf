#!/bin/bash
Tname=nips3
K=2482
L=2862
I=14036
it=20
wp=5
Bname="/scratch/zgh23/sparse_ten/$Tname-zero.tns"
Rname="/home/zgh23/code/ctf/mttkrp-$Tname.csv"
Js=(4 8 16 32 64 128) # 256 512 1024 2048)
for J in ${Js[@]}; do
    Dname="/scratch/zgh23/sparse_ten/generated/D-$Tname-4.txt"
    Cname="/scratch/zgh23/sparse_ten/generated/C-$Tname-4.txt"
    mpirun -n 1 ./bin/mymttkrp -tensor $Bname -dims $K,$L,$I -iter $it -warmup $wp -mttkrpLDim $J -mode 2 -matrixD $Dname -matrixC $Cname -result $Rname
done