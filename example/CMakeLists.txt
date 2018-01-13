add_executable(demo main.cpp)

target_link_libraries(demo ${CMAKE_PROJECT_NAME})

if (Boost_LIBRARIES)
    target_link_libraries(demo ${Boost_LIBRARIES})
else()
    target_link_libraries(demo boost_system-mt boost_thread-mt)
endif ()

add_executable(example_generic example_generic.cpp)