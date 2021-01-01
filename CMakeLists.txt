cmake_minimum_required(VERSION 3.10)
project(2DPathtracer)

# add_executable(2DPathtracer
#     src/main.c
#     # src/quadtree.c
#     src/array/array.c
#     src/Mesh.c
#     src/tri_tri_intersect.c
# )

add_executable(MarchingSquare
    src/MarchingSquare/main.c
    src/MarchingSquare/ValueField.c
    src/MarchingSquare/perlin.c
)

# target_link_libraries(2DPathtracer PRIVATE m csfml-system csfml-window csfml-graphics)
# target_compile_options(2DPathtracer PRIVATE -g)


target_link_libraries(MarchingSquare PRIVATE m csfml-system csfml-window csfml-graphics)
target_compile_options(MarchingSquare PRIVATE -g)