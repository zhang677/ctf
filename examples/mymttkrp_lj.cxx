#include <ctf.hpp>
#include <chrono>
#include <float.h>

using namespace CTF;

double benchmarkWithWarmup(int64_t warmup, int64_t numIter, std::function<void(void)> f) {
  for (int64_t i = 0; i < warmup; i++) { f(); }
  auto start = std::chrono::high_resolution_clock::now();
  for (int64_t i = 0; i < numIter; i++) { f(); }
  auto end = std::chrono::high_resolution_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  return double(ms) / double(numIter);
}

std::vector<std::string> split(const std::string &str, const std::string &delim, bool keepDelim) {
  std::vector<std::string> results;
  size_t prev = 0;
  size_t next = 0;

  while ((next = str.find(delim, prev)) != std::string::npos) {
    if (next - prev != 0) {
      std::string substr = ((keepDelim) ? delim : "")
                         + str.substr(prev, next-prev);
      results.push_back(substr);
    }
    prev = next + delim.size();
  }

  if (prev < str.size()) {
    string substr = ((keepDelim) ? delim : "") + str.substr(prev);
    results.push_back(substr);
  }

  return results;
}


void mttkrp_fail(int64_t nIter, int64_t warmup, std::vector<int64_t> dims, World& dw, int64_t ldim) {
  double sp_frac = 0.1;
  Tensor<double> B(3, true /* is_sparse */, dims.data(), dw);
  // These look backwards because the MTTKRP built-in routine operates on
  // transposed matrices.
  int64_t dimA[2] = {ldim, dims[0]};
  int64_t dimC[2] = {ldim, dims[1]};
  int64_t dimD[2] = {ldim, dims[2]};
  Tensor<double> A(2, true, dimA, dw);
  Tensor<double> C(2, true, dimC, dw);
  Tensor<double> D(2, true, dimD, dw);
  A.fill_sp_random(-1.,1.,sp_frac);
  B.fill_sp_random(-1.,1.,sp_frac);
  C.fill_sp_random(-1.,1.,sp_frac);
  D.fill_sp_random(-1.,1.,sp_frac);
  
  Tensor<double>* mats[] = {&A, &C, &D};
  auto avgMs = benchmarkWithWarmup(warmup, nIter, [&]() {
    MTTKRP(&B, mats, 0, true);
  });

  if (dw.rank == 0) {
    std::cout << "Average execution time: " << avgMs << " ms." << std::endl;
  }
}

void mttkrp_gen(int64_t nIter, int64_t warmup, std::vector<int64_t> dims, World& dw, int64_t ldim, std::string filename1, std::string filename2, std::string filename3, std::string filename4) {
  double sp_frac = 0.1;
  // These look backwards because the MTTKRP built-in routine operates on
  // transposed matrices.
  // i: dim[0]; k: dim[1]; l: dim[2]; j: ldim
  int64_t K = dims[0], L = dims[1], I = dims[2], J = ldim;
  int64_t dimA[2] = {I, J}; // 28818,32
  int64_t dimB[3] = {K, L, I}; // 12092,9184,28818
  int64_t dimC[2] = {K, J}; // 12092,32
  int64_t dimD[2] = {L, J}; // 9184,32
  Tensor<double> B(3, true /* is_sparse */, dimB, dw, Ring<double>(), "B");
  Tensor<double> A(2, true, dimA, dw, Ring<double>(), "A");
  Tensor<double> C(2, true, dimC, dw, Ring<double>(), "C");
  Tensor<double> D(2, true, dimD, dw, Ring<double>(), "D");
  // A.fill_sp_random(-1.,1.,sp_frac);
  std::cout << "Start generating B..." << std::endl;
  // B.fill_sp_random(-1.,1.,sp_frac);
  B.read_sparse_from_file(filename1.c_str());
  std::cout << "Finish generating B..." << std::endl;
  C.fill_sp_random(-1.,1.,sp_frac);
  D.fill_sp_random(-1.,1.,sp_frac);

  auto avgMs = benchmarkWithWarmup(warmup, nIter, [&]() {
    // A["ji"] = B["ikl"] * C["jk"] * D["jl"];
    std::cout << "Computing..." << std::endl;
    A["ij"] = B["kli"] * C["kj"] * D["lj"];
    std::cout << "Finish Computing" << std::endl;
  });
  std::cout << "A: (" << dimA[0] << "," << dimA[1] << "): " << A.nnz_tot << std::endl;
  std::cout << "B: (" << dimB[0] << "," << dimB[1] << "," << dimB[2] << "): " << B.nnz_tot << std::endl;
  std::cout << "C: (" << dimC[0] << "," << dimC[1] << "): " << C.nnz_tot<< std::endl;
  std::cout << "D: (" << dimD[0] << "," << dimD[1] << "): " << D.nnz_tot << std::endl;
  C.write_sparse_to_file(filename2.c_str()); // "/home/zgh23/code/ctf/C.mtx"
  D.write_sparse_to_file(filename3.c_str()); // "/home/zgh23/code/ctf/D.mtx"
  A.write_sparse_to_file(filename4.c_str()); // "/home/zgh23/code/ctf/A.mtx"
  if (dw.rank == 0) {
    std::cout << "Average execution time: " << avgMs << " ms." << std::endl;
  }
}

void mttkrp_check(int64_t nIter, int64_t warmup, std::vector<int64_t> dims, World& dw, int64_t ldim, std::string filename1, std::string filename2, std::string filename3, std::string filename4) {
  // These look backwards because the MTTKRP built-in routine operates on
  // transposed matrices.
  // i: dim[0]; k: dim[1]; l: dim[2]; j: ldim
  int64_t K = dims[0], L = dims[1], I = dims[2], J = ldim;
  int64_t dimA[2] = {I, J}; // 28818,32
  int64_t dimB[3] = {K, L, I}; // 12092,9184,28818
  int64_t dimC[2] = {K, J}; // 12092,32
  int64_t dimD[2] = {L, J}; // 9184,32
  Tensor<double> B(3, true /* is_sparse */, dimB, dw, Ring<double>(), "B");
  Tensor<double> A(2, true, dimA, dw, Ring<double>(), "A");
  Tensor<double> C(2, true, dimC, dw, Ring<double>(), "C");
  Tensor<double> D(2, true, dimD, dw, Ring<double>(), "D");
  // A.fill_sp_random(-1.,1.,sp_frac);
  // B.fill_sp_random(-1.,1.,sp_frac);
  B.read_sparse_from_file(filename1.c_str());
  std::cout << "Reading C and D from file..." << std::endl;
  C.read_sparse_from_file(filename2.c_str()); // "/home/zgh23/code/ctf/C.txt"
  D.read_sparse_from_file(filename3.c_str()); // "/home/zgh23/code/ctf/D.txt"

  auto avgMs = benchmarkWithWarmup(warmup, nIter, [&]() {
    // A["ji"] = B["ikl"] * C["jk"] * D["jl"];
    A["ij"] = B["kli"] * C["kj"] * D["lj"];
  });
  std::cout << "A: (" << dimA[0] << "," << dimA[1] << "): " << A.nnz_tot << std::endl;
  std::cout << "B: (" << dimB[0] << "," << dimB[1] << "," << dimB[2] << "): " << B.nnz_tot << std::endl;
  std::cout << "C: (" << dimC[0] << "," << dimC[1] << "): " << C.nnz_tot<< std::endl;
  std::cout << "D: (" << dimD[0] << "," << dimD[1] << "): " << D.nnz_tot << std::endl;
  A.write_sparse_to_file(filename4.c_str()); // "/home/zgh23/code/ctf/A.mtx"
  if (dw.rank == 0) {
    std::cout << "Average execution time: " << avgMs << " ms." << std::endl;
  }
}

void mttkrp_bench(int64_t nIter, int64_t warmup, std::vector<int64_t> dims, World& dw, int64_t ldim, std::string filename1, std::string filename2, std::string filename3, std::string resultname) {
  // These look backwards because the MTTKRP built-in routine operates on
  // transposed matrices.
  // i: dim[0]; k: dim[1]; l: dim[2]; j: ldim
  int64_t K = dims[0], L = dims[1], I = dims[2], J = ldim;
  int64_t dimA[2] = {I, J}; // 28818,32
  int64_t dimB[3] = {K, L, I}; // 12092,9184,28818
  int64_t dimC[2] = {K, J}; // 12092,32
  int64_t dimD[2] = {L, J}; // 9184,32
  Tensor<double> B(3, true /* is_sparse */, dimB, dw, Ring<double>(), "B");
  Tensor<double> A(2, true, dimA, dw, Ring<double>(), "A");
  Tensor<double> C(2, true, dimC, dw, Ring<double>(), "C");
  Tensor<double> D(2, true, dimD, dw, Ring<double>(), "D");
  // A.fill_sp_random(-1.,1.,sp_frac);
  // B.fill_sp_random(-1.,1.,sp_frac);
  std::cout << "Reading B from file..." << std::endl;
  B.read_sparse_from_file(filename1.c_str());
  std::cout << "Reading C and D from file..." << std::endl;
  C.read_sparse_from_file(filename2.c_str()); // "/home/zgh23/code/ctf/C.txt"
  D.read_sparse_from_file(filename3.c_str()); // "/home/zgh23/code/ctf/D.txt"

  auto avgMs = benchmarkWithWarmup(warmup, nIter, [&]() {
    // A["ji"] = B["ikl"] * C["jk"] * D["jl"];
    A["ij"] = B["kli"] * C["kj"] * D["lj"];
  });
  auto nameStr = split(filename1, "/", false /* keepDelim */);
  if (dw.rank == 0) {
    std::cout << nameStr[nameStr.size()-1] << "," << ldim << "," << "Average execution time: " << avgMs << " ms." << std::endl;
    std::ofstream outfile;
    outfile.open(resultname, std::ios_base::app);
    outfile << nameStr[nameStr.size()-1] << "," << ldim << "," << avgMs << std::endl;
  }
}

int main(int argc, char** argv) {
  int64_t nIter = 20, warmup = 5, mttkrpLDim = 32, mode = 0;
  std::string tensorDims = "100,101,102", filename1, filename2, filename3, filename4, resultname = "/home/zgh23/code/ctf/mttkrp.csv";
  for (int64_t i = 1; i < argc; i++) {
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
    INT_ARG("-iter", nIter);
    INT_ARG("-warmup", warmup);
    INT_ARG("-mttkrpLDim", mttkrpLDim);
    STRING_ARG("-dims", tensorDims);
    STRING_ARG("-tensor", filename1);
    STRING_ARG("-matrixC", filename2);
    STRING_ARG("-matrixD", filename3);
    STRING_ARG("-matrixA", filename4);
    STRING_ARG("-result", resultname);
    INT_ARG("-mode", mode);
#undef INT_ARG
#undef STRING_ARG
  }
  srand(42);
  auto dimsStr = split(tensorDims, ",", false /* keepDelim */);
  std::vector<int64_t> dims;
  for (auto it : dimsStr) {
    dims.push_back((int64_t)atoi(it.c_str()));
  }
  MPI_Init(&argc, &argv);
  int np;
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  World dw;
  int retVal = 0;
  if (mode == 0) {
    mttkrp_gen(nIter, warmup, dims, dw, mttkrpLDim, filename1, filename2, filename3, filename4);
  } else if (mode == 1) {
    mttkrp_check(nIter, warmup, dims, dw, mttkrpLDim, filename1, filename2, filename3, filename4);
  } else {
    mttkrp_bench(nIter, warmup, dims, dw, mttkrpLDim, filename1, filename2, filename3, resultname);
  }
  MPI_Finalize();
  return retVal;
}