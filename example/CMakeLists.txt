add_executable(producer Producer.cpp)
add_executable(pullConsumer PullConsumer.cpp)
add_executable(pushConsumer PushConsumer.cpp)

add_executable(c_producer Producer.c)

macro(link_example name)
    target_link_libraries(${name} dl)
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

link_example(producer)
link_example(pullConsumer)
link_example(pushConsumer)
link_example(c_producer)