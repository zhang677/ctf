#!/bin/bash
#Js=(16 64 128 256 512 1024 2048)
# Js=(4 8 32)
# K=12092
# L=9184
# for J in ${Js[@]}; do
#     Dname="/home/zgh23/code/ctf/D-nell-2-$J.mtx"
#     Cname="/home/zgh23/code/ctf/C-nell-2-$J.mtx"
#     mpirun -n 1 ./bin/mygenmat -K $K -L $L -J $J -matrixD $Dname -matrixC $Cname
#     Dtxt="${Dname%.*}.txt"
#     Ctxt="${Cname%.*}.txt"
#     cp $Cname $Ctxt
#     line_count=$(wc -l < $Cname)
#     echo $line_count
#     sed -i "1i $K $J $line_count" $Cname
#     sed -i '1i %%MatrixMarket matrix coordinate real general' $Cname
#     cp $Dname $Dtxt
#     line_count=$(wc -l < $Dname)
#     echo $line_count
#     sed -i "1i $L $J $line_count" $Dname
#     sed -i '1i %%MatrixMarket matrix coordinate real general' $Dname
# done

# Bname=nell-1
# Js=(4 8 16 32 64)
# K=2902330
# L=2143368
# for J in ${Js[@]}; do
#     Dname="/home/zgh23/code/ctf/D-$Bname-$J.mtx"
#     Cname="/home/zgh23/code/ctf/C-$Bname-$J.mtx"
#     mpirun -n 1 ./bin/mygenmat -K $K -L $L -J $J -matrixD $Dname -matrixC $Cname
#     Dtxt="${Dname%.*}.txt"
#     Ctxt="${Cname%.*}.txt"
#     cp $Cname $Ctxt
#     line_count=$(wc -l < $Cname)
#     echo $line_count
#     sed -i "1i $K $J $line_count" $Cname
#     sed -i '1i %%MatrixMarket matrix coordinate real general' $Cname
#     cp $Dname $Dtxt
#     line_count=$(wc -l < $Dname)
#     echo $line_count
#     sed -i "1i $L $J $line_count" $Dname
#     sed -i '1i %%MatrixMarket matrix coordinate real general' $Dname
# done

# Bname=delicious-3d
# Js=(4 8 16 32 64)
# K=532924
# L=17262471
# for J in ${Js[@]}; do
#     Dname="/home/zgh23/code/ctf/D-$Bname-$J.mtx"
#     Cname="/home/zgh23/code/ctf/C-$Bname-$J.mtx"
#     mpirun -n 1 ./bin/mygenmat -K $K -L $L -J $J -matrixD $Dname -matrixC $Cname
#     Dtxt="${Dname%.*}.txt"
#     Ctxt="${Cname%.*}.txt"
#     cp $Cname $Ctxt
#     line_count=$(wc -l < $Cname)
#     echo $line_count
#     sed -i "1i $K $J $line_count" $Cname
#     sed -i '1i %%MatrixMarket matrix coordinate real general' $Cname
#     cp $Dname $Dtxt
#     line_count=$(wc -l < $Dname)
#     echo $line_count
#     sed -i "1i $L $J $line_count" $Dname
#     sed -i '1i %%MatrixMarket matrix coordinate real general' $Dname
# done

# Bname=freebase_sampled
# Js=(4 8 16 32 64)
# K=38955429
# L=38955429
# I=532
# for J in ${Js[@]}; do
#     Dname="/home/zgh23/code/ctf/D-$Bname-$J.mtx"
#     Cname="/home/zgh23/code/ctf/C-$Bname-$J.mtx"
#     mpirun -n 1 ./bin/mygenmat -K $K -L $L -J $J -matrixD $Dname -matrixC $Cname
#     Dtxt="${Dname%.*}.txt"
#     Ctxt="${Cname%.*}.txt"
#     cp $Cname $Ctxt
#     line_count=$(wc -l < $Cname)
#     echo $line_count
#     sed -i "1i $K $J $line_count" $Cname
#     sed -i '1i %%MatrixMarket matrix coordinate real general' $Cname
#     cp $Dname $Dtxt
#     line_count=$(wc -l < $Dname)
#     echo $line_count
#     sed -i "1i $L $J $line_count" $Dname
#     sed -i '1i %%MatrixMarket matrix coordinate real general' $Dname
# done

# Bname=flickr-3d
# Js=(4 8 16 32 64)
# K=319686
# L=28153045
# for J in ${Js[@]}; do
#     Dname="/home/zgh23/code/ctf/D-$Bname-$J.mtx"
#     Cname="/home/zgh23/code/ctf/C-$Bname-$J.mtx"
#     mpirun -n 1 ./bin/mygenmat -K $K -L $L -J $J -matrixD $Dname -matrixC $Cname
#     Dtxt="${Dname%.*}.txt"
#     Ctxt="${Cname%.*}.txt"
#     cp $Cname $Ctxt
#     line_count=$(wc -l < $Cname)
#     echo $line_count
#     sed -i "1i $K $J $line_count" $Cname
#     sed -i '1i %%MatrixMarket matrix coordinate real general' $Cname
#     cp $Dname $Dtxt
#     line_count=$(wc -l < $Dname)
#     echo $line_count
#     sed -i "1i $L $J $line_count" $Dname
#     sed -i '1i %%MatrixMarket matrix coordinate real general' $Dname
# done

# Bname=freebase_music
# Js=(4 8 16 32 64)
# K=23344784
# L=23344784
# I=166
# for J in ${Js[@]}; do
#     Dname="/home/zgh23/code/ctf/D-$Bname-$J.mtx"
#     Cname="/home/zgh23/code/ctf/C-$Bname-$J.mtx"
#     mpirun -n 1 ./bin/mygenmat -K $K -L $L -J $J -matrixD $Dname -matrixC $Cname
#     Dtxt="${Dname%.*}.txt"
#     Ctxt="${Cname%.*}.txt"
#     cp $Cname $Ctxt
#     line_count=$(wc -l < $Cname)
#     echo $line_count
#     sed -i "1i $K $J $line_count" $Cname
#     sed -i '1i %%MatrixMarket matrix coordinate real general' $Cname
#     cp $Dname $Dtxt
#     line_count=$(wc -l < $Dname)
#     echo $line_count
#     sed -i "1i $L $J $line_count" $Dname
#     sed -i '1i %%MatrixMarket matrix coordinate real general' $Dname
# done

# Bname=patents
# K=46
# L=239172
# I=239172
# Js=(4 8 16 32 64)

# Bname=uber3
# K=182
# L=1139
# I=1716
# Js=(4 8 16 32 64 128 256 512 1024 2048)
# for J in ${Js[@]}; do
#     Dname="/home/zgh23/code/ctf/D-$Bname-$J.mtx"
#     Cname="/home/zgh23/code/ctf/C-$Bname-$J.mtx"
#     mpirun -n 1 ./bin/mygenmat -K $K -L $L -J $J -matrixD $Dname -matrixC $Cname
#     Dtxt="${Dname%.*}.txt"
#     Ctxt="${Cname%.*}.txt"
#     cp $Cname $Ctxt
#     line_count=$(wc -l < $Cname)
#     echo $line_count
#     sed -i "1i $K $J $line_count" $Cname
#     sed -i '1i %%MatrixMarket matrix coordinate real general' $Cname
#     cp $Dname $Dtxt
#     line_count=$(wc -l < $Dname)
#     echo $line_count
#     sed -i "1i $L $J $line_count" $Dname
#     sed -i '1i %%MatrixMarket matrix coordinate real general' $Dname
# done

Bname=facebook
K=1504
L=42390
I=39986
Js=(4 8 16 32 64 128 256 512 1024 2048)
for J in ${Js[@]}; do
    Dname="/home/zgh23/code/ctf/D-$Bname-$J.mtx"
    Cname="/home/zgh23/code/ctf/C-$Bname-$J.mtx"
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