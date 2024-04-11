cmake_minimum_required(VERSION 3.0)
project(mjyrpc)

enable_language(ASM)
#生成debug版本，进行调试
set(CMAKE_BUILD_TYPE "Debug")

#设置项目可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#设置项目库文件输出的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/src)
#设置项目库文件搜索路径 -L
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(src)
add_subdirectory(test_case)
