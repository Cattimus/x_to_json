cmake_minimum_required(VERSION 3.15)

#output locations (for windows and linux)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY x_to_json/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY x_to_json/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY x_to_json/lib)

#copy header files to include directory
file(GLOB HEADER_FILES "src/*.h")
file(COPY ${HEADER_FILES} DESTINATION "x_to_json/include")

#only works with gcc/g++
add_compile_options(-O3 -s)

project("x_to_json")
add_library(x_to_json_dynamic SHARED "src/value.cpp" "src/json_object.cpp" "src/json_array.cpp" "src/x_to_json.cpp")
add_library(x_to_json_static STATIC "src/value.cpp" "src/json_object.cpp" "src/json_array.cpp" "src/x_to_json.cpp")
