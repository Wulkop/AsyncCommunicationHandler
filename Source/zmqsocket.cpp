/*
 * zmqsocket.cpp
 *
 *  Created on: 06.05.2017
 *      Author: jan
 */

#include "include/zmqsocket.h"

ZmqSocket::ZmqSocket() {
	m_pSocket = NULL;
	m_pContext = NULL;
	initialized = false;

}
ZmqSocket::~ZmqSocket() {
	// TODO Auto-generated destructor stub
}
void ZmqSocket::open(const char * ports, int flag)
{
	m_pContext=new zmq::context_t(1);
	m_pSocket=new zmq::socket_t(*m_pContext,flag);
	m_pSocket->bind(ports);
	initialized=true;
}
void ZmqSocket::connect(const char *toAddr, int flag)
{
	m_pContext=new zmq::context_t(1);
	m_pSocket=new zmq::socket_t(*m_pContext,flag);
	m_pSocket->connect(toAddr);
	initialized=true;
}
void ZmqSocket::send(const char * str, int size)
{
	m_pSocket->send(str, size,0);
}
std::string ZmqSocket::receive()
{
	zmq::message_t mesg;
	char buffer[MAX_MASSAGE_SIZE];
	int actualSize=m_pSocket->recv(buffer,MAX_MASSAGE_SIZE,0);
	std::string buf=std::string(buffer,actualSize);
	return buf;
}
void ZmqSocket::setSocketOptions(int val, const void * optval, size_t optvallen)
{
	m_pSocket->setsockopt(val,optval,optvallen);//No filter
}

