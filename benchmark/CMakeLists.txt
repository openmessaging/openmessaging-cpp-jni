add_executable(benchmark_example ExampleBenchmarkTest.cpp)

add_executable(benchmark_fixture FixtureBenchmarkTest.cpp)

add_executable(benchmark_producer ProducerImplBenchmarkTest.cpp)

target_link_libraries(benchmark_example benchmark)
target_link_libraries(benchmark_fixture benchmark boost_thread-mt boost_system-mt)
target_link_libraries(benchmark_producer
        benchmark
        ${CMAKE_PROJECT_NAME}
        boost_log-mt
        boost_date_time-mt
        boost_system-mt
        boost_thread-mt
        boost_chrono-mt)