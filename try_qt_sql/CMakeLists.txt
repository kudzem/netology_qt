cmake_minimum_required(VERSION 3.14)

project(try_qt_sql LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Sql Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql Network)

add_executable(try_qt_sql
  main.cpp
)
target_link_libraries(try_qt_sql Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(try_qt_sql Qt${QT_VERSION_MAJOR}::Sql)
target_link_libraries(try_qt_sql Qt${QT_VERSION_MAJOR}::Network)

include(GNUInstallDirs)
install(TARGETS try_qt_sql
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
