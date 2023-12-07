cmake_minimum_required(VERSION 3.5)

project(matfHub VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Core)

#CALCULATOR THINGS
# find_package(Armadillo REQUIRED)
# include_directories(${ARMADILLO_INCLUDE_DIRS})
#find_package(Matplot++ REQUIRED)

file(GLOB PROJECT_SOURCES
        "matfHub/src/*.cpp"
        "matfHub/include/*.hpp"
        "matfHub/include/*.h"
	"matfHub/*.ui"
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(matfHub
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        matfHub/include/notes.h
        matfHub/src/notes.cpp
        matfHub/resources.qrc
        matfHub/src/calculator.cpp
        matfHub/include/calculator.h
	matfHub/include/config.hpp
	matfHub/src/config.cpp


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET matfHub APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(matfHub SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(matfHub
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(matfHub PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Core)
# target_link_libraries(matfHub PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
#target_link_libraries(matfHub PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${ARMADILLO_LIBRARIES} PUBLIC Matplot++::matplot)
# target_link_libraries(matfHub PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${ARMADILLO_LIBRARIES})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.matfHub)
endif()
set_target_properties(matfHub PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS matfHub
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(matfHub)
endif()
