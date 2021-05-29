cmake_minimum_required(VERSION 3.5)

project(GraphDecimator LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED PrintSupport)
find_package(ElesyOpcUaClient REQUIRED)

add_executable(
    GraphDecimator
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    qcustomplot.cpp
    qcustomplot.h
    datadecimator.cpp
    datadecimator.h
    graph.cpp
    graph.h
    util.cpp
    util.h
    quabrowser.cpp
    quabrowser.h
    quaconfigurationwidget.cpp
    quaconfigurationwidget.h
    connectionnotifier.h
    filedialog.cpp
    filedialog.h
    stylesheet.cpp
    stylesheet.h
)

target_link_libraries(
    GraphDecimator
    PRIVATE
    Qt5::Widgets
    Qt5::PrintSupport
    ElesyOpcUaClient::ElesyOpcUaClient
    pthread
)
