// vector addition

#include <hip/hip_runtime.h>
#include <iostream>
#include <vector>

__global__ void vectorAdd(const float* A, const float* B, float* C, int N){

	int i = blockIdx.x * blockDim.x + threadIdx.x;

	if(i < N){
		C[i] = A[i] + B[i];
	}

}

int main(){

    int N = 1000000;
    size_t bytes = N * sizeof(float);

    std::vector<float> h_A(N, 1.0f);
    std::vector<float> h_B(N, 2.0f);
    std::vector<float> h_C(N, 0.0f);

    float *d_A, *d_B, *d_C;
    hipMalloc(&d_A, bytes);
    hipMalloc(&d_B, bytes);
    hipMalloc(&d_C, bytes);

    hipMemcpy(d_A, h_A.data(), bytes, hipMemcpyHostToDevice);
    hipMemcpy(d_B, h_B.data(), bytes, hipMemcpyHostToDevice);

    int threadsPerBlock = 256;
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;
    vectorAdd<<<blocksPerGrid, threadsPerBlock>>>(d_A, d_B, d_C, N);

    hipMemcpy(h_C.data(), d_C, bytes, hipMemcpyDeviceToHost);

    std::cout << "Result[0]: " << h_C[0] << " (Expected: 3)\n";
    std::cout << "Result[999999]: " << h_C[N - 1] << " (Expected: 3)\n";

    hipFree(d_A);
    hipFree(d_B);
    hipFree(d_C);

    return 0;
	
}
