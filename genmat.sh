#!/bin/bash
#Js=(16 64 128 256 512 1024 2048)
Js=(4 8 32)
K=12092
L=9184
for J in ${Js[@]}; do
    Dname="/home/zgh23/code/ctf/D-nell-2-$J.mtx"
    Cname="/home/zgh23/code/ctf/C-nell-2-$J.mtx"
    mpirun -n 1 ./bin/mygenmat -K $K -L $L -J $J -matrixD $Dname -matrixC $Cname
    Dtxt="${Dname%.*}.txt"
    Ctxt="${Cname%.*}.txt"
    cp $Cname $Ctxt
    line_count=$(wc -l < $Cname)
    echo $line_count
    sed -i "1i $K $J $line_count" $Cname
    sed -i '1i %%MatrixMarket matrix coordinate real general' $Cname
    cp $Dname $Dtxt
    line_count=$(wc -l < $Dname)
    echo $line_count
    sed -i "1i $L $J $line_count" $Dname
    sed -i '1i %%MatrixMarket matrix coordinate real general' $Dname
done