project(SnakeClient_Cpp)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_executable("snake_client" "src/main.cpp" "src/Message.cpp" "src/Math.cpp" "src/World.cpp" "src/Decision.cpp")
configure_file(src/config config)
