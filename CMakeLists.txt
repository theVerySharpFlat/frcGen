project(frcCommandGenerator)

cmake_minimum_required(VERSION 3.21)

add_executable(frcCMDGen src/main.cpp)

target_compile_features(frcCMDGen PUBLIC cxx_std_17)

add_subdirectory(vendor/incbin)
target_link_libraries(frcCMDGen PUBLIC incbin)
