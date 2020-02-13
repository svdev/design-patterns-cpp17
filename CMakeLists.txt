cmake_minimum_required(VERSION 3.12.4)
project("patterns"
        VERSION 0.1.0
        DESCRIPTION "A header only c++17 library with GoF design patterns"
        HOMEPAGE_URL "https://github.com/svdev/design-patterns-cpp17"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

add_library(${PROJECT_NAME} INTERFACE)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

set(CMAKE_ROOT)
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_ROOT}/Modules/${PROJECT_NAME})

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        DESTINATION ${CMAKE_ROOT}/Modules/${PROJECT_NAME})

file(GLOB PATTERN_HEADERS
        ${PROJECT_SOURCE_DIR}/include/**/*.hpp
        ${PROJECT_SOURCE_DIR}/include/*.hpp)

foreach(FILE ${PATTERN_HEADERS})
    get_filename_component(DIR ${FILE} DIRECTORY)
    file(RELATIVE_PATH REL ${PROJECT_SOURCE_DIR}/include ${DIR})
    install(FILES ${FILE} DESTINATION include/${PROJECT_NAME}/${REL})
endforeach()

include_directories(include)

# EXCLUDE_FROM_ALL disables install targets for googletest subdirectory.
add_subdirectory(lib/googletest EXCLUDE_FROM_ALL)
add_subdirectory(test)
