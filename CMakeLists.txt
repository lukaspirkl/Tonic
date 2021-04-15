cmake_minimum_required(VERSION 3.13)
project(Tonic LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
    ${PROJECT_SOURCE_DIR}/src/Tonic/*.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})

if(EMSCRIPTEN)
    message("EMSCRIPTEN BUILD")
    SET(CMAKE_EXECUTABLE_SUFFIX ".html")
    string(APPEND CMAKE_CXX_FLAGS " -s DEMANGLE_SUPPORT=1")
    string(APPEND CMAKE_CXX_FLAGS " -s DISABLE_EXCEPTION_CATCHING=2")
    string(APPEND CMAKE_CXX_FLAGS " -s USE_GLFW=3")
    string(APPEND CMAKE_CXX_FLAGS " -s USE_WEBGL2=1")
    string(APPEND CMAKE_CXX_FLAGS " -s WASM=1")
    string(APPEND CMAKE_CXX_FLAGS " -s ASSERTIONS=1")
    string(APPEND CMAKE_CXX_FLAGS " -s USE_PTHREADS=1")
    
    # this can be removed when cpp is used
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
endif()
