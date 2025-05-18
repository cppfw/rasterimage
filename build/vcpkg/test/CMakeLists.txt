cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

project(test)

find_package(utki CONFIG REQUIRED)
find_package(papki CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(r4 CONFIG REQUIRED)
find_package(rasterimage CONFIG REQUIRED)

add_executable(test main.cpp)

target_link_libraries(test PRIVATE rasterimage::rasterimage)
