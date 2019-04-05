# Basic Ray Tracing

Basic ray tracing using pure C++ (no illumination)

It is an algorithm of basic raytracing. From a point, you throw rays to every pixel of the image to generate. These rays
can collide with triangles or spheres. An sphere can be transparent or translucid (which have an refraction index). 

No illumination have been applied. Only ray 'physics' (Snell's Law)

Code parallelized using MPI
