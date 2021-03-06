cmake_minimum_required(VERSION 3.2.0)

project (cppmetrics)

set (CPACK_GENERATOR TGZ)
set (CPACK_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

set(CMAKE_CXX_STANDARD 14)

set(THREADS_PREFER_PTHREAD_FLAG OFF)
find_package(Threads REQUIRED)

set (cppmetrics_VERSION_MAJOR 0)
set (cppmetrics_VERSION_MINOR 2)
set (cppmetrics_VERSION_PATCH 0)
set (cppmetrics_VERSION_TAG "")
set (cppmetrics_VERSION "${cppmetrics_VERSION_MAJOR}.${cppmetrics_VERSION_MINOR}.${cppmetrics_VERSION_PATCH}${cppmetrics_VERSION_TAG}")
set(CPACK_PACKAGE_VERSION ${cppmetrics_VERSION})
include(CPack)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if(APPLE)
    set(CMAKE_MACOSX_RPATH 1)
    if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
      cmake_policy(SET CMP0004 NEW)
    endif(COMMAND cmake_policy)
endif()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Winit-self -Wno-unused-local-typedef -m64 -pipe")

if(DEFINED INCLUDE_SYSTEM_DIRS)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include")
    SET (EXTRA_LD_FLAGS "-L/usr/lib")
endif()

set(ASIO_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/deps/asio/asio/include)
add_definitions(-DASIO_STANDALONE)

set (GLOG_LD_FLAGS "-lglog")

set (cppmetrics_global_EXTERNAL_HEADERS
  src/cppmetrics/cppmetrics.h
)

set (cppmetrics_SOURCES
  src/cppmetrics/core/histogram.cpp
  src/cppmetrics/core/meter.cpp
  src/cppmetrics/core/metric_registry.cpp
  src/cppmetrics/core/scheduled_reporter.cpp
  src/cppmetrics/core/timer_context.cpp
  src/cppmetrics/core/timer.cpp
  src/cppmetrics/concurrent/simple_thread_pool_executor.cpp
  src/cppmetrics/concurrent/simple_scheduled_thread_pool_executor.cpp
  src/cppmetrics/core/ewma.cpp
  src/cppmetrics/core/exp_decay_sample.cpp
  src/cppmetrics/core/snapshot.cpp
  src/cppmetrics/core/uniform_sample.cpp
  src/cppmetrics/graphite/graphite_sender_tcp.cpp
  src/cppmetrics/graphite/graphite_reporter.cpp
  src/cppmetrics/core/console_reporter.cpp
  src/cppmetrics/graphite/graphite_sender_udp.cpp
)

set (cppmetrics_core_EXTERNAL_HEADERS
  src/cppmetrics/core/counter.h
  src/cppmetrics/core/histogram.h
  src/cppmetrics/core/gauge.h
  src/cppmetrics/core/meter.h
  src/cppmetrics/core/metered.h
  src/cppmetrics/core/metric.h
  src/cppmetrics/core/metric_registry.h
  src/cppmetrics/core/reporter.h
  src/cppmetrics/core/scheduled_reporter.h
  src/cppmetrics/core/timer.h
  src/cppmetrics/core/timer_context.h
  src/cppmetrics/core/exp_decay_sample.h
  src/cppmetrics/core/sample.h
  src/cppmetrics/core/snapshot.h
  src/cppmetrics/core/uniform_sample.h
  src/cppmetrics/core/sampling.h
  src/cppmetrics/core/types.h
  src/cppmetrics/core/trivial_gauge.h
  src/cppmetrics/core/console_reporter.h
 )

set (cppmetrics_graphite_EXTERNAL_HEADERS
  src/cppmetrics/graphite/graphite_sender.h
  src/cppmetrics/graphite/graphite_sender_tcp.h
  src/cppmetrics/graphite/graphite_reporter.h
  src/cppmetrics/graphite/graphite_sender_udp.h
)

set (cppmetrics_concurrent_EXTERNAL_HEADERS
  src/cppmetrics/concurrent/simple_thread_pool_executor.h
  src/cppmetrics/concurrent/simple_scheduled_thread_pool_executor.h
)

set (cppmetrics_HEADERS
  src/cppmetrics/core/ewma.h
  ${cppmetrics_core_EXTERNAL_HEADERS}
  ${cppmetrics_concurrent_EXTERNAL_HEADERS}
  ${cppmetrics_graphite_EXTERNAL_HEADERS}
)

set (DIST_INCLUDE_DIR include/cppmetrics)


if(DEFINED GLOG_DIR)
    set (GLOG_INCLUDE_DIR ${GLOG_DIR}/include)
    set (GLOG_LIB_DIR ${GLOG_DIR}/lib)
endif()

include_directories (src ${ASIO_INCLUDE_DIRS} ${GLOG_INCLUDE_DIR} ${GTEST_DIR})
link_directories (${GLOG_LIB_DIR} ${GTEST_DIR})
install (FILES ${cppmetrics_global_EXTERNAL_HEADERS} DESTINATION ${DIST_INCLUDE_DIR})
install (FILES ${cppmetrics_core_EXTERNAL_HEADERS} DESTINATION ${DIST_INCLUDE_DIR}/core)
install (FILES ${cppmetrics_concurrent_EXTERNAL_HEADERS} DESTINATION ${DIST_INCLUDE_DIR}/concurrent)
install (FILES ${cppmetrics_graphite_EXTERNAL_HEADERS} DESTINATION ${DIST_INCLUDE_DIR}/graphite)

# Static library
add_library (cppmetrics STATIC ${cppmetrics_HEADERS} ${cppmetrics_SOURCES})
install (TARGETS cppmetrics DESTINATION lib)

# Shared library
add_library (cppmetricsshared SHARED ${cppmetrics_HEADERS} ${cppmetrics_SOURCES})
target_link_libraries (cppmetricsshared ${GLOG_LD_FLAGS})
install (TARGETS cppmetricsshared DESTINATION lib)

# Examples
add_executable(graphite_client examples/graphite_client.cpp)
target_link_libraries(graphite_client cppmetrics ${GLOG_LD_FLAGS} Threads::Threads)
add_executable(console_client examples/console_client.cpp)
target_link_libraries(console_client cppmetrics ${GLOG_LD_FLAGS} Threads::Threads)



# Code formatting
add_custom_target(clang-format
    COMMAND docker run --rm -v ${PWD}:/root/sources onedata/clang-format-check:1.1
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(cppmetrics_core_TEST_SOURCES
  test/cppmetrics/core/test_counter.cpp
  test/cppmetrics/core/test_ewma.cpp
  test/cppmetrics/core/test_exp_decay_sample.cpp
  test/cppmetrics/core/test_histogram.cpp
  test/cppmetrics/core/test_meter.cpp
  test/cppmetrics/core/test_metric_registry.cpp
  test/cppmetrics/core/test_scheduled_reporter.cpp
  test/cppmetrics/core/test_snapshot.cpp
  test/cppmetrics/core/test_timer.cpp
  test/cppmetrics/core/test_uniform_sample.cpp
  test/cppmetrics/core/test_console_reporter.cpp
)

set(cppmetrics_graphite_TEST_SOURCES
  test/cppmetrics/graphite/test_graphite_sender_tcp.cpp
  test/cppmetrics/graphite/test_graphite_reporter.cpp
)

set(cppmetrics_concurrent_TEST_SOURCES
  test/cppmetrics/concurrent/test_simple_thread_pool_executor.cpp
  test/cppmetrics/concurrent/test_simple_scheduled_thread_pool_executor.cpp
)

set(cppmetrics_TEST_SOURCES
    ${cppmetrics_core_TEST_SOURCES}
    ${cppmetrics_concurrent_TEST_SOURCES}
    ${cppmetrics_graphite_TEST_SOURCES}
)

add_executable(gtest_cppmetrics EXCLUDE_FROM_ALL ${cppmetrics_TEST_SOURCES})
target_link_libraries (gtest_cppmetrics cppmetrics ${GTEST_BOTH_LIBRARIES} ${GLOG_LD_FLAGS} Threads::Threads)
add_custom_target(gtest gtest_cppmetrics --gtest_output=xml DEPENDS gtest_cppmetrics)
