# Minimum required CMake version
cmake_minimum_required(VERSION 3.15)

# Project name and version
project(SSHClient 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for headers (this assumes they're in the src folder)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Use ExternalProject to download and build libssh
include(ExternalProject)

ExternalProject_Add(
    libssh
    GIT_REPOSITORY https://git.libssh.org/apps/libssh.git
    GIT_TAG master
    PREFIX ${CMAKE_BINARY_DIR}/_deps
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/_install
)

# Add your executable (.cpp files in the src folder)
add_executable(SSHClient 
    src/main.cpp
    src/ssh.cpp
    src/mysql.cpp
    src/credentials.cpp
)

# Make sure the executable depends on libssh
add_dependencies(SSHClient libssh)

# Link the libssh library once it's built
target_link_libraries(SSHClient PRIVATE ${CMAKE_BINARY_DIR}/_install/lib/libssh.lib)

# Add include directories for libssh headers
include_directories(${CMAKE_BINARY_DIR}/_install/include)
