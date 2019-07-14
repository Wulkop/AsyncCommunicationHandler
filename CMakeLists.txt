CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0)

project(CommunicationHandler)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


SET(ZeroMQ_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/libzmq/include)
SET(CppZMQ_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/cppzmq)
SET(CommunicationHandler_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source/include)

if(NOT TARGET CommunicationHandler)
	ADD_SUBDIRECTORY(Dependencies)

    	add_library(Networking STATIC
				Source/ZmqSocket.cpp
				Source/CommunicationHandler.cpp
                    )
	target_link_libraries(Networking
				${PTHREAD_LIBRARY}
				libzmq-static)
	target_include_directories(Networking
			   PUBLIC
			   ${ZeroMQ_INCLUDE_DIR}
			   ${CppZMQ_INCLUDE_DIR}
			   ${CommunicationHandler_INCLUDE_DIR}
			   )
endif()
