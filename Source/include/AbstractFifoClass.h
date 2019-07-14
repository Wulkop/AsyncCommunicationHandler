/*
 * abstractfifoclass.h
 *
 *  Created on: May 8, 2017
 *      Author: jan
 */

#ifndef SOURCE_INCLUDE_ABSTRACTFIFOCLASS_H_
#define SOURCE_INCLUDE_ABSTRACTFIFOCLASS_H_
#include <iostream>
#include <queue>
#include <pthread.h>


class AbstractFifoClass {
public:
	void initFifo()
	{
		pthread_mutex_init(&fifoMutex, NULL);
	}
	std::string popFifo()
	{
		pthread_mutex_lock(&fifoMutex);
		std::string element = fifo.front();
		fifo.pop();
		pthread_mutex_unlock(&fifoMutex);
		return element;
	}
	int getFifoSize()
	{
		int size = 0;
		pthread_mutex_lock(&fifoMutex);
		size = fifo.size();
		pthread_mutex_unlock(&fifoMutex);
		return size;
	}
	void pushFifo(std::string element)
	{
		pthread_mutex_lock(&fifoMutex);
		fifo.push(element);
		pthread_mutex_unlock(&fifoMutex);
	}
	virtual void checkFifo() = 0;
private:
	pthread_mutex_t fifoMutex;
	std::queue<std::string> fifo;
};

#endif /* SOURCE_INCLUDE_ABSTRACTFIFOCLASS_H_ */
