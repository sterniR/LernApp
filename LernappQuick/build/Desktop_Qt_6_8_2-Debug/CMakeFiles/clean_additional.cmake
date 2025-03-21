# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appLernappQuick_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appLernappQuick_autogen.dir/ParseCache.txt"
  "appLernappQuick_autogen"
  )
endif()
