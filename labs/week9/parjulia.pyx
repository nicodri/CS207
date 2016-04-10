# distutils: extra_compile_args = -fopenmp
# distutils: extra_link_args = -fopenmp
#your code here

cimport cython
import numpy as np
cimport numpy as np
from cython.parallel cimport prange

cdef inline double norm2(double complex z) nogil:
    return z.real*z.real + z.imag*z.imag

cdef int escape(int maxiter, double complex z, double complex c) nogil:
    cdef int n=0
    while n < maxiter and norm2(z) < 4:
        z=z*z+c
        n+=1
    return n

@cython.boundscheck(False)
@cython.wraparound(False)
cpdef run_cython_amalgamated2(int gridsize, box, double complex c, int maxiter):
    #your code here
    cdef:
        int i,j
        double xstep = (box.x2 - box.x1)/(gridsize - 1.0)
        double ystep = (box.y2 - box.y1)/(gridsize - 1.0)
        double x,y
        double x_start = box.x1
        double y_start = box.y1
        double complex [:] output = np.empty(gridsize*gridsize, dtype=np.complex)
        double complex z
    for i in prange(gridsize, nogil=True, schedule='static', chunksize=1):
        x = x_start + i*xstep
        for j in range(gridsize):
            y = y_start + i*ystep
            z.imag = y
            z.real = x
            output[i+j*gridsize] = escape(maxiter, z, c)