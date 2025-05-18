#pragma once
#ifndef CUDAFUNCS_HPP
# define CUDAFUNCS_HPP
# include "../Project.hpp"
# include <cuda.h>
# include <builtin_types.h>
# include <cuda_runtime.h> // For CUDA functions
extern "C" __global__ void vectorSum(int *to, int **from, int size);
namespace Wrapper {
	void VSWrapper(int *to, int **from, int size);
}
#endif