cmake_minimum_required(VERSION "3.25.1")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(OpenGL_GL_PREFERENCE GLVND)
project(PhysicsGame)


set(source_dir "${PROJECT_SOURCE_DIR}/src")
set(stbimage_dir "${PROJECT_SOURCE_DIR}/lib/stbimage")

#Actor source
file(GLOB_RECURSE actor_s "${source_dir}/Engine/Actors/*.cpp")
file(GLOB_RECURSE actor_h "${source_dir}/Engine/Actors/*.h")
file(GLOB_RECURSE actor_comp_s "${source_dir}/Engine/Actors/Components/*.cpp")
file(GLOB_RECURSE actor_comp_h "${source_dir}/Engine/Actors/Components/*.h")

#physics source
file(GLOB_RECURSE physics_s "${source_dir}/Engine/Physics/*.cpp")
file(GLOB_RECURSE physics_h "${source_dir}/Engine/Physics/*.h")

#renderer source
file(GLOB_RECURSE render_s "${source_dir}/Engine/Render/*.cpp")
file(GLOB_RECURSE render_h "${source_dir}/Engine/Render/*.h")

#engine source
file(GLOB_RECURSE engine_s "${source_dir}/Engine/*.cpp")
file(GLOB_RECURSE engine_h "${source_dir}/Engine/*.h")

file(GLOB_RECURSE main_s ${source_dir}/*.cpp)
file(GLOB_RECURSE main_h ${source_dir}/*.h)
file(GLOB_RECURSE stbimage_source_files "${stbimage_dir}/*.cpp") #only one file

add_executable(${PROJECT_NAME} ${main_s} ${main_h} ${stbimage_source_files})
target_include_directories(${PROJECT_NAME} PUBLIC "${source_dir}/Engine")


#libraries
add_subdirectory("lib/assimp")
target_include_directories(assimp PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib/assimp/include>$<INSTALL_INTERFACE:lib/assimp/include>")
target_link_libraries(${PROJECT_NAME} PUBLIC assimp)

add_subdirectory("lib/GLEW/build/cmake")
target_include_directories(glew PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib/GLEW/include>$<INSTALL_INTERFACE:lib/GLEW/include>")
target_link_libraries(${PROJECT_NAME} PUBLIC glew)

add_subdirectory("lib/GLFW")
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_include_directories(glfw PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib/GLFW/include>$<INSTALL_INTERFACE:lib/GLFW/include>")

add_subdirectory("lib/glm")
target_link_libraries(${PROJECT_NAME} PUBLIC glm)
target_include_directories(glm INTERFACE "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/lib/glm>$<INSTALL_INTERFACE:lib/glm/glm>")

#header only
target_include_directories(${PROJECT_NAME} PUBLIC "${stbimage_dir}/include")



