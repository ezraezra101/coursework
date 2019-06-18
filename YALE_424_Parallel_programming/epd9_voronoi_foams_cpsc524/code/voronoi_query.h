#ifndef __VORONOI_QUERY_H__
#define __VORONOI_QUERY_H__

#define FP float
#define VOX int //Single voxel in the foam. Can be boolean.

#define CELL_SIZE 1.5 // Coarse cell size
#define BEAM_RAD 0.1 // Beam radius


#define PRINT_POINT(q) { point p=(q); printf("(%f, %f, %f)\n", p.x,p.y,p.z); }
#define PRINT_LABELED(str, q) { point p=(q); printf("%s: (%f, %f, %f)\n", str, p.x,p.y,p.z); }

// If faking nvcc:
#ifndef __CUDACC__ //__NVCC__ //__CUDA_API_VERSION
#warning "Faking CUDA keywords"
#define __host__
#define __device__
#endif



struct point {
	FP x,y,z;

	// Idiot constructors... this is c++... sigh.

__host__ __device__
	point(FP x, FP y, FP z) : x(x), y(y), z(z) {};

__host__ __device__
	point() {};
};


// Density function. 
__host__ __device__
extern FP rho(point q);

__host__ __device__
VOX voronoi_query(point q);

#endif