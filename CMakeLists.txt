cmake_minimum_required(VERSION 3.16)

project(nimamp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets Multimedia)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appnimamp
        sources/main.cpp
        headers/player.hpp
        sources/player.cpp
        headers/playlist.hpp
        sources/playlist.cpp
        headers/utils.hpp
        headers/song.hpp
        sources/song.cpp
        sources/mainwindow.cpp
        headers/mainwindow.hpp
        sources/MainWindow.ui
        sources/queue.cpp
        headers/queue.hpp
        sources/SongItem.cpp
        headers/song_item.hpp
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appnimamp PROPERTIES
        #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appnimamp
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

target_link_libraries(appnimamp
        PRIVATE Qt6::Multimedia
        PRIVATE Qt6::Widgets
)

include(GNUInstallDirs)
install(TARGETS appnimamp
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
