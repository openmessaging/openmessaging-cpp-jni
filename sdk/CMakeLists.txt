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

        MessagingAccessPointImpl.cpp)

target_link_libraries(oms_rocketmq
        dl
        ${JAVA_JVM_LIBRARY}
        ${BOOST_chrono_LIBRARY}
        ${BOOST_date_time_LIBRARY}
        ${BOOST_thread_LIBRARY}
        ${BOOST_filesystem_LIBRARY}
        ${BOOST_system_LIBRARY})

if (UNIX AND NOT APPLE)
    set_target_properties(oms_rocketmq PROPERTIES LINK_FLAGS "-Wl,-rpath,'$ORIGIN/vm/lib/amd64/server/'")
endif ()

set_target_properties(oms_rocketmq PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
        OUTPUT_NAME oms_jni
        )

install(TARGETS oms_rocketmq LIBRARY DESTINATION lib)
