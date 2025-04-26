# CMakeList.txt : CMake project for HuffmanCoding, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (SmartCpp-HuffmanCoding "main.cpp" "message.h" "tree.h"  "storage.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SmartCpp-HuffmanCoding PROPERTY CXX_STANDARD 20)
  set_property(TARGET SmartCpp-HuffmanCoding PROPERTY CXX_STANDARD_REQUIRED ON)
endif()

# TODO: Add tests and install targets if needed.
