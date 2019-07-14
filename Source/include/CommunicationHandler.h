/*
 *
 *  Created on: 06.05.2017
 *      Author: jan
 */
#pragma once

#include <thread>
#include <mutex>
#include "ZmqSocket.h"
#include "AbstractFifoClass.h"

struct ThreadArgs
{
	void * thisPtr;
	void * fifoClassPtr;
};

class CommunicationHandler {
public:
	/**
	 * Constrcutor for IpcHandler. Takes a pointer to an object of AbstractFifoClass. This object will be used to store the received messages in a queue.
	 */
	CommunicationHandler(AbstractFifoClass * fifoClass);
	virtual ~CommunicationHandler();
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
	AbstractFifoClass * m_FifoClass;

	ZmqSocket * m_SubSocket;
	ZmqSocket * m_PubSocket;

    std::mutex m_StopMutex;
	std::thread m_Thread;
	bool m_StopIssued;
	ThreadArgs * m_ThreadArgs;
};

