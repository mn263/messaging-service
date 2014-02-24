#include "users.h"

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <string>

using namespace std;
class Worker {
public:
	Worker(Users*);
	virtual ~Worker();

	void handle(int);
    string handle_request(int);
    string parsePut(int, string);
    string parseList(int, string);
    string parseGet(int, string);
    string get_request(int);
    bool send_response(int, string);

    Users* users_;
    int buflen_;
    char* buf_;
};
