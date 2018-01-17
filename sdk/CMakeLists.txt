add_library(${CMAKE_PROJECT_NAME} SHARED
        core.cpp
        KeyValueImpl.cpp
        ProducerImpl.cpp
        MessagingAccessPointImpl.cpp
        MessagingAccessPointFactory.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} ${JNI_LIBRARIES})

add_definitions(-DBOOST_LOG_DYN_LINK=1)

if (Boost_LIBRARIES)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES})
else ()
    target_link_libraries(${CMAKE_PROJECT_NAME} boost_log-mt boost_date_time-mt boost_system-mt boost_thread-mt)
endif ()

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

install(TARGETS ${CMAKE_PROJECT_NAME}
        LIBRARY DESTINATION lib)
