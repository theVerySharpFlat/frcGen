project(incbin)

add_library(incbin STATIC incbin.c)
target_include_directories(incbin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
