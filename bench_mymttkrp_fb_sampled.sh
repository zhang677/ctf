#!/bin/bash
Tname=freebase_sampled
J=$1
K=38955429
L=38955429
I=532
it=20
wp=5
Bname="/scratch/zgh23/sparse_ten/$Tname-zero.tns"
Dname="/scratch/zgh23/sparse_ten/D-$Tname-$J.txt"
Cname="/scratch/zgh23/sparse_ten/C-$Tname-$J.txt"
mpirun -n 4 ./bin/mymttkrp -tensor $Bname -dims $K,$L,$I -iter $it -warmup $wp -mttkrpLDim $J -mode 2 -matrixD $Dname -matrixC $Cname
