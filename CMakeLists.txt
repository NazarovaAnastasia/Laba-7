cmake_minimum_required(VERSION 3.14)
project(order_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tests
    domain/money.cpp
    domain/order_line.cpp
    domain/order.cpp
    
    application/pay_order_use_case.cpp
    
    infrastructure/in_memory_order_repository.cpp
    infrastructure/fake_payment_gateway.cpp
    
    test/pay_order_use_case_tests.cpp
)

include_directories(
    domain
    application
    infrastructure
    interfaces
)

find_package(GTest REQUIRED)
target_link_libraries(tests GTest::gtest GTest::gtest_main)

enable_testing()
add_test(NAME all_tests COMMAND tests)
