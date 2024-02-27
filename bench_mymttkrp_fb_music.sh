#!/bin/bash
# [23343790, 423344784, 166]
Tname=freebase_music
K=23343790
L=23344784
I=166
it=20
wp=5
Bname="/scratch/zgh23/sparse_ten/$Tname-zero.tns"
Rname="/home/zgh23/code/ctf/mttkrp-freebase_music.csv"
Js=(4 8 16 32 64 128) # 256 512 1024 2048)
for J in ${Js[@]}; do
    Dname="/scratch/zgh23/sparse_ten/spws_generated/D-freebase_music-4.txt"
    Cname="/scratch/zgh23/sparse_ten/spws_generated/C-freebase_music-4.txt"
    mpirun -n 1 ./bin/mymttkrp -tensor $Bname -dims $K,$L,$I -iter $it -warmup $wp -mttkrpLDim $J -mode 2 -matrixD $Dname -matrixC $Cname -result $Rname
done