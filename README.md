## Echo Server and Client

This code implements a more functional and complete echo server and
client. The client sends messages terminated by a newline, and the
server parses until it receives a newline. Both client and server
check the return value of send() and recv() and use loops to be sure
the entire message is sent or received in each case.

The main programs are in `echo-server.cc` and `echo-client.cc`. The
server uses `inet-server.cc` and `inet-server.h` to create Internet
sockets using IPv4 addresses. These files in turn rely on `server.cc`
and `server.h` for the protocol logic and processing. The client uses
`inet-client.cc` and `inet-client.h` to create the client version of
an Internet socket. These files likewise rely on `client.cc` and
`client.h` for the protocol processing.


