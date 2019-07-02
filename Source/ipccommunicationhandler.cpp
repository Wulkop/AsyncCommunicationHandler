/*
 * ipccommunicationhandler.cpp
 *
 *  Created on: 06.05.2017
 *      Author: jan
 */

#include "include/ipccommunicationhandler.h"

IpcCommunicationHandler::IpcCommunicationHandler(AbstractFifoClass * fifoClass) {
	subSocket = new ZmqSocket;
	pubSocket = new ZmqSocket;
	stopIssued = false;
	this->fifoClass = fifoClass;
	pthread_mutex_init(&stopMutex, NULL);
	args = new ThreadArgs;
	args->thisPtr = this;
	args->fifoClassPtr = fifoClass;
}

IpcCommunicationHandler::~IpcCommunicationHandler() {
	delete subSocket;
	delete pubSocket;
	delete args;
	pthread_cancel(thread);
}
void IpcCommunicationHandler::setSocketServer()
{
	pubSocket->open("tcp://*:55556", ZMQ_PUB);

	subSocket->open("tcp://*:55555", ZMQ_SUB);
	subSocket->setSocketOptions(ZMQ_SUBSCRIBE,"",0);
}
void IpcCommunicationHandler::setSocketClient(std::string ip)
{
	pubSocket->connect(("tcp://"+ ip + ":55555").c_str(), ZMQ_PUB);

	subSocket->connect(("tcp://" + ip + ":55556").c_str(), ZMQ_SUB);
	subSocket->setSocketOptions(ZMQ_SUBSCRIBE,"",0);
}
void IpcCommunicationHandler::start()
{
	pthread_create(&thread, NULL, receive, args);
}
void IpcCommunicationHandler::stop()
{
	pthread_mutex_lock(&stopMutex);
	stopIssued = true;
	pthread_mutex_unlock(&stopMutex);
}
bool IpcCommunicationHandler::isStopIssued()
{
	bool result = false;
	pthread_mutex_lock(&stopMutex);
	result = stopIssued;
	pthread_mutex_unlock(&stopMutex);
	return result;

}
void * IpcCommunicationHandler::receive(void * structPtr)
{
	ThreadArgs * args = (ThreadArgs *) structPtr;
	IpcCommunicationHandler * handler = (IpcCommunicationHandler *) args->thisPtr;
	AbstractFifoClass * fifoClass = (AbstractFifoClass *) args->fifoClassPtr;
	while(!handler->isStopIssued())
	{
		if(handler->subSocket->initialized)
		{
			std::string buffer;
			std::cout<<"Ready to receive"<<std::endl;
			buffer = handler->subSocket->receive();
			std::cout<<"Got: "<<buffer<<std::endl;
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
	if(pubSocket->initialized)
	{
		pubSocket->send(message.c_str(), message.size());
	}
}
