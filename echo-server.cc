#include <stdlib.h>
#include <unistd.h>

#include <iostream>

#include "server.h"

using namespace std;

int
main(int argc, char **argv)
{
    int option, port;

    // setup default arguments
    port = 3000;

    // process command line options using getopt()
    // see "man 3 getopt"
    while ((option = getopt(argc,argv,"p:d")) != -1) {
        switch (option) {
            case 'p':
                port = atoi(optarg);
                break;
            case 'd':
            	cout << "Debugging is enabled\n" << endl;
            	break;
            default:
                cout << "server [-p port] [-d]" << endl;
                exit(EXIT_FAILURE);
        }
    }

//    Users users;
    Server server(port);
    server.run();
}
