#include <ctf.hpp>
#include <chrono>
#include <float.h>
#include <functional>

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

void mttkrp(int nIter, int warmup, std::string filename, std::vector<int> dims, World& dw, int ldim) {
  Tensor<double> B(3, true /* is_sparse */, dims.data(), dw);
  // These look backwards because the MTTKRP built-in routine operates on
  // transposed matrices.
  Matrix<double> A(ldim, dims[0], dw);
  Matrix<double> C(ldim, dims[1], dw);
  Matrix<double> D(ldim, dims[2], dw);
  C.fill_random(1.0, 1.0);
  D.fill_random(1.0, 1.0);
  // T.fill_sp_random(-1.,1.,sp_frac);

  B.read_sparse_from_file(filename.c_str());
  if (dw.rank == 0) {
    std::cout << "Read " << B.nnz_tot << " non-zero entries from the file." << std::endl;
  }
  
  Tensor<double>* mats[] = {&A, &C, &D};
  auto avgMs = benchmarkWithWarmup(warmup, nIter, [&]() {
    MTTKRP(&B, mats, 0, true);
  });

  if (dw.rank == 0) {
    std::cout << "Average execution time: " << avgMs << " ms." << std::endl;
  }
}



int main(int argc, char** argv) {
  int nIter = 20, warmup = 10, mttkrpLDim = 32, spmmJDim = 32, sddmmJDim = 32;
  std::string filename, bench = "spmv", tensorDims, spmspvVecFile, tensorC, tensorD;
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
    INT_ARG("-n", nIter);
    INT_ARG("-warmup", warmup);
    STRING_ARG("-tensor", filename);
    STRING_ARG("-bench", bench);
    STRING_ARG("-dims", tensorDims);
    STRING_ARG("-spmspvVec", spmspvVecFile);
    STRING_ARG("-tensorC", tensorC);
    STRING_ARG("-tensorD", tensorD);
    INT_ARG("-mttkrpLDim", mttkrpLDim);
    INT_ARG("-spmmJDim", spmmJDim);
    INT_ARG("-sddmmJDim", sddmmJDim);
#undef INT_ARG
#undef STRING_ARG
  }

  if (filename.empty()) {
    std::cout << "Please provide an input filename." << std::endl;
    return -1;
  }

  if (tensorDims.empty()) {
    std::cout << "Must provide tensor dims." << std::endl;
    return -1;
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
  if (bench == "spttm") {
    
  } else if (bench == "spmttkrp") {
    mttkrp(nIter, warmup, filename, dims, dw, mttkrpLDim);
  } else {
    std::cout << "Unknown benchmark name: " << bench << std::endl;
    retVal = -1;
  }
  MPI_Finalize();
  return retVal;
}
