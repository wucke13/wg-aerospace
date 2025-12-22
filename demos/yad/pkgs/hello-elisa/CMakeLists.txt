cmake_minimum_required(VERSION 3.13)  # CMake version check
project(hello_elisa)                  # Create project "hello_elisa"
set(CMAKE_C_STANDARD 23)              # Enable C23 standard



# Add main.cpp file of the project root directory as a source file
set(SOURCE_FILES src/main.c)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(hello_elisa ${SOURCE_FILES})


# Make the compiler be as strict as possible
if(MSVC)
  target_compile_options(hello_elisa PRIVATE /W4 /WX)
else()
  target_compile_options(hello_elisa PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()


# Add install target
install(TARGETS hello_elisa DESTINATION bin)
