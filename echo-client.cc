#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <iostream>

#include "inet-client.h"

using namespace std;

int
main(int argc, char **argv)
{
    int option;

    // setup default arguments
    int port = 3000;
    string host = "localhost";

    // process command line options using getopt()
    // see "man 3 getopt"
    while ((option = getopt(argc,argv,"h:p:d")) != -1) {
        switch (option) {
            case 'p':
                port = atoi(optarg);
                break;
            case 'h':
                host = optarg;
                break;
            case 'd':
            	cout << "Debugging is enabled\n" << endl;
            	break;
            default:
                cout << "client [-h host] [-p port] [-d]" << endl;
                exit(EXIT_FAILURE);
        }
    }

    InetClient client = InetClient(host, port);
    client.run();
}

