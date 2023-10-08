#!/bin/bash
K=12092
L=9184
I=28818
J=32
it=1
wp=1
Dname="/home/zgh23/code/ctf/D.txt"
Cname="/home/zgh23/code/ctf/C.txt"
Bname="/scratch/zgh23/sparse_ten/nell-2-zero.tns"
mpirun -n 1 ./bin/mymttkrp -tensor $Bname -dims $K,$L,$I -iter $it -warmup $wp -mttkrpLDim $J -mode 2 -matrixD $Dname -matrixC $Cname
