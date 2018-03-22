add_executable(producer Producer.cpp)
add_executable(pullConsumer PullConsumer.cpp)
add_executable(pushConsumer PushConsumer.cpp)

macro(link_example name)
    target_link_libraries(${name} dl)
    set_target_properties(${name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
            LINK_FLAGS "-Wl,-rpath,\$ORIGIN/lib")
endmacro()

link_example(producer)
link_example(pullConsumer)
link_example(pushConsumer)