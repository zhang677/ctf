#!/bin/bash
Tname=facebook
K=1504
L=42390
I=39986
J=4
Aname="/home/zgh23/code/ctf/A-$Tname-$J.txt"
Dname="/home/zgh23/code/ctf/D-$Tname-$J.txt"
Cname="/home/zgh23/code/ctf/C-$Tname-$J.txt"
Bname="/scratch/zgh23/sparse_ten/$Tname-zero.tns"
mpirun -n 1 ./bin/mymttkrp -tensor $Bname -dims $K,$L,$I -iter 1 -warmup 1 -mttkrpLDim $J -mode 1 -matrixA $Aname -matrixD $Dname -matrixC $Cname

Amtx="${Aname%.*}.mtx"
python /home/zgh23/code/SparseWS/scripts/clean_zeros_ctf.py --input $Aname --output $Amtx
line_count=$(wc -l < $Amtx)
echo $line_count
sed -i "1i $I $J $line_count" $Amtx
sed -i '1i %%MatrixMarket matrix coordinate real general' $Amtx
sed -n '1,4p' $Amtx