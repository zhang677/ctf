#!/bin/bash
Tname=nips3
K=2482
L=2862
I=14036
J=$1
Aname="/home/zgh23/code/ctf/A-$Tname-$J.mtx"
Bname="/scratch/zgh23/sparse_ten/$Tname-zero.tns"
Dname="/home/zgh23/code/ctf/D-$Tname-$J.mtx"
Cname="/home/zgh23/code/ctf/C-$Tname-$J.mtx"
mpirun -n 1 ./bin/mymttkrp -tensor $Bname -dims $K,$L,$I -iter 1 -warmup 1 -mttkrpLDim $J -mode 0 -matrixA $Aname -matrixD $Dname -matrixC $Cname
# TODO: Add mtx formating
# Replace .mtx with .txt in Aname
Atxt="${Aname%.*}.txt"
# Replace .mtx with .txt in Dname
Dtxt="${Dname%.*}.txt"
# Replace .mtx with .txt in Cname
Ctxt="${Cname%.*}.txt"

cp $Cname $Ctxt
line_count=$(wc -l < $Cname)
echo $line_count
sed -i "1i $K $J $line_count" $Cname
sed -i '1i %%MatrixMarket matrix coordinate real general' $Cname
sed -n '1,4p' $Cname

cp $Dname $Dtxt
line_count=$(wc -l < $Dname)
echo $line_count
sed -i "1i $L $J $line_count" $Dname
sed -i '1i %%MatrixMarket matrix coordinate real general' $Dname
sed -n '1,4p' $Dname

cp $Aname $Atxt
line_count=$(wc -l < $Aname)
echo $line_count
sed -i "1i $I $J $line_count" $Aname
sed -i '1i %%MatrixMarket matrix coordinate real general' $Aname
sed -n '1,4p' $Aname