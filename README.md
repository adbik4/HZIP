
# HZIP

A simple compression program which uses dynamic Huffman encoding
to try to reduce file size without loss of information.   

It started as a demo of Hufmann encoding for university, but later turned into a custom file format.
## Build locally

To build the project yourself, make sure you have these dependencies installed:   
- MSVC x64
- CMake
- vcpkg

(optionally) If you don't have Boosts program_options installed
```bash
  vcpkg install boost-program-options:x64-windows
```

Clone the project

```bash
  git clone https://github.com/adbik4/HZIP.git
```

Go to the project directory

```bash
  cd HZIP\HuffmanCoding
```

Add a build directory

```bash
  mkdir build
  cd build
```

Build

```bash
  cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
  cmake --build . --config Release
```
(optionally) Add HZIP to your PATH variable
- on Windows
```bash
[Environment]::SetEnvironmentVariable("Path", "$env:Path;C:\path\to\your\build\Release\", "User")
```
- on Linux
```bash
  nano ~/.bashrc
  export PATH="\path\to\your\build\HZIP.exe:$PATH"
```

Finally, restart your terminal
## Demo

placeholder


## Screenshots

![Available Commands](/docs/demo_screenshot.png?raw=true "Available Commands")

