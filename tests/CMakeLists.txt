# file(GLOB_RECURSE TEST_FILES ${CMAKE_SOURCE_DIR}/tests/*.cpp)

include_directories(include)

add_executable(runUnitTests
        RunAllTest.cpp
        CountdownLatch.cpp
        scoped_array_test.cpp
        ByteMessageImplTest.cpp
        UncopyableTest.cpp
#        CountdownLatchTest.cpp
        CoreTest.cpp
        CurrentEnvTest.cpp
        KeyValueImplTest.cpp
        OMSTest.cpp
        PromiseImplTest.cpp
        MessagingAccessPointFactoryImplTest.cpp
        ProducerImplTest.cpp
        PullConsumerImplTest.cpp
        PushConsumerImplTest.cpp
        ServerTest.cpp
        )

# Standard linking to gtest stuff.
target_link_libraries(runUnitTests gtest)

add_definitions(-DBOOST_LOG_DYN_LINK=1)
add_definitions(-DBOOST_ALL_DYN_LINK=1)

# Extra linking for the project.
target_link_libraries(runUnitTests ${CMAKE_PROJECT_NAME})

if (Boost_LIBRARIES)
    target_link_libraries(runUnitTests ${Boost_LIBRARIES})
else()
    target_link_libraries(runUnitTests
            pthread
            boost_log-mt
            boost_date_time-mt
            boost_system-mt
            boost_thread-mt
            boost_chrono-mt)
endif ()

set_target_properties(runUnitTests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
