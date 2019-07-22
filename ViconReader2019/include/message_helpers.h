#ifndef __MESSAGE_HELPERS_H_ZMQ__
#define __MESSAGE_HELPERS_H_ZMQ__

#include <zmq.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

class context_helper {
public:
	context_helper();				/// creates a ZeroMQ context, which is passed to pub/sub objects
	~context_helper();			/// destructor
	void* get_context();		/// returns a pointer to the context
private:
	void* context;					/// Void pointer used for context variables by ZeroMQ. This pointer is needed when sockets are initialized.
};

class publisher_helper {
public:
	publisher_helper(context_helper* context, const char* bindString, bool verboseFlag=false);
	~publisher_helper();
	// publish string
	int send(const char*);
	// debugging variable
	int bind_status;
private:
	void* publisher;
	context_helper* contextpt;
	bool verbose;
};

class subscriber_helper {
public:
	/** Object-based wrapper for ZMQ subscriber socket. Subscribes to all messages. Only the timeout
	 *  socket option can be set in this functionality. All other messaging options are left at defaults.
	 */
	subscriber_helper(context_helper* context, const char* bindString, bool verbose=false, int timeout_ms=250);
	~subscriber_helper();									/// Destructor
	int recv();														/// Receive message from publisher
	int bind_status;											/// Integer that contains the last value returned by zmq_setsockoptions or zmq_connect
	void get_msg(char*buffercopy);				/// Return a copy of the message buffer
private:
	void* subscriber;						/// Void pointer to the ZMQ subscriber socket
	context_helper* contextpt;	/// Pointer to the common context object used by the main program
	bool verbose;								/// Flag that controls behavior of real-time printing
	char buffer[1024];					/// Static buffer into which messages are read
};

#endif
