include_directories(include)

add_executable(runUnitTests
        RunAllTest.cpp
        CountdownLatch.cpp
        scoped_array_test.cpp
        ByteMessageImplTest.cpp
        UncopyableTest.cpp
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

target_link_libraries(runUnitTests
        dl
        pthread
        boost_log-mt
        boost_date_time-mt
        boost_system-mt
        boost_thread-mt
        boost_chrono-mt
        gtest
        oms_rocketmq)

set_target_properties(runUnitTests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
