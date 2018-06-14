add_executable(producer Producer.cpp)
add_executable(pullConsumer PullConsumer.cpp)
add_executable(pushConsumer PushConsumer.cpp)

add_executable(c_producer Producer.c)

macro(link_example name link_oms)
    target_link_libraries(${name} dl)

    if (${link_oms} STREQUAL "ON")
        target_link_libraries(${name} oms_rocketmq)
    endif ()

    if(UNIX)
        if(NOT APPLE)
            set_target_properties(${name} PROPERTIES
                    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
                    LINK_FLAGS "-Wl,-rpath,\$ORIGIN/lib")
        else()
            set_target_properties(${name} PROPERTIES
                    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
                    LINK_FLAGS "-Wl,-rpath,./lib")
        endif()
    endif()
endmacro()

link_example(producer OFF)
link_example(pullConsumer OFF)
link_example(pushConsumer OFF)
link_example(c_producer ON)