cmake_minimum_required(VERSION 3.16)

project(nimamp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets Svg Multimedia Network)

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
        src/main.cpp
        include/key_events.hpp
        include/player.hpp
        src/player.cpp
        include/playlist.hpp
        src/playlist.cpp
        include/utils.hpp
        include/song.hpp
        src/song.cpp
        src/mainwindow.cpp
        include/mainwindow.hpp
        src/song_item.cpp
        include/song_item.hpp
        src/playlist_item.cpp
        include/playlist_item.hpp
        src/queue_widget.cpp
        include/queue_widget.hpp
        src/playlist_view_widget.cpp
        include/playlist_view_widget.hpp
        src/transport_widget.cpp
        include/transport_widget.hpp
        src/playlist_tabs_widget.cpp
        include/playlist_tabs_widget.hpp
        src/metadata_widget.cpp
        include/metadata_widget.hpp
        src/lyrics_widget.cpp
        include/lyrics_widget.hpp

        ui/MainWindow.ui
        ui/SongItem.ui
        ui/PlaylistItem.ui
        ui/QueueWidget.ui
        ui/PlaylistViewWidget.ui
        ui/TransportWidget.ui
        ui/PlaylistTabsWidget.ui
        ui/MetadataWidget.ui
        ui/LyricsWidget.ui

        resources.qrc
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
        PRIVATE Qt6::Svg
        PRIVATE Qt6::Network
        PRIVATE ${TAGLIB_LIBRARIES}
)

include(GNUInstallDirs)
install(TARGETS appnimamp
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
