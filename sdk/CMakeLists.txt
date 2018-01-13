add_library(${CMAKE_PROJECT_NAME} SHARED core.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}
        ${JNI_LIBRARIES})

if (Boost_LIBRARIES)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES})
else()
    target_link_libraries(${CMAKE_PROJECT_NAME} boost_system-mt boost_thread-mt)
endif ()

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/distribution)

install(TARGETS ${CMAKE_PROJECT_NAME}
        LIBRARY DESTINATION lib)