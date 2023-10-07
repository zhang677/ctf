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

void ttm(int nIter, int warmup, std::vector<int> dims, World& dw, int ldim, std::string filename1) {
  double sp_frac = 0.1;
  // These look backwards because the MTTKRP built-in routine operates on
  // transposed matrices.
  // i: dim[0]; k: dim[1]; l: dim[2]; j: ldim
  int K = dims[0], I = dims[1], J = dims[2], L = ldim;
  int dimA[3] = {I, J, L};
  int dimB[3] = {K, I, J};
  int dimC[2] = {K, L};
  Tensor<double> B(3, true /* is_sparse */, dimB, dw, Ring<double>(), "B");
  Tensor<double> A(3, true, dimA, dw, Ring<double>(), "A");
  Tensor<double> C(2, true, dimC, dw, Ring<double>(), "C");
  // B.fill_sp_random(-1.,1.,sp_frac);
  B.read_sparse_from_file(filename1.c_str());
  C.fill_sp_random(-1.,1.,sp_frac);

  auto avgMs = benchmarkWithWarmup(warmup, nIter, [&]() {
    A["ijl"] = B["kij"] * C["kl"];
  });

  if (dw.rank == 0) {
    std::cout << "Average execution time: " << avgMs << " ms." << std::endl;
  }
}

int main(int argc, char** argv) {
  int nIter = 20, warmup = 5, ttmLDim = 32;
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
    INT_ARG("-LDim", ttmLDim);
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
  ttm(nIter, warmup, dims, dw, ttmLDim, filename1);
  MPI_Finalize();
  return retVal;
}