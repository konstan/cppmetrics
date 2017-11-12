#include <cppmetrics/cppmetrics.h>
#include <glog/logging.h>
#include <random>
#include <iostream>

using namespace cppmetrics;

class RandomGauge : public core::Gauge {
public:
    RandomGauge() {
        eng = std::mt19937(std::random_device());
        distr = std::uniform_int_distribution<>(0, 50);
    }
    virtual int64_t getValue() {
        return distr(eng);
    }

private:
    std::mt19937 eng;
    std::uniform_int_distribution<> distr;
};

int main(int argc, char **argv)
{
    if(argc != 4) {
        std::cout << "Usage: " << argv[0] << " <graphite_hostname> <graphite_port> <duration>\n";
    }

    std::string graphite_hostname{argv[1]};
    int graphite_port{std::stoi(argv[2])};
    int duration{std::stoi(argv[3])};

    std::unique_ptr<cppmetrics::graphite::GraphiteReporter> graphite_reporter_;
    core::MetricRegistryPtr metric_registry(new core::MetricRegistry());

    core::GaugePtr gauge_ptr(new RandomGauge());
    metric_registry->addGauge("RANDOM_GAUGE", gauge_ptr);

    graphite::GraphiteSenderPtr graphite_sender(
        new graphite::GraphiteSenderTCP(graphite_hostname, graphite_port));

    graphite_reporter_.reset(
        new graphite::GraphiteReporter(metric_registry,
            graphite_sender, "cppmetrics_test"));

    graphite_reporter_->start(std::chrono::seconds(1));
    std::this_thread::sleep_for(std::chrono::seconds(duration));
}
