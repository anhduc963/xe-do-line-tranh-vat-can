cmake_minimum_required(VERSION 3.16)

# Ten du an va phien ban
project(RobotControl VERSION 1.0.0 LANGUAGES CXX)

# Thiet lap tieu chuan C++17 (bat buoc cho Qt 6)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tu dong xu ly cac file dac trung cua Qt nhu MOC (Signal/Slot), RCC (Resources), va UIC (Giao dien)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Tim kiem cac thu vien Qt 6 bat buoc: Widgets (Giao dien) va Bluetooth
find_package(Qt6 REQUIRED COMPONENTS Widgets Bluetooth)

# Danh sach cac file nguon trong du an
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Tao file thuc thi tu cac file nguon
add_executable(RobotControl ${PROJECT_SOURCES})

# Lien ket du an voi cac thu vien Qt 6 tuong ung
target_link_libraries(RobotControl PRIVATE Qt6::Widgets Qt6::Bluetooth)

# Thiet lap thu tinh cho file thuc thi (an cua so lenh tren Windows)
set_target_properties(RobotControl PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
