# ##############################
# FACTORY PATTERN
# ##############################
set(FACTORY_BINARY factory_test)
set(FACTORY_BINARY ${FACTORY_BINARY} PARENT_SCOPE)
add_executable(${FACTORY_BINARY}
        factory.cpp
        ${CMAKE_BINARY_DIR}/include/creational/factory.hpp)
add_test(NAME ${FACTORY_BINARY} COMMAND ${FACTORY_BINARY})
target_link_libraries(${FACTORY_BINARY} gtest)

# ##############################
# SINGLETON PATTERN
# ##############################
set(SINGLETON_BINARY singleton_test)
set(SINGLETON_BINARY ${SINGLETON_BINARY} PARENT_SCOPE)
add_executable(${SINGLETON_BINARY}
        singleton.cpp
        ${CMAKE_BINARY_DIR}/include/creational/singleton.hpp)
add_test(NAME ${SINGLETON_BINARY} COMMAND ${SINGLETON_BINARY})
target_link_libraries(${SINGLETON_BINARY} gtest)
