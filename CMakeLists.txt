# base stuff
cmake_minimum_required(VERSION 3.10)

project(MyExecutableProject LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED TRUE) 

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED TRUE) 

include(FetchContent)

# logger dep
FetchContent_Declare(
    Logger
    GIT_REPOSITORY https://github.com/benzhenwen/Logger.git
    GIT_TAG main
)

FetchContent_MakeAvailable(Logger)


# raylib dep
set(RAYLIB_VERSION 5.5)  # latest stable as of now :contentReference[oaicite:0]{index=0}
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)


# exe
add_executable(particles 
        main.cpp
        particles/simulation.c
) 


# link deps
target_link_libraries(
    particles 
    PRIVATE 
        Logger::Logger
        raylib
)