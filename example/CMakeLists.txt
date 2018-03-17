add_executable(producer Producer.cpp)
add_executable(pullConsumer PullConsumer.cpp)
add_executable(pushConsumer PushConsumer.cpp)

macro(link_example name)
    target_link_libraries(${name} dl)
endmacro()

link_example(producer)
link_example(pullConsumer)
link_example(pushConsumer)