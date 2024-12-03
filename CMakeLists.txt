cmake_minimum_required(VERSION 3.16)

project(nimamp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets Svg Multimedia)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TAGLIB REQUIRED IMPORTED_TARGET taglib)

include_directories(${TAGLIB_INCLUDE_DIRS})
link_directories(${TAGLIB_LIBRARY_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

qt_standard_project_setup(REQUIRES 6.5)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

qt_add_executable(appnimamp
        sources/main.cpp
        headers/key_events.hpp
        headers/player.hpp
        sources/player.cpp
        headers/playlist.hpp
        sources/playlist.cpp
        headers/utils.hpp
        headers/song.hpp
        sources/song.cpp
        sources/mainwindow.cpp
        headers/mainwindow.hpp
        sources/queue_song_item.cpp
        headers/queue_song_item.hpp
        sources/playlist_item.cpp
        headers/playlist_item.hpp
        sources/queue_widget.cpp
        headers/queue_widget.hpp
        sources/playlist_view_widget.cpp
        headers/playlist_view_widget.hpp
        sources/transport_widget.cpp
        headers/transport_widget.hpp
        sources/playlist_tabs_widget.cpp
        headers/playlist_tabs_widget.hpp
        sources/metadata_widget.cpp
        headers/metadata_widget.hpp

        ui/MainWindow.ui
        ui/QueueSongItem.ui
        ui/PlaylistItem.ui
        ui/QueueWidget.ui
        ui/PlaylistViewWidget.ui
        ui/TransportWidget.ui
        ui/PlaylistTabsWidget.ui
        ui/MetadataWidget.ui
)

qt_add_resources(appnimamp resources.qrc)

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
        PRIVATE Qt6::Svg
        PRIVATE ${TAGLIB_LIBRARIES}
)

include(GNUInstallDirs)
install(TARGETS appnimamp
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
