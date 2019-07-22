#include "message_helpers.h"

context_helper::context_helper() {
	context = zmq_ctx_new();
}

context_helper::~context_helper() {
	// destroy the context
	zmq_ctx_destroy(context);
}

void* context_helper::get_context() {
	return context;
}

publisher_helper::publisher_helper(context_helper* context, const char* bindString, bool verboseFlag) {
	// save the pointer to context
	contextpt = context;
	// set verbose flag
	verbose = verboseFlag;
	// create publisher
	publisher = zmq_socket(contextpt->get_context(), ZMQ_PUB);
	if (verbose){
		std::cout << "Created publisher\n";
	}
	// bind publisher to socket
	bind_status = zmq_bind(publisher, bindString);
	if (verbose) {
		std::cout << "Bound publisher to socket: " << bindString << "\nBind return flag: " << bind_status << std::endl;
	}
}
publisher_helper::~publisher_helper() {
	// destroy publisher
	zmq_close(publisher);

	if (verbose) {
		std::cout << "Closed publisher\n";
	}
}

int publisher_helper::send(const char* strin) {
	int size = zmq_send(publisher,strin,strlen(strin), 0);
	if (verbose) {
		std::cout << "Published " << size << " bytes of string: " << strin << "\n";
	}
	return size;
}

subscriber_helper::subscriber_helper(context_helper* context, const char* bindString, bool verboseFlag, int timeout_ms) {
	verbose = verboseFlag;
	contextpt = context;
	subscriber = zmq_socket(contextpt->get_context(), ZMQ_SUB);
	bind_status = zmq_connect(subscriber, bindString);
	if (verbose)
		printf("zmq_connect returned: %d\n", bind_status);

	//  Subscribe to every message
	bind_status = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);
	if (verbose)
		printf("zmq_setsockopt returned: %d\n", bind_status);
	// add a timeout
	int timeout = timeout_ms;
	bind_status = zmq_setsockopt(subscriber, ZMQ_RCVTIMEO, &timeout, sizeof(int));
	if (verbose)
		std::cout << "Added timeout filter, return flag " << bind_status << std::endl;
}

subscriber_helper::~subscriber_helper() {
	zmq_close(subscriber);
	if (verbose)
		std::cout << "Closed subscriber\n";
}

int subscriber_helper::recv() {
	// read into buffer
	int size = zmq_recv(subscriber, buffer, 1023, 0);
	// check that bytes were received
	if (size!=-1) {
		buffer[size] = '\0';
		if (verbose) {
			printf("%s",buffer);
			printf("\n");
		}
	}

	if (verbose)
		printf("%d bytes\n", size);
	return size;
}

void subscriber_helper::get_msg(char*buffercopy){
	strcpy(buffercopy,buffer);
}
