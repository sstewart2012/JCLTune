__kernel void hello(const int n, __global const int * const A, __global int * const B) {
    const int tid = get_global_id(0);
    if (tid < n) {
        B[tid] = A[tid];
    }
}