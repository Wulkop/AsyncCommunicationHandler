CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

project(CommunicationHandler)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


SET(ZeroMQ_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/libzmq/include)
SET(CppZMQ_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/cppzmq)
SET(CommunicationHandler_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source/include)

if(NOT TARGET Networking)
	ADD_SUBDIRECTORY(Dependencies)

	include_directories(
				${ZeroMQ_INCLUDE_DIR}
				${CppZMQ_INCLUDE_DIR}
				${CommunicationHandler_INCLUDE_DIR}
				)
    add_library(Networking STATIC
                    Source/zmqsocket.cpp
                    Source/ipccommunicationhandler.cpp
                    )
	target_link_libraries(Networking 
					${PTHREAD_LIBRARY}
					libzmq)
    add_dependencies(Networking libzmq)
	if(WIN32)
	if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
		find_library(PTHREAD_LIBRARY pthread HINTS "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/pthreads/lib/x64")
	else( CMAKE_SIZEOF_VOID_P EQUAL 8 )
		find_library(PTHREAD_LIBRARY pthread HINTS "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/pthreads/lib/x86")
	endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )
		include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/pthreads/include")
	endif(WIN32)
	if(UNIX)
	target_link_libraries(Networking
		pthread
		)
	endif(UNIX)
endif()
