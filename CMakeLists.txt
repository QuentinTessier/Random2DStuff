cmake_minimum_required(VERSION 3.10)
project(2DPathtracer)

add_executable(2DPathtracer
    src/main.c
    # src/quadtree.c
    src/array/array.c
    src/perlin.c
    src/Mesh.c
    src/tri_tri_intersect.c
)

target_link_libraries(2DPathtracer PRIVATE m csfml-system csfml-window csfml-graphics)
target_compile_options(2DPathtracer PRIVATE -g)