include(/home/roman/dev_code/LernApp/LernappQuick/build/Desktop_Qt_6_8_2-Debug/cmake/CPM_0.27.2.cmake)
CPMAddPackage(NAME;curl;GITHUB_REPOSITORY;curl/curl;GIT_TAG;curl-7_82_0;OPTIONS;CURL_ENABLE_SSL OFF;CMAKE_USE_LIBSSH2 OFF;BUILD_CURL_EXE OFF;BUILD_SHARED_LIBS OFF;BUILD_TESTING OFF)
set(curl_FOUND TRUE)