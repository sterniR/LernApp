cmake_minimum_required(VERSION 3.16)

project(devLernapp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(OPENSSL_ROOT_DIR "${CMAKE_BINARY_DIR}/_deps/openssl-build")
# set(OPENSSL_INCLUDE_DIR "${CMAKE_BINARY_DIR}/_deps/openssl-src/include")
# set(OPENSSL_CRYPTO_LIBRARY "${CMAKE_BINARY_DIR}/_deps/openssl-build/lib/libcrypto.a")
# set(OPENSSL_SSL_LIBRARY "${CMAKE_BINARY_DIR}/_deps/openssl-build/lib/libssl.a")

include(cmake/CPM.cmake)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Sql)
find_package(Qt6 REQUIRED COMPONENTS Network)
# find_package(OpenSSL REQUIRED)

# CPMAddPackage("https://github.com/Mbed-TLS/mbedtls/archive/refs/tags/mbedtls-3.6.2.zip")

CPMAddPackage(
    NAME openssl
    GITHUB_REPOSITORY "openssl/openssl"
    GIT_TAG "openssl-3.4.1"
    DOWNLOAD_ONLY YES
)

if(openssl_ADDED)
    set(OPENSSL_SRC_DIR "${openssl_SOURCE_DIR}")

    execute_process(
        COMMAND ./config no-shared --prefix=${CMAKE_BINARY_DIR}/_deps/openssl-build
        WORKING_DIRECTORY ${OPENSSL_SRC_DIR}
    )

    execute_process(
        COMMAND make -j$(nproc)
        WORKING_DIRECTORY ${OPENSSL_SRC_DIR}
    )

    execute_process(
        COMMAND make install
        WORKING_DIRECTORY ${OPENSSL_SRC_DIR}
    )
endif()

CPMAddPackage(
    NAME mbedtls
    GITHUB_REPOSITORY "Mbed-TLS/mbedtls"
    GIT_TAG "v3.6.2"
)

CPMAddPackage(
    NAME curl
    GITHUB_REPOSITORY "curl/curl"
    GIT_TAG "curl-8_12_0"
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
        android/AndroidManifest.xml android/build.gradle android/res/values/libs.xml android/res/xml/qtprovider_paths.xml android/gradle/wrapper/gradle-wrapper.jar android/gradle/wrapper/gradle-wrapper.properties android/gradle.properties android/gradlew android/gradlew.bat
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

target_link_libraries(devLernapp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(devLernapp PRIVATE Qt${QT_VERSION_MAJOR}::Sql)
target_link_libraries(devLernapp PRIVATE Qt6::Network)
target_link_libraries(devLernapp PRIVATE libcurl)
target_include_directories(devLernapp PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(devLernapp PRIVATE ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})

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
