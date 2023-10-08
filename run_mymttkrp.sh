mpirun -n 1 ./bin/mymttkrp -tensor /scratch/zgh23/sparse_ten/nell-2.tns -dims 12092,9184,28818 -iter 2 -warmup 1 -mttkrpLDim 32
# TODO: Add mtx formating
# line_count=$(wc -l < file.mtx)
# sed -i '1i 9184 32 $line_count' D.mtx
# sed -i '1i %%MatrixMarket matrix coordinate real general' D.mtx