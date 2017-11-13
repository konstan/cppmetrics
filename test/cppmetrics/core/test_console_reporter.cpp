/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * test_console_reporter.cpp
 *
 *  Created on: Jul 2, 2014
 *      Author: vpoliboy
 */

#include "cppmetrics/core/console_reporter.h"
#include <chrono>
#include <gtest/gtest.h>
#include <random>
#include <thread>

namespace cppmetrics {
namespace core {

namespace {

class TestGauge : public Gauge {
public:
    virtual int64_t getValue() { return 100; }
};
}

TEST(consolereporter, gaugetest)
{

    MetricRegistryPtr metric_registry(new MetricRegistry());
    metric_registry->addGauge("new.gauge", GaugePtr(new TestGauge()));
    std::shared_ptr<ConsoleReporter> console_reporter(
        new ConsoleReporter(metric_registry, std::cout));
    console_reporter->start(std::chrono::milliseconds(1000));
    std::this_thread::sleep_for(std::chrono::milliseconds(10 * 1000));
}

TEST(consolereporter, timerContextTest)
{

    MetricRegistryPtr metric_registry(new MetricRegistry());
    std::mt19937 rng;
    std::shared_ptr<ConsoleReporter> console_reporter(
        new ConsoleReporter(metric_registry, std::cout));
    console_reporter->start(std::chrono::milliseconds(1000));
    for (size_t i = 0; i < 100; ++i) {
        std::uniform_int_distribution<> uniform(10, 30);
        size_t sleep_time = uniform(rng);
        TimerContextPtr time_context(
            metric_registry->timer("test.timer")->timerContextPtr());
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10 * 1000));
}
}
}
