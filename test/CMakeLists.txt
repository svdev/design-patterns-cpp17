enable_testing()

add_subdirectory(creational)
add_subdirectory(behavioral)
add_subdirectory(structural)
add_subdirectory(di)

add_custom_target(check
        COMMAND ${SINGLETON_BINARY}
        COMMAND ${STATIC_FACTORY_BINARY}
        COMMAND ${ABSTRACT_FACTORY_BINARY}
        COMMAND ${VISITOR_BINARY}
        COMMAND ${OBSERVER_BINARY}
        COMMAND ${IOC_BINARY})
