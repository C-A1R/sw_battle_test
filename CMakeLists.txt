cmake_minimum_required(VERSION 3.27)
project(sw_battle_test)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES 
    src/*.cpp 
    src/*.hpp
    src/Game/*.cpp
    src/Game/BattleActions/*.cpp
    src/Game/BattleCore/*.cpp
    src/Game/Units/*.cpp
    src/Game/Units/Actions/*.cpp
)
add_executable(sw_battle_test ${SOURCES})

target_include_directories(sw_battle_test PUBLIC src/)
