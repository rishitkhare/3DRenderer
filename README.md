# 3DRenderer
This project was written as a personal exercise in order to challenge myself to make a complete 3D renderer from the bottom up using no external libraries. Only regular C++ and SDL (Simple DirectMedia Layer) are used. SDL is only used to write pixels to the screen.

For now, all the rendering takes place on the CPU, although I hope to optimize it through GPU usage in the future.

Currently, the renderer supports rendering triangles with vertex colors, affine model transformations, depth buffer calculation, and simple camera movement. When run, it will display a hardcoded [octahedron](https://en.wikipedia.org/wiki/Octahedron) shape that rotates. An example video is shown below:

https://github.com/user-attachments/assets/8cd18a94-f4a0-48ca-972f-19419edb94f1

Below are some features I hope to add in the future:
 - load model from file (.obj)
 - Indexed vertices, i.e. allow different triangles to reference the same vertices
 - Backface culling
 - GPU use and/or parallelization
