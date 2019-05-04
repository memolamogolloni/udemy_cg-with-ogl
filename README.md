# PRACTICES ON UDEMY'S [OPENGL COURSE](https://www.udemy.com/graphics-with-modern-opengl/)

#### Requires:

* **[Make](https://www.gnu.org/software/make/)**
* **[CMake](https://cmake.org/)**
* **[Conan Package Manager](https://conan.io/)**

## USAGE:

* Create build folder and go into it:
  *`mkdir build && cd /build`*
* Install *conan* dependencies listed in *conantfile.txt*:
  `conan install ..`

* Run CMake based on rules specified at  *CMakeLists.txt*:
  `cmake ..`
* Build and run:
  `make && make run`