#!/bin/bash
Tname=facebook
Ls=(4) # 8 16 32 64 128 256 512 1024 2048)
K=23343790
I=23344784
J=166
it=1
wp=0
Bname="/scratch/zgh23/sparse_ten/$Tname-zero.tns"
for L in ${Ls[@]}; do
    Cname="/scratch/zgh23/sparse_ten/spws_generated/D-freebase_music-4.txt"
    mpirun -n 1 ./bin/myttm_il -tensor $Bname -dims $K,$I,$J -iter $it -warmup $wp -ttmLDim $L -mode 1 -matrixC $Cname
done