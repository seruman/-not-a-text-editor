cmake_minimum_required(VERSION 3.6)
project(bahadir_prolab)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

include_directories(gapbuffer)

set(SOURCE_FILES main.c backbuffer.c backbuffer.h gapbuffer/src/lib/gap_buffer.c tui.c tui.h)
add_executable(bahadir_prolab ${SOURCE_FILES})
target_link_libraries(bahadir_prolab termbox)
