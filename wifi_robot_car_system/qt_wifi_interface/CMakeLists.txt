cmake_minimum_required(VERSION 3.16)

project(RobotWiFiControl VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Tim kiem cac thu vien Network thay vi Bluetooth
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

add_executable(RobotWiFiControl ${PROJECT_SOURCES})

# Lien ket voi Qt6::Network
target_link_libraries(RobotWiFiControl PRIVATE Qt6::Widgets Qt6::Network)

set_target_properties(RobotWiFiControl PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
