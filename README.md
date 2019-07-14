# AsyncCommunicationHandler
This repository provides a simple static library which allows an application to setup a simple 1-Host, n-Client TCP communication via ZeroMQ

## What is this library and why do I need it?

This library can be used in an application where multiple parts (processes or even threads) should communicate with each other.
I've noticed that there are many applications where we have a central component which receives messages from other components and answers via a broadcast.
I try to make setting up these kind of network architecture easier with this library.

# How can I build this library
The easiest way to build this library is using CMake (at least version 3.1.0).
1. Check if you cloned this repository with all dependencies `git submodule update --init --recursive`
2. Create a build folder `mkdir build`
3. Navigate to that folder `cd build`
4. Call CMake `cmake PATH_TO_ROOT_OF_THIS_REPO`
5. Either call `make` (on UNIX) or open the Visual Studio Solution (on Windows)

# How do I use it in my application?

In order to transmit messages between two components we need at least one host, which opens the ports (Default TCP ports 55555 and 55556).
The other components will connect to these ports and can now send to or receive messages from the host.
Each message the host sends will be broadcasted to all clients.
Each message the client sends will only be received by the host.

In order to use this library inside your application you need to implement your own class based on the `AbstractFifoClass`.
This abstract class already implements methods for a thread-safe Fifo container holding the incoming messages.
In your inherited class you just need to implement the method `checkFifo`.
You can either implement this as an empty method or you can use it to process the incoming messages.
Since the class is thread-safe it doesn't matter in which thread this method is called.

After you've instaniated an object of this class you can pass is as an argument to the CommunicationHandler constructor.
Before you call the method `start` you need to specify if this handler should act as a host or a client.
You can do this via the methods `setSocketClient` or `setSocketServer`.
The moment you call `start` a new thread will be created and waits for incoming messages.

# External Dependencies
* [ZeroMQ](https://github.com/zeromq/libzmq) : LGPL License
* [CppZMQ](https://github.com/zeromq/cppzmq): MIT License
