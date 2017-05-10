CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

project(CommunicationHandler)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


SET(ZeroMQ_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/libzmq/include CACHE INTERNAL "ZeroMq Include Dir")
SET(CppZMQ_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/cppzmq CACHE INTERNAL "CppZMQ Include Dir")
SET(CommunicationHandler_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source/include CACHE INTERNAL "CommunicationHanlder Include Dir")

if(NOT TARGET Networking)
	message(${CppZMQ_INCLUDE_DIR})
	ADD_SUBDIRECTORY(Dependencies)

	include_directories(
				${ZeroMQ_INCLUDE_DIR}
				${CppZMQ_INCLUDE_DIR}
				${CommunicationHandler_INCLUDE_DIR}

				)
        add_library(Networking SHARED
                        Source/zmqsocket.cpp
                        Source/ipccommunicationhandler.cpp
                        )
        target_link_libraries (Networking 
                        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libzmq.so
                        )
        add_dependencies(Networking libzmq)
endif()
