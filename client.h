#pragma once

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <locale>
#include <sstream>

using namespace std;

class Client {
public:
    Client();
    ~Client();

    void run();

protected:
    virtual void create();
    virtual void close_socket();
    void echo();
    string parseSendMessage(string);
    bool send_request(string);
    bool handleResponse(string);
    string get_response();
    string parseSendResponse();
    string parseListResponse();
    string parseReadResponse();
    bool fillIndexAndSubjects(string, int, vector<string> &, vector<string> &);
    int convertStringToInt(string);

    int server_;
    int buflen_;
    char* buf_;
};
