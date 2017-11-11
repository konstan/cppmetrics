/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * test_graphite_sender_tcp.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: vpoliboy
 */

#include "cppmetrics/graphite/graphite_sender_tcp.h"
#include <gtest/gtest.h>

namespace cppmetrics {
namespace graphite {

TEST(graphiteSenderTCP, connectionFailuresTest)
{
    GraphiteSenderTCP graphite_sender_tcp("localhost", 3483);
    ASSERT_THROW(graphite_sender_tcp.connect(), std::runtime_error);
}
}
}
