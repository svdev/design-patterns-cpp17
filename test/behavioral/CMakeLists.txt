# ##############################
# VISITOR PATTERN
# ##############################
set(VISITOR_BINARY visitor_test)
set(VISITOR_BINARY ${VISITOR_BINARY} PARENT_SCOPE)
add_executable(${VISITOR_BINARY}
        visitor.cpp
        ${CMAKE_BINARY_DIR}/include/behavioral/visitor.hpp)
add_test(NAME ${VISITOR_BINARY} COMMAND ${VISITOR_BINARY})
target_link_libraries(${VISITOR_BINARY} gtest)

# ##############################
# OBSERVER PATTERN
# ##############################
set(OBSERVER_BINARY observer_test)
set(OBSERVER_BINARY ${OBSERVER_BINARY} PARENT_SCOPE)
add_executable(${OBSERVER_BINARY}
        observer.cpp
        ${CMAKE_BINARY_DIR}/include/behavioral/observer.hpp)
add_test(NAME ${OBSERVER_BINARY} COMMAND ${OBSERVER_BINARY})
target_link_libraries(${OBSERVER_BINARY} gtest)
