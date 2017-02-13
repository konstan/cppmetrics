//
// Created by noam on 2017-02-12
//

#ifndef MEDIASERVER_TRIVIAL_GAUGE_H_H
#define MEDIASERVER_TRIVIAL_GAUGE_H_H

#include "gauge.h"

namespace cppmetrics {
namespace core {

class TrivialGauge : public Gauge {
public:
    virtual boost::int64_t getValue() { return value_; }
    void setValue(boost::int64_t v) { value_ = v;}
    boost::int64_t value_;
};

typedef boost::shared_ptr<TrivialGauge> TrivialGaugePtr;
}}
#endif //MEDIASERVER_TRIVIAL_GAUGE_H_H
