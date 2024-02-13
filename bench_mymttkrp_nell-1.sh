Tname=nell-1
J=$1
K=2902330
L=2143368
I=25495389
it=20
wp=5
Bname="/scratch/zgh23/sparse_ten/$Tname-zero.tns"
Dname="/home/zgh23/code/ctf/D-$Tname-$J.txt"
Cname="/home/zgh23/code/ctf/C-$Tname-$J.txt"
mpirun -n 1 ./bin/mymttkrp -tensor $Bname -dims $K,$L,$I -iter $it -warmup $wp -mttkrpLDim $J -mode 2 -matrixD $Dname -matrixC $Cname