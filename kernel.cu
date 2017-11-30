#include <stdio.h>
#include "cuda.h"
#include <cuda_runtime.h>

//checking error di cuda
void checkCUDAError(const char* msg) {
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err) {
		fprintf(stderr, "cuda error: %s: %s.\n", msg, cudaGetErrorString(err));
		exit(EXIT_FAILURE);
	}
}

//fungsi kernel untuk dieksekusi di GPU
__global__ void kernel(uchar4* pos, unsigned int w, unsigned int h, float time) {
	int idx = blockIdx.x*blockDim.x + threadIdx.x; //pembagian array berdasarkan index array dan index threadnya
	unsigned int x = idx%w, y = idx%w;
	if (idx < w*h) {
		unsigned char r = (x + (int)time) & 0xff;
		unsigned char g = (y + (int)time) & 0xff;
		unsigned char b = (x + y + (int)time) & 0xff;

		//thread menulis lokasi piksel di texture
		pos[idx].w = 0;
		pos[idx].x = r;
		pos[idx].y = g;
		pos[idx].z = b;
	}
}

//wrapper for global call
extern "C" void launch_kernel(uchar4* pos, unsigned int iw, unsigned int ih, float time) {
	//execute kernel
	int nThreads = 256;
	int total = iw*ih; //jumlah data yang akan diproses oleh GPU
	int nBlocks = total / nThreads; //jumlah block yang akan digunakan
	nBlocks += (total%nThreads > 0 ? 0 : 1);

	kernel << <nBlocks, nThreads >> > (pos, iw, ih, time); //pemanggilan fungsi kernel
	cudaThreadSynchronize(); //penyelarasan thread untuk memastikan thread sudah selesai semua
	checkCUDAError("kernel failed");
}