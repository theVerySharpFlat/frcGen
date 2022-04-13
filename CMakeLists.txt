project(frcCommandGenerator)

cmake_minimum_required(VERSION 3.21)

add_executable(frcGen src/main.cpp)

target_compile_features(frcGen PUBLIC cxx_std_17)

add_subdirectory(vendor/incbin)
add_subdirectory(vendor/cxxopts)
add_subdirectory(vendor/filesystem)
target_link_libraries(frcGen PUBLIC incbin cxxopts ghc_filesystem)
target_include_directories(frcGen PUBLIC vendor/rapidjson/include)
target_precompile_headers(frcGen PUBLIC src/pch.h)
