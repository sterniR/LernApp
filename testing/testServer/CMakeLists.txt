cmake_minimum_required(VERSION 3.16)

project(testServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(testServer main.cpp
    ftpsclient.h
    ftpsclient.cpp)

include(GNUInstallDirs)
install(TARGETS testServer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Network)

target_link_libraries(testServer PRIVATE Qt6::Core)
target_link_libraries(testServer PRIVATE Qt6::Network)
