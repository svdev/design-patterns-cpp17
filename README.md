# Design Patterns C++17

Design Patterns Headers-only Library for C++17


## Build

    git clone --recurse-submodules http://github.com/svdev/design-patterns-cpp17
    cd design-patterns-cpp17
    cmake -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -G "Unix Makefiles" .
    make -j8    

## Test

    make check
    
    
## Install

    sudo make install    
    
## Usage

    project("my-project")
    
    find_package(patterns CONFIG REQUIRED)
    
    add_executable(${PROJECT_NAME} src/main.cc)
    target_link_libraries(${PROJECT_NAME} patterns::patterns)

    
## Author

    svdev - http://github.com/svdev