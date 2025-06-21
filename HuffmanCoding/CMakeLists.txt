# CMakeList.txt : CMake project for HuffmanCoding, include source and define
# project specific logic here.
#

set(CPP_LIBS E:/Programowanie/C++/libraries)

set(SOURCES
    Src/main.cpp
    Src/io.cpp
    Src/tree.cpp
    Src/types.cpp
    Src/file.cpp
    Src/general.cpp
)

set(HEADERS
    Inc/file.h
    Inc/tree.h
    Inc/general.h
)

add_executable(SmartCpp-HuffmanCoding ${SOURCES} ${HEADERS})

target_include_directories(SmartCpp-HuffmanCoding PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Src
    ${CPP_LIBS}/boost_1_88_0
)

find_package(Boost REQUIRED COMPONENTS program_options)

target_link_libraries(SmartCpp-HuffmanCoding PRIVATE Boost::program_options)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SmartCpp-HuffmanCoding PROPERTY CXX_STANDARD 20)
  set_property(TARGET SmartCpp-HuffmanCoding PROPERTY CXX_STANDARD_REQUIRED ON)
endif()

# TODO: Add tests and install targets if needed.
