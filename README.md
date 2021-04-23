# x_to_json
Json parsing/construction library written in c++

## Documentation
[Access the documentation here](https://github.com/Cattimus/x_to_json/wiki)

## Sidenote and warnings
This project was originally created as an exercise in writing your own portable library and using it in another project. I chose json specifically because it's required for virtually everything on the web and does not have native support in most languages. 

That being said, using this code in production is likely a very bad idea. While I've done my best to prevent harmful mistakes, I haven't covered every possible case or every possible error. You shouldn't be using this library in any situation where you may face any kind of security threats, since it will likely be trivial to exploit.

## Build instructions

If you are familiar with cmake and build systems in general, feel free to ignore these instructions and build in any way you see fit. This library doesn't require any special linking and only uses pieces of the C++ and C standard library, so most compilers should work just fine.

Building has only been tested on MinGW/g++, so that is the recommended compiler option.

### Windows requirements:
- cmake of version 3.15 or higher
- any compiler that can be used with cmake (comment out line 13 in CMakeLists.txt if you're not using MinGW)

1. Navigate to the folder that contains the x_to_json project files
2. Create a folder named "build" or something similar
3. Navigate to the build folder
4. To generate build files for a MinGW build, run `cmake -G "MinGW Makefiles" ..`
5. To build the project, run `mingw32-make`

You should now have an output directory called "x_to_json" that contains the files necessary for linking.

### Linux requirements:
- g++ installed
- cmake installed

1. Navigate to the folder that contains the x_to_json project files
2. Create a folder named "build" or something similar
3. Navigate to the build folder
4. To generate build files, run `cmake ..`
5. To build the project, run `make`

You should now have an output directory called "x_to_json" that contains the files necessary for linking
