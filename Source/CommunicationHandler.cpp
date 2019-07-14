/*
 * ipccommunicationhandler.cpp
 *
 *  Created on: 06.05.2017
 *      Author: jan
 */

#include "CommunicationHandler.h"

CommunicationHandler::CommunicationHandler(AbstractFifoClass * fifoClass): m_SubSocket(new ZmqSocket), m_PubSocket(new ZmqSocket),
m_StopIssued(false), m_FifoClass(fifoClass), m_StopMutex(), m_ThreadArgs(new ThreadArgs)
{

    m_ThreadArgs->thisPtr = this;
    m_ThreadArgs->fifoClassPtr = fifoClass;
}

CommunicationHandler::~CommunicationHandler() {
    stop();
    delete m_SubSocket;
	delete m_PubSocket;
	delete m_ThreadArgs;

	//pthread_cancel(thread);
}
void CommunicationHandler::setSocketServer()
{
	m_PubSocket->open("tcp://*:55556", ZMQ_PUB);

	m_SubSocket->open("tcp://*:55555", ZMQ_SUB);
	m_SubSocket->setSocketOptions(ZMQ_SUBSCRIBE,"",0);
}
void CommunicationHandler::setSocketClient(std::string ip)
{
	m_PubSocket->connect(("tcp://"+ ip + ":55555").c_str(), ZMQ_PUB);

	m_SubSocket->connect(("tcp://" + ip + ":55556").c_str(), ZMQ_SUB);
	m_SubSocket->setSocketOptions(ZMQ_SUBSCRIBE,"",0);
}
void CommunicationHandler::start()
{
    m_Thread = std::thread(receive, m_ThreadArgs);
}
void CommunicationHandler::stop()
{
    m_StopMutex.lock();
	m_StopIssued = true;
	m_StopMutex.unlock();
}
bool CommunicationHandler::isStopIssued()
{
	bool result = false;
	m_StopMutex.lock();
	result = m_StopIssued;
	m_StopMutex.unlock();
	return result;

}
void * CommunicationHandler::receive(void * structPtr)
{
	ThreadArgs * args = (ThreadArgs *) structPtr;
	CommunicationHandler * handler = (CommunicationHandler *) args->thisPtr;
	AbstractFifoClass * fifoClass = (AbstractFifoClass *) args->fifoClassPtr;
	while(!handler->isStopIssued())
	{
		if(handler->m_SubSocket->initialized)
		{
			zmq::message_t message;
			std::string buffer;
			buffer = handler->m_SubSocket->receive();
			fifoClass->pushFifo(buffer);
		}
		else
		{
			std::cout<<"Socket not initialized"<<std::endl;
		}
	}
	return NULL;
}
void CommunicationHandler::send(std::string message)
{
	if(m_PubSocket->initialized)
	{
		m_PubSocket->send(message.c_str(), message.size());
	}
}
