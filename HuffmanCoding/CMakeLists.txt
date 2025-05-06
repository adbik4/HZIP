# CMakeList.txt : CMake project for HuffmanCoding, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (SmartCpp-HuffmanCoding "main.cpp" "file.h" "tree.h"   "io.cpp" "tree.cpp" "types.cpp" "file.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SmartCpp-HuffmanCoding PROPERTY CXX_STANDARD 20)
  set_property(TARGET SmartCpp-HuffmanCoding PROPERTY CXX_STANDARD_REQUIRED ON)
endif()

# Link system library (Windows Sockets)
target_link_libraries(SmartCpp-HuffmanCoding PRIVATE ws2_32)

# TODO: Add tests and install targets if needed.
