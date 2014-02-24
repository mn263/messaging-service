#include "server.h"

Server::Server(int port) {
	port_ = port;
}

Server::~Server() {}
/////////////////////////////////////////////////////////
struct c_struct {
	Users* users;
    Queue* queue;
};

void *doit(void *);


void *
doit(void *vptr)
{
    struct c_struct* c;
    c = (struct c_struct*) vptr;
    while (1) {
		int client = c->queue->take();
		Worker w = Worker(c->users);
		w.handle(client);
    }
}
//////////////////////////////////////////////////////////
void Server::serve() {
	// setup client
	int client;
	Users users;
	struct sockaddr_in client_addr;
	socklen_t clientlen = sizeof(client_addr);

    vector<pthread_t*> threads;
    for (int i=0; i<10; i++) {
        pthread_t* thread = new pthread_t;
        c_struct* data = new c_struct;
        data->queue = &queue_;
        data->users = &users;
        // create thread
        pthread_create(thread, NULL, &doit, (void *) data);
        threads.push_back(thread);
    }


	while (1) {
		client = accept(server_, (struct sockaddr *) &client_addr, &clientlen);
		//add to queue
		queue_.append(client);
	}

    // wait for threads to terminate.
    for (int i=0; i<10; i++) {
        pthread_join(*threads[i], NULL);
        delete threads[i];
    }
}

void Server::run() {
	create();
	serve();
}

void Server::create() {
    struct sockaddr_in server_addr;

    // setup socket address structure
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // create socket
    server_ = socket(PF_INET,SOCK_STREAM,0);
    if (!server_) {
        perror("socket");
        exit(-1);
    }

    // set socket to immediately reuse port when the application closes
    int reuse = 1;
    if (setsockopt(server_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        exit(-1);
    }

    // call bind to associate the socket with our local address and port
    if (bind(server_,(const struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) {
        perror("bind");
        exit(-1);
    }

      // convert the socket to listen for incoming connections
    if (listen(server_,SOMAXCONN) < 0) {
        perror("listen");
        exit(-1);
    }}
