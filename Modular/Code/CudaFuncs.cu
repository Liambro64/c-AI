//i will write the cuda functions here
#include "../Project.hpp"
//this should set to[i] to the sum of from[i]
//the first integer in from[i] (aka from[i][0])
//is the size of that array.
extern "C" __global__ void vectorSum(int *to, int **from, int size) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    if (idx < size) {
        int *current_from_array = from[idx];
        
        // Ensure the pointer is not null before dereferencing
        if (current_from_array != nullptr) {
            int num_elements_to_sum = current_from_array[0];
            int current_sum = 0;
            
            // Sum elements from current_from_array[1] to current_from_array[num_elements_to_sum]
            for (int j = 1; j <= num_elements_to_sum; j++) {
                current_sum += current_from_array[j];
            }
            to[idx] = current_sum;
        }
    }
}

namespace Wrapper{

void VSWrapper(int *to, int **from, int size) {
    // Define the number of threads and blocks
    int threadsPerBlock = 256;
    int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;

    // Launch the kernel
    vectorSum<<<blocksPerGrid, threadsPerBlock>>>(to, from, size);
    // Check for errors in kernel launch (optional)
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        fprintf(stderr, "CUDA error: %s\n", cudaGetErrorString(err));
    }
	err = cudaDeviceSynchronize(); // Wait for kernel to complete
    if (err != cudaSuccess) {
        fprintf(stderr, "CUDA error: %s\n", cudaGetErrorString(err));
    }
}
}