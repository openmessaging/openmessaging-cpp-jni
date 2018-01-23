# file(GLOB_RECURSE TEST_FILES ${CMAKE_SOURCE_DIR}/tests/*.cpp)

add_executable(runUnitTests
        RunAllTest.cpp
        CoreTest.cpp
        KeyValueImplTest.cpp
        PromiseImplTest.cpp
        MessagingAccessPointFactoryImplTest.cpp
        )

# Standard linking to gtest stuff.
target_link_libraries(runUnitTests gtest)

# Extra linking for the project.
target_link_libraries(runUnitTests ${CMAKE_PROJECT_NAME})

if (Boost_LIBRARIES)
    target_link_libraries(runUnitTests ${Boost_LIBRARIES})
else()
    target_link_libraries(runUnitTests boost_log-mt boost_date_time-mt boost_system-mt boost_thread-mt)
endif ()

set_target_properties(runUnitTests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
