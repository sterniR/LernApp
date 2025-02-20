cmake_minimum_required(VERSION 3.16)

project(devLernapp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPM_DOWNLOAD_VERSION 0.27.2)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()

include(${CPM_DOWNLOAD_LOCATION})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Sql Network)

# CPMAddPackage(
#     NAME libpsl
#     GIT_REPOSITORY "https://github.com/rockdaboot/libpsl.git"
#     GIT_TAG "0.21.5"
#     # CONFIGURE_COMMAND ./autogen.sh && ./configure --disable-shared --prefix=${CMAKE_BINARY_DIR}/_deps/libpsl-install
#     # BUILD_COMMAND make -j$(nproc)
#     # INSTALL_COMMAND make install
# )

CPMAddPackage(
    NAME mbedtls
    GITHUB_REPOSITORY "Mbed-TLS/mbedtls"
    GIT_TAG "v3.6.2"
    OPTIONS
        "ENABLE_PROGRAMS OFF"
        "ENABLE_TESTING OFF"
)

set(MBEDTLS_INCLUDE_DIRS ${mbedtls_SOURCE_DIR}/include)
set(MBEDTLS_LIBRARY ${mbedtls_BINARY_DIR}/library/libmbedtls.a)
set(MBEDX509_LIBRARY ${mbedtls_BINARY_DIR}/library/libmbedx509.a)
set(MBEDCRYPTO_LIBRARY ${mbedtls_BINARY_DIR}/library/libmbedcrypto.a)

CPMAddPackage(
    NAME curl
    GITHUB_REPOSITORY "curl/curl"
    GIT_TAG "curl-7_82_0"
    OPTIONS
        "CMAKE_USE_LIBSSH2 OFF"
        "CURL_USE_MBEDTLS ON"
        "BUILD_CURL_EXE OFF"
        "BUILD_SHARED_LIBS OFF"
        "BUILD_TESTING OFF"
)

set(PROJECT_SOURCES
        main.cpp
        lernapp.cpp
        lernapp.h
        lernapp.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(devLernapp
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET devLernapp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(devLernapp SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(devLernapp
            ${PROJECT_SOURCES}
        )
    endif()
endif()

include_directories(${MBEDTLS_INCLUDE_DIRS})
link_directories(${mbedtls_BINARY_DIR}/library)

target_link_libraries(devLernapp PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt6::Network
    libcurl
)



# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.devLernapp)
endif()
set_target_properties(devLernapp PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS devLernapp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(devLernapp)
endif()
