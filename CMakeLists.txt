cmake_minimum_required(VERSION 3.16)
project(PSD LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Quick REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)

file(GLOB_RECURSE SOURCES "src/*.cc" "src/*.h" "src/*.qrc")
add_executable(psd ${SOURCES})

target_link_libraries(psd PRIVATE 
    Qt6::Quick
    Eigen3::Eigen
)

target_include_directories(psd PRIVATE 
    src
)