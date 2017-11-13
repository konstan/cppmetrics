#include <cppmetrics/cppmetrics.h>
#include <glog/logging.h>
#include <iostream>
#include <random>

using namespace cppmetrics;

class RandomGauge : public core::Gauge {
public:
    RandomGauge()
    {
        eng = std::mt19937(std::random_device()());
        distr = std::uniform_int_distribution<>(0, 50);
    }
    virtual int64_t getValue() { return distr(eng); }

private:
    std::mt19937 eng;
    std::uniform_int_distribution<> distr;
};

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <duration>\n";
        return 1;
    }

    int duration{std::stoi(argv[1])};

    std::shared_ptr<core::ConsoleReporter> console_reporter;
    core::MetricRegistryPtr metric_registry(new core::MetricRegistry());

    core::GaugePtr gauge_ptr(new RandomGauge());
    metric_registry->addGauge("RANDOM_GAUGE", gauge_ptr);

    console_reporter.reset(
        new core::ConsoleReporter(metric_registry, std::cout));

    console_reporter->start(std::chrono::seconds(1));

    LOG(ERROR) << "STARTING WAITING...";

    std::this_thread::sleep_for(std::chrono::seconds(duration));

    LOG(ERROR) << "FINISHED WAITING...";

    console_reporter->stop();
}
