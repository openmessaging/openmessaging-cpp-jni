add_library(${CMAKE_PROJECT_NAME} SHARED
        JavaOption.cpp
        CurrentEnv.cpp
        CountdownLatch.cpp
        core.cpp
        KeyValueImpl.cpp
        OMS.cpp
        BuiltinKeys.cpp
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
target_link_libraries(${CMAKE_PROJECT_NAME} ${JNI_LIBRARIES})

add_definitions(-DBOOST_LOG_DYN_LINK=1)
add_definitions(-DBOOST_ALL_DYN_LINK=1)

if (Boost_LIBRARIES)
    target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES})
else ()
    target_link_libraries(${CMAKE_PROJECT_NAME}
            boost_log-mt
            boost_date_time-mt
            boost_system-mt
            boost_thread-mt
            boost_chrono-mt
            boost_filesystem-mt
            )
endif ()

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

install(TARGETS ${CMAKE_PROJECT_NAME}
        LIBRARY DESTINATION lib)
