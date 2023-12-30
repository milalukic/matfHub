cmake_minimum_required(VERSION 3.22)

project(Test CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)

# add_library(
#     plotter
#     src/plotter.hpp
#     plotter.cpp
# )

set(TEST_SOURCES
	parser.test.cpp
	plotter.test.cpp
	statistics.test.cpp
	fmanager.test.cpp
)

include(FetchContent)
FetchContent_Declare(matplotplusplus
        GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
        GIT_TAG origin/master) # or whatever tag you want

FetchContent_GetProperties(matplotplusplus)
if(NOT matplotplusplus_POPULATED)
    FetchContent_Populate(matplotplusplus)
    add_subdirectory(${matplotplusplus_SOURCE_DIR} ${matplotplusplus_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

file(GLOB CPP_SOURCES "../src/*.cpp")

add_executable(tests ${TEST_SOURCES} ${CPP_SOURCES} ../mainwindow.ui ../catch.hpp)
target_link_libraries(tests PUBLIC matplot)
target_link_libraries(tests PRIVATE Qt6::Widgets Qt6::Core)



