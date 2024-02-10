# Specify minimum supported cmake version
cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# set the project name
project(MathematicsForGameProgrammers)

option(MIKROPLOT_BUILD_EXAMPLES "" OFF)
add_subdirectory("ext/mikroplot-main")
add_subdirectory("ext/glm-master")
include_directories(".")

add_executable(lin_ingertation main_lin_integ.cpp math_utils.h)
target_link_libraries(lin_ingertation PUBLIC mikroplot glm)

add_executable(simple_math simple_math.cpp)
target_link_libraries(simple_math mikroplot)

add_executable(exerc1_particles submissions/exerc1_particles.cpp)
target_link_libraries(exerc1_particles PUBLIC mikroplot glm)

add_executable(exerc2_springforce submissions/exerc2_springforce.cpp)
target_link_libraries(exerc2_springforce PUBLIC mikroplot glm)

add_executable(exerc3_rotation submissions/exerc3_rotation.cpp)
target_link_libraries(exerc3_rotation PUBLIC mikroplot glm)

add_executable(exerc4_jumpy_game submissions/exerc4_jumpy_game/jumpy_main.cpp)
target_link_libraries(exerc4_jumpy_game PUBLIC mikroplot glm)

add_executable(particles main_particle.cpp)
target_link_libraries(particles PUBLIC mikroplot glm)

add_executable(AxisAlignedBoundingBox main_aabb.cpp)
target_link_libraries(AxisAlignedBoundingBox PUBLIC mikroplot glm)