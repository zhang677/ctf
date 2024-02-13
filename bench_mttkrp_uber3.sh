#!/bin/bash
# [182, 1139, 1716]
Tname=uber3
K=183
L=1140
I=1717
it=20
wp=5
Bname="/scratch/zgh23/sparse_ten/$Tname-zero.tns"
Js=(4 8 16 32 64 128 256 512 1024 2048)
for J in ${Js[@]}; do
    Dname="/home/zgh23/code/ctf/D-$Tname-$J.txt"
    Cname="/home/zgh23/code/ctf/C-$Tname-$J.txt"
    mpirun -n 1 ./bin/mymttkrp -tensor $Bname -dims $K,$L,$I -iter $it -warmup $wp -mttkrpLDim $J -mode 2 -matrixD $Dname -matrixC $Cname
done
# J=$1
# Dname="/home/zgh23/code/ctf/D-$Tname-$J.txt"
# Cname="/home/zgh23/code/ctf/C-$Tname-$J.txt"
# Aname="/home/zgh23/code/ctf/A-$Tname-$J.txt"
# mpirun -n 1 ./bin/mymttkrp -tensor $Bname -dims $K,$L,$I -iter $it -warmup $wp -mttkrpLDim $J -mode 2 -matrixD $Dname -matrixC $Cname -matrixA $Aname