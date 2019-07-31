cmake_minimum_required(VERSION 2.8)

project(parallelFuncSum)

find_package (Threads)

set(PARALLEL_FUNC_SUM_LIB "parallelFuncSum.h")
add_compile_options(-pthread)

add_library(parallelFuncSumLib STATIC ${PARALLEL_FUNC_SUM_LIB})
set_target_properties(parallelFuncSumLib PROPERTIES LINKER_LANGUAGE CXX)

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} parallelFuncSumLib ${CMAKE_THREAD_LIBS_INIT})
