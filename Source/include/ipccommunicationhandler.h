/*
 * ipccommunicationhandler.h
 *
 *  Created on: 06.05.2017
 *      Author: jan
 */

#ifndef SOURCE_INCLUDE_IPCCOMMUNICATIONHANDLER_H_
#define SOURCE_INCLUDE_IPCCOMMUNICATIONHANDLER_H_

#include <pthread.h>
#include "zmqsocket.h"
#include "abstractfifoclass.h"

class XWindowManager;

struct ThreadArgs
{
	void * thisPtr;
	void * fifoClassPtr;
};

class IpcCommunicationHandler {
public:
	/**
	 * Constrcutor for IpcHandler. Takes a pointer to an object of AbstractFifoClass. This object will be used to store the received messages in a queue.
	 */
	IpcCommunicationHandler(AbstractFifoClass * fifoClass);
	virtual ~IpcCommunicationHandler();
	/**
	 * Set socket options for server usage.
	 */
	void setSocketServer();
	/**
	 * Set socket options for client usage.
	 */
	void setSocketClient(std::string ip);
	/**
	 * Send a message
	 */
	void send(std::string message);
	/**
	 * Start receive thread
	 */
	void start();
	/**
	 * Stop receive thread
	 */
	void stop();
private:
	/**
	 * Main receive loop. Sice this methon is blocking, this should run in an own thread
	 */
	static void * receive(void * structPtr);
	/**
	 * Threadsafe getter method to check if a thread stop is requested
	 */
	bool isStopIssued();
	AbstractFifoClass * fifoClass;
	pthread_mutex_t stopMutex;
	ZmqSocket * subSocket;
	ZmqSocket * pubSocket;
	pthread_t thread;
	bool stopIssued;
	ThreadArgs * args;
};

#endif /* SOURCE_INCLUDE_IPCCOMMUNICATIONHANDLER_H_ */
