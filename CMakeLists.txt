cmake_minimum_required(VERSION 2.8)
project(merkletree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

set(SRC "src")

set( project_sources
   main.cpp
   src/merkletree.cpp
   src/merkletreenode.cpp
   src/hash.cpp
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package( CryptoPP REQUIRED )
include_directories(${CRYPTOPP_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${project_sources})

target_link_libraries( ${PROJECT_NAME} ${CRYPTOPP_LIBRARY})

