include_directories(include)

add_executable(runUnitTests
        RunAllTest.cpp
        smart_pointer_test.cpp
        CountdownLatch.cpp
        ManagedArrayTest.cpp
        ByteMessageImplTest.cpp
        UncopyableTest.cpp
        CoreTest.cpp
        CurrentEnvTest.cpp
        KeyValueImplTest.cpp
        ServerTest.cpp
        OMSTest.cpp
        PromiseImplTest.cpp
        ProducerImplTest.cpp
        SingletonTest.cpp
        PullConsumerImplTest.cpp
        PushConsumerImplTest.cpp
        )

target_link_libraries(runUnitTests
        dl
        ${JAVA_JVM_LIBRARY}
        ${BOOST_chrono_LIBRARY}
        ${BOOST_date_time_LIBRARY}
        ${BOOST_thread_LIBRARY}
        ${BOOST_filesystem_LIBRARY}
        ${BOOST_system_LIBRARY}
        gtest
        oms_rocketmq)

if (UNIX AND NOT APPLE)
    target_link_libraries(runUnitTests rt)
endif ()

set_target_properties(runUnitTests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
