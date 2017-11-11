/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * test_simple_scheduled_thread_pool_executor.cpp
 *
 *  Created on: Jun 11, 2014
 *      Author: vpoliboy
 */

#include "cppmetrics/concurrent/simple_scheduled_thread_pool_executor.h"
#include <chrono>
#include <glog/logging.h>
#include <gtest/gtest.h>
#include <thread>

namespace cppmetrics {
namespace concurrent {

TEST(simplescheduledthreadpoolexecutor, fixedDelayTest)
{
    SimpleScheduledThreadPoolExecutor sstpe(3);

    ASSERT_FALSE(sstpe.isShutdown());

    std::atomic<size_t> counter{0UL};

    auto timer_task = [&]() {
        ++counter;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };

    sstpe.scheduleAtFixedDelay(timer_task, std::chrono::milliseconds(100));
    ASSERT_FALSE(sstpe.isShutdown());

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // As there is a sleep of 100ms in the timertask, it gets invoked only at
    // every 200ms.
    ASSERT_LE((size_t)4, counter);
    ASSERT_GE((size_t)6, counter);

    ASSERT_FALSE(sstpe.isShutdown());
    sstpe.shutdownNow();
    ASSERT_TRUE(sstpe.isShutdown());
}

TEST(simplescheduledthreadpoolexecutor, fixedRateTest)
{
    SimpleScheduledThreadPoolExecutor sstpe(3);

    ASSERT_FALSE(sstpe.isShutdown());

    std::atomic<size_t> counter{0UL};

    auto timer_task = [&]() {
        ++counter;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };

    sstpe.scheduleAtFixedRate(timer_task, std::chrono::milliseconds(100));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    ASSERT_LE((size_t)9, counter);
    ASSERT_GE((size_t)10, counter);

    ASSERT_FALSE(sstpe.isShutdown());
    sstpe.shutdownNow();
    ASSERT_TRUE(sstpe.isShutdown());
}
}
}
