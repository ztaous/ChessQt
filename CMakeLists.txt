cmake_minimum_required(VERSION 3.16)
project(ChessQt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg)

set(PROJECT_SOURCES
    src/main.cpp
    src/gui/mainwindow.cpp src/gui/mainwindow.h src/gui/mainwindow.ui
    src/gui/boardview.cpp src/gui/boardview.h
    src/board.cpp src/board.h
    src/piece.h
    src/king.cpp src/king.h
    src/queen.cpp src/queen.h
    src/rook.cpp src/rook.h
    src/bishop.cpp src/bishop.h
    src/knight.cpp src/knight.h
    src/pawn.cpp src/pawn.h
    resources/resources.qrc
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_executable(ChessQt ${PROJECT_SOURCES})
else()
    add_executable(ChessQt ${PROJECT_SOURCES})
endif()

target_compile_options(ChessQt PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

target_link_libraries(ChessQt PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Svg
)

set_target_properties(ChessQt PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ChessQt
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cpp src/*.h)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
        COMMENT "Running clang-format"
    )
endif()
