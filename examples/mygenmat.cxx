#include <ctf.hpp>
#include <chrono>
#include <float.h>

using namespace CTF;

void gen_mat_mttkrp(int K, int L, int J, World& dw, std::string filename1, std::string filename2) {
    double sp_frac = 0.01;
    int dimC[2] = {K, J};
    int dimD[2] = {L, J};
    Tensor<double> C(2, true, dimC, dw, Ring<double>(), "C");
    Tensor<double> D(2, true, dimD, dw, Ring<double>(), "D");
    C.fill_sp_random(-1.,1.,sp_frac);
    D.fill_sp_random(-1.,1.,sp_frac);
    C.write_sparse_to_file(filename1.c_str()); // "/home/zgh23/code/ctf/C.mtx"
    D.write_sparse_to_file(filename2.c_str()); // "/home/zgh23/code/ctf/D.mtx"
}

void gen_mat_ttm(int K, int J, World& dw, std::string filename1) {
    double sp_frac = 0.01;
    int dimC[2] = {K, J};
    Tensor<double> C(2, true, dimC, dw, Ring<double>(), "C");
    C.fill_sp_random(-1.,1.,sp_frac);
    C.write_sparse_to_file(filename1.c_str()); // "/home/zgh23/code/ctf/C.mtx"
}

int main(int argc, char** argv) {
    std::string filename1 = "/home/zgh23/code/ctf/C-nell-2-32.mtx", filename2 = "/home/zgh23/code/ctf/D-nell-2-32.mtx";
    int K = 12092, L = 9184, J = 32, mode = 0;
    for (int i = 1; i < argc; i++) {
#define INT_ARG(argname, varname) do {      \
          if (!strcmp(argv[i], (argname))) {  \
            varname = atoi(argv[++i]);      \
            continue;                       \
          } } while(0);
#define STRING_ARG(argname, varname) do {      \
          if (!strcmp(argv[i], (argname))) {  \
            varname = std::string(argv[++i]);      \
            continue;                       \
          } } while(0);
    INT_ARG("-K", K);
    INT_ARG("-L", L);
    INT_ARG("-J", J);
    INT_ARG("-mode", mode);
    STRING_ARG("-matrixC", filename1);
    STRING_ARG("-matrixD", filename2);
#undef INT_ARG
#undef STRING_ARG
  }
    srand(42);
    MPI_Init(&argc, &argv);
    int np;
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    World dw;
    int retVal = 0;
    if (mode == 0) {
      gen_mat_mttkrp(K, L, J, dw, filename1, filename2);
    }
    else if (mode == 1) {
      gen_mat_ttm(K, J, dw, filename1);
    }
    
    MPI_Finalize();
    return retVal;
}