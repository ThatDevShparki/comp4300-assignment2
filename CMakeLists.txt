cmake_minimum_required(VERSION 3.27)

project(comp4300_assignment2)

set(CMAKE_CXX_STANDARD 14)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

include_directories(.)

add_executable(comp4300_assignment2 main.cpp
        EntityManager.cpp
        EntityManager.h
        Entity.cpp
        Entity.h
        Game.cpp
        Game.h
        Vec2.cpp
        Vec2.h
        Components.h)
target_link_libraries(comp4300_assignment2 PRIVATE sfml-graphics)
target_compile_features(comp4300_assignment2 PRIVATE cxx_std_17)

install(TARGETS comp4300_assignment2)
