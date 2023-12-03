cmake_minimum_required(VERSION 3.9)

project(FallingBalls)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lmingw32")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

add_executable(FallingBalls main.cpp SDL_Plotter.h SDL_Plotter.cpp polygon.h polygon.cpp Object.h Object.cpp ball.h ball.cpp force.h force.cpp vect.h vect.cpp draw.h draw.cpp obstacle.h obstacle.cpp)

##

target_include_directories(FallingBalls PRIVATE ../SDL2-2.28.5/x86_64-w64-mingw32/include)
target_include_directories(FallingBalls PRIVATE ../SDL2_mixer-2.6.3/x86_64-w64-mingw32/include)

target_link_directories(FallingBalls PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../SDL2_mixer-2.6.3/x86_64-w64-mingw32/lib)
target_link_directories(FallingBalls PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../SDL2-2.28.5/x86_64-w64-mingw32/lib)
target_link_directories(FallingBalls PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../SDL2_ttf-2.20.2/x86_64-w64-mingw32/lib)

INCLUDE_DIRECTORIES(../SDL2-2.28.5/x86_64-w64-mingw32/include)
INCLUDE_DIRECTORIES(../SDL2-2.28.5/x86_64-w64-mingw32/include/SDL2)
INCLUDE_DIRECTORIES(../SDL2_ttf-2.20.2/x86_64-w64-mingw32/include)
INCLUDE_DIRECTORIES(../SDL2_ttf-2.20.2/x86_64-w64-mingw32/include/SDL2)

target_link_libraries(FallingBalls SDL2main SDL2 SDL2_mixer SDL2_ttf)