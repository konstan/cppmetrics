/*
 * TeeSender.h
 *
 *  Created on: Jan 15, 2017
 *      Author: noam
 */

#ifndef SRC_CPPMETRICS_GRAPHITE_TEESENDER_H_
#define SRC_CPPMETRICS_GRAPHITE_TEESENDER_H_


/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * graphite_sender_tcp.h
 *
 *  Created on: Jun 16, 2014
 *      Author: vpoliboy
 */

#ifndef TEE_SENDER_TCP_H_
#define TEE_SENDER_TCP_H_

#include "Poco/Net/StreamSocket.h"
#include "cppmetrics/graphite/graphite_sender.h"

namespace cppmetrics {
namespace graphite {

/**
 * Send the same metrics to two GraphiteSender instances.
 * This is needed as a stopgap solution for working with both DD and Graphite
 */
class TeeSender: public GraphiteSender {
public:

    /**
     * Creates a new sender with the given params.
     * @param host The graphite server host.
     * @param port The graphite server port.
     */
	TeeSender(GraphiteSender* a, GraphiteSender*b){
		sender_a = a;
		sender_b = b;
	}

    virtual ~TeeSender(){}

    /**
     * Connects to the graphite sender over TCP.
     * @return True on success, false otherwise.
     * @throws std::runtime_error if there is a problem.
     */
    virtual void connect(){
    	sender_a->connect();
    	sender_b->connect();
    }

    /**
     * Posts the metric name, value and timestamp to the graphite server.
     * @param name The name of the metric
     * @param value The value of the metric
     * @param timestamp The timestamp of the metric.
     * @return True on success false otherwise.
     * @throws std::runtime_error if there is a problem.
     */
    virtual void send(const std::string& name,
            const std::string& value,
            uint64_t timestamp){
    	sender_a->send(name, value, timestamp);
    	sender_b->send(name, value, timestamp);
    }
    /**
     * Closes the connection.
     */
    virtual void close(){
    	sender_a->close();
    	sender_b->close();
    }

private:
    GraphiteSender* sender_a,* sender_b;
};

} /* namespace graphite */
} /* namespace cppmetrics */
#endif /* GRAPHITE_SENDER_TCP_H_ */



#endif /* SRC_CPPMETRICS_GRAPHITE_TEESENDER_H_ */
