add_library(${CMAKE_PROJECT_NAME} SHARED
        JavaOption.cpp
        CurrentEnv.cpp
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

message("Boost library directories: ${Boost_LIBRARY_DIRS}")
link_directories(${Boost_LIBRARY_DIRS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    message("Boost is dynamically linked")
    target_link_libraries(${CMAKE_PROJECT_NAME}
            boost_log-mt
            boost_date_time-mt
            boost_system-mt
            boost_thread-mt
            boost_chrono-mt
            boost_filesystem-mt
            )
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message("Boost is statically linked")
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS
            "-Wl,-Bstatic -lboost_log -lboost_date_time -lboost_system -lboost_thread -lboost_filesystem -Wl,-Bdynamic")
else()
    message("Compiler ID is unknown")
endif ()

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

install(TARGETS ${CMAKE_PROJECT_NAME}
        LIBRARY DESTINATION lib)
