cmake_minimum_required(VERSION 3.10)
project(pim-compiler)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Include project headers
include_directories(include)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(test)
