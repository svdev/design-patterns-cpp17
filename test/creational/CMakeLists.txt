# ##############################
# ABSTRACT FACTORY PATTERN
# ##############################
set(ABSTRACT_FACTORY_BINARY abstract_factory_test)
set(ABSTRACT_FACTORY_BINARY ${ABSTRACT_FACTORY_BINARY} PARENT_SCOPE)
add_executable(${ABSTRACT_FACTORY_BINARY}
        abstract_factory.cpp
        ${CMAKE_BINARY_DIR}/include/creational/factory.hpp)
add_test(NAME ${ABSTRACT_FACTORY_BINARY} COMMAND ${ABSTRACT_FACTORY_BINARY})
target_link_libraries(${ABSTRACT_FACTORY_BINARY} gtest)

# ##############################
# STATIC FACTORY PATTERN
# ##############################
set(STATIC_FACTORY_BINARY static_factory_test)
set(STATIC_FACTORY_BINARY ${STATIC_FACTORY_BINARY} PARENT_SCOPE)
add_executable(${STATIC_FACTORY_BINARY}
        static_factory.cpp
        ${CMAKE_BINARY_DIR}/include/creational/factory.hpp)
add_test(NAME ${STATIC_FACTORY_BINARY} COMMAND ${STATIC_FACTORY_BINARY})
target_link_libraries(${STATIC_FACTORY_BINARY} gtest)

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
