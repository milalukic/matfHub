# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/MatfHub_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/MatfHub_autogen.dir/ParseCache.txt"
  "MatfHub_autogen"
  )
endif()
