# ##############################
# Inversion Of Control (IoC) PATTERN
# ##############################
set(IOC_BINARY ioc_test)
set(IOC_BINARY ${IOC_BINARY} PARENT_SCOPE)
add_executable(${IOC_BINARY}
        ioc.cpp
        ${CMAKE_BINARY_DIR}/include/di/ioc.hpp)
add_test(NAME ${IOC_BINARY} COMMAND ${IOC_BINARY})
target_link_libraries(${IOC_BINARY} gtest)
