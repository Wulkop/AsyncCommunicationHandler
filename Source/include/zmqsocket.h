/*
 * zmqsocket.h
 *
 *  Created on: 06.05.2017
 *      Author: jan
 */

#ifndef SOURCE_INCLUDE_ZMQSOCKET_H_
#define SOURCE_INCLUDE_ZMQSOCKET_H_
#define MAX_MASSAGE_SIZE 20
#include <zmq.hpp>


class ZmqSocket {
public:
	/**
	*Initialize socket and context
	*/
	ZmqSocket();
	virtual ~ZmqSocket();
	/**
	*Defines context and socket and open ports(Server Socket)
	*/
	void open(const char* Ports, int flag);
	/**
	*Defines context and socket und connect to server
	*/
	void connect(const char* toAddr, int flag);
	/**
	*Send the given String
	*/
	void send(const char* str,int size);
	/**
	*Receive a string
	*/
	std::string receive();
	/**
	*Setup the subscriber socket
	*/
	void setSocketOptions(int val, const void* optval, size_t optvallen);
	/**
	*Is Socket already initialized?
	*/
	bool initialized;
private:
	zmq::context_t* m_pContext;
	zmq::socket_t*  m_pSocket;
};

#endif /* SOURCE_INCLUDE_ZMQSOCKET_H_ */
