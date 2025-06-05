# CMakeList.txt : CMake project for HuffmanCoding, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (SmartCpp-HuffmanCoding "Src/main.cpp" "Inc/file.h" "Inc/tree.h"   "Src/io.cpp" "Src/tree.cpp" "Src/types.cpp" "Src/file.cpp")

target_include_directories(SmartCpp-HuffmanCoding PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Src
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SmartCpp-HuffmanCoding PROPERTY CXX_STANDARD 20)
  set_property(TARGET SmartCpp-HuffmanCoding PROPERTY CXX_STANDARD_REQUIRED ON)
endif()

# TODO: Add tests and install targets if needed.
