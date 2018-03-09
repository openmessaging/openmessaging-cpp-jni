add_library(oms_rocketmq SHARED
        JavaOption.cpp
        CurrentEnv.cpp
        core.cpp
        KeyValueImpl.cpp
        OMS.cpp
        ServiceLifecycleImpl.cpp
        PromiseImpl.cpp

        ByteMessageImpl.cpp
        producer/SendResultImpl.cpp
        producer/ProducerImpl.cpp
        producer/LocalTransactionBranchExecutorImpl.cpp

        consumer/ContextImpl.cpp
        consumer/StreamImpl.cpp
        consumer/PullConsumerImpl.cpp
        consumer/PushConsumerImpl.cpp
        consumer/StreamingConsumerImpl.cpp

        MessagingAccessPointImpl.cpp
        MessagingAccessPointFactory.cpp)

if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    message("Boost is dynamically linked")
    target_link_libraries(oms_rocketmq
            dl
            boost_date_time-mt
            boost_system-mt
            boost_thread-mt
            boost_chrono-mt
            boost_filesystem-mt
            jvm
            )
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message("Boost is statically linked")
    set_target_properties(oms_rocketmq PROPERTIES LINK_FLAGS
            "-Wl,-Bstatic -lboost_date_time -lboost_system -lboost_thread -lboost_filesystem -Wl,-Bdynamic -ldl -ljvm")
else()
    message("Compiler ID is unknown")
endif ()

set_target_properties(oms_rocketmq PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

install(TARGETS oms_rocketmq LIBRARY DESTINATION lib)
