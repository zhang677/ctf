#!/bin/bash
K=12092
L=9184
I=28818
J=32
Aname="/home/zgh23/code/ctf/A.mtx"
Dname="/home/zgh23/code/ctf/D.txt"
Cname="/home/zgh23/code/ctf/C.txt"
Bname="/scratch/zgh23/sparse_ten/nell-2-zero.tns"
mpirun -n 1 ./bin/mymttkrp -tensor $Bname -dims $K,$L,$I -iter 1 -warmup 1 -mttkrpLDim $J -mode 1 -matrixA $Aname -matrixD $Dname -matrixC $Cname

Atxt="${Aname%.*}.txt"
cp $Aname $Atxt
line_count=$(wc -l < $Aname)
echo $line_count
sed -i "1i $I $J $line_count" $Aname
sed -i '1i %%MatrixMarket matrix coordinate real general' $Aname
sed -n '1,4p' $Aname