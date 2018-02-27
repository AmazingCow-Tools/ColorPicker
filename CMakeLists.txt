cmake_minimum_required(VERSION 2.8.11)

##----------------------------------------------------------------------------##
## Project                                                                    ##
##----------------------------------------------------------------------------##
project(ColorPicker)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


##----------------------------------------------------------------------------##
## QT                                                                         ##
##----------------------------------------------------------------------------##
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5Widgets)

include_directories(${Qt5Widgets_INCLUDES})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")


##----------------------------------------------------------------------------##
## Amazing Cow Libs                                                           ##
##----------------------------------------------------------------------------##
add_subdirectory(libs/CoreAssert)
add_subdirectory(libs/CoreString)
add_subdirectory(libs/CoreColor )


##----------------------------------------------------------------------------##
## Sources                                                                    ##
##----------------------------------------------------------------------------##
set(SOURCES
    ColorPicker/main.cpp
    ColorPicker/MainWindow.cpp
)


##----------------------------------------------------------------------------##
## Executable                                                                 ##
##----------------------------------------------------------------------------##
add_executable(ColorPicker ${SOURCES})


##----------------------------------------------------------------------------##
## Dependencies                                                               ##
##----------------------------------------------------------------------------##
target_link_libraries(ColorPicker LINK_PUBLIC Qt5::Widgets)
target_link_libraries(ColorPicker LINK_PUBLIC CoreAssert  )
target_link_libraries(ColorPicker LINK_PUBLIC CoreString  )
target_link_libraries(ColorPicker LINK_PUBLIC CoreColor   )