#include <ctf.hpp>
#include <chrono>
#include <float.h>

using namespace CTF;

double benchmarkWithWarmup(int warmup, int numIter, std::function<void(void)> f) {
  for (int i = 0; i < warmup; i++) { f(); }
  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < numIter; i++) { f(); }
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


void mttkrp_fail(int nIter, int warmup, std::vector<int> dims, World& dw, int ldim) {
  double sp_frac = 0.1;
  Tensor<double> B(3, true /* is_sparse */, dims.data(), dw);
  // These look backwards because the MTTKRP built-in routine operates on
  // transposed matrices.
  int dimA[2] = {ldim, dims[0]};
  int dimC[2] = {ldim, dims[1]};
  int dimD[2] = {ldim, dims[2]};
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

void mttkrp(int nIter, int warmup, std::vector<int> dims, World& dw, int ldim, std::string filename1) {
  double sp_frac = 0.1;
  // These look backwards because the MTTKRP built-in routine operates on
  // transposed matrices.
  // i: dim[0]; k: dim[1]; l: dim[2]; j: ldim
  int K = dims[0], L = dims[1], I = dims[2], J = ldim;
  int dimA[2] = {I, J};
  int dimB[3] = {K, L, I};
  int dimC[2] = {K, J};
  int dimD[2] = {L, J};
  Tensor<double> B(3, true /* is_sparse */, dimB, dw, Ring<double>(), "B");
  Tensor<double> A(2, true, dimA, dw, Ring<double>(), "A");
  Tensor<double> C(2, true, dimC, dw, Ring<double>(), "C");
  Tensor<double> D(2, true, dimD, dw, Ring<double>(), "D");
  // A.fill_sp_random(-1.,1.,sp_frac);
  // B.fill_sp_random(-1.,1.,sp_frac);
  B.read_sparse_from_file(filename1.c_str());
  C.fill_sp_random(-1.,1.,sp_frac);
  D.fill_sp_random(-1.,1.,sp_frac);

  auto avgMs = benchmarkWithWarmup(warmup, nIter, [&]() {
    // A["ji"] = B["ikl"] * C["jk"] * D["jl"];
    A["ij"] = B["kli"] * C["kj"] * D["lj"];
  });

  if (dw.rank == 0) {
    std::cout << "Average execution time: " << avgMs << " ms." << std::endl;
  }
}

int main(int argc, char** argv) {
  int nIter = 20, warmup = 5, mttkrpLDim = 32;
  std::string tensorDims = "100,101,102", filename1;
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
    INT_ARG("-iter", nIter);
    INT_ARG("-warmup", warmup);
    INT_ARG("-mttkrpLDim", mttkrpLDim);
    STRING_ARG("-dims", tensorDims);
    STRING_ARG("-tensor", filename1);
#undef INT_ARG
#undef STRING_ARG
  }
  auto dimsStr = split(tensorDims, ",", false /* keepDelim */);
  std::vector<int> dims;
  for (auto it : dimsStr) {
    dims.push_back(atoi(it.c_str()));
  }
  MPI_Init(&argc, &argv);
  int np;
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  World dw;
  int retVal = 0;
  mttkrp(nIter, warmup, dims, dw, mttkrpLDim, filename1);
  MPI_Finalize();
  return retVal;
}