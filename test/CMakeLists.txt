enable_testing()

add_subdirectory(creational)
add_subdirectory(behavioral)
add_subdirectory(structural)

add_custom_target(check
        COMMAND ${SINGLETON_BINARY}
        COMMAND ${FACTORY_BINARY}
        COMMAND ${VISITOR_BINARY}
        COMMAND ${OBSERVER_BINARY})
