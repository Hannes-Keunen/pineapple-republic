project(shaders)

file(GLOB SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.glsl)
add_spirv_target(${PROJECT_NAME} ${SHADERS})