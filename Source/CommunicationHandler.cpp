/*
 * ipccommunicationhandler.cpp
 *
 *  Created on: 06.05.2017
 *      Author: jan
 */

#include "CommunicationHandler.h"

IpcCommunicationHandler::IpcCommunicationHandler(AbstractFifoClass * fifoClass): m_SubSocket(new ZmqSocket), m_PubSocket(new ZmqSocket),
m_StopIssued(false), m_FifoClass(fifoClass), m_StopMutex(), m_ThreadArgs(new ThreadArgs)
{

    m_ThreadArgs->thisPtr = this;
    m_ThreadArgs->fifoClassPtr = fifoClass;
}

IpcCommunicationHandler::~IpcCommunicationHandler() {
    stop();
    delete m_SubSocket;
	delete m_PubSocket;
	delete m_ThreadArgs;

	//pthread_cancel(thread);
}
void IpcCommunicationHandler::setSocketServer()
{
	m_PubSocket->open("tcp://*:55556", ZMQ_PUB);

	m_SubSocket->open("tcp://*:55555", ZMQ_SUB);
	m_SubSocket->setSocketOptions(ZMQ_SUBSCRIBE,"",0);
}
void IpcCommunicationHandler::setSocketClient(std::string ip)
{
	m_PubSocket->connect(("tcp://"+ ip + ":55555").c_str(), ZMQ_PUB);

	m_SubSocket->connect(("tcp://" + ip + ":55556").c_str(), ZMQ_SUB);
	m_SubSocket->setSocketOptions(ZMQ_SUBSCRIBE,"",0);
}
void IpcCommunicationHandler::start()
{
    m_Thread = std::thread(receive, m_ThreadArgs);
}
void IpcCommunicationHandler::stop()
{
    m_StopMutex.lock();
	m_StopIssued = true;
	m_StopMutex.unlock();
}
bool IpcCommunicationHandler::isStopIssued()
{
	bool result = false;
	m_StopMutex.lock();
	result = m_StopIssued;
	m_StopMutex.unlock();
	return result;

}
void * IpcCommunicationHandler::receive(void * structPtr)
{
	ThreadArgs * args = (ThreadArgs *) structPtr;
	IpcCommunicationHandler * handler = (IpcCommunicationHandler *) args->thisPtr;
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
void IpcCommunicationHandler::send(std::string message)
{
	if(m_PubSocket->initialized)
	{
		m_PubSocket->send(message.c_str(), message.size());
	}
}
