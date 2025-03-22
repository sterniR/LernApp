cmake_minimum_required(VERSION 3.16)

project(LernappQuick VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPM_DOWNLOAD_VERSION 0.27.2)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()

include(${CPM_DOWNLOAD_LOCATION})

CPMAddPackage(
    NAME curl
    GITHUB_REPOSITORY "curl/curl"
    GIT_TAG "curl-7_82_0"
    OPTIONS
        "CURL_ENABLE_SSL OFF"
        "CMAKE_USE_LIBSSH2 OFF"
        "BUILD_CURL_EXE OFF"
        "BUILD_SHARED_LIBS OFF"
        "BUILD_TESTING OFF"
)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appLernappQuick
    main.cpp
)

qt_add_qml_module(appLernappQuick
    URI LernappQuick
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES networtbackend.h networtbackend.cpp
)

# set_source_files_properties(Main.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appLernappQuick PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appLernappQuick
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appLernappQuick
    PRIVATE Qt6::Quick
    libcurl
    PRIVATE Qt6::QuickControls2
)

include(GNUInstallDirs)
install(TARGETS appLernappQuick
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
