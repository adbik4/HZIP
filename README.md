
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

## Screenshots

![Available Commands](/docs/demo_screenshot.png?raw=true "Available Commands")

![File Analysis](/docs/demo_screenshot2.png?raw=true "File Analysis")


## Benchmark

Here's how the algorithm stacks up against other compression methods with their default settings.

- ### Text file performance - on the script of the entire Bee Movie
![Text Comparison](/docs/text_comparison_alpha.png?raw=true "Text Comparison")
The ideal scenario for most compression algorithms, there are plenty of patterns to simplify.

- ### Mixed file performance - on a scientific paper in .pdf format
![Mixed Comparison](/docs/mixed_comparison_alpha.png?raw=true "Mixed Comparison")
PDFs offer a good mix of both text and images, and are already semi-compressed.

- ### Image file performance - on a high resolution .png photograph
![Image Comparison](/docs/image_comparison_alpha.png?raw=true "Image Comparison")
PNG files are already heavily compressed, this is a good example of "uncompressible" data.

- ### High entropy data performance - on a 1 MiB stream from /dev/urandom
![Random Comparison](/docs/random_comparison_alpha.png?raw=true "Random Comparison")
Worst case scenario for all compression algorithms.
