#include "client.h"

Client::Client() {
    // setup variables
    buflen_ = 1024;
    buf_ = new char[buflen_+1];
}

Client::~Client() {
}

void Client::run() {
    // connect to the server and run echo program
    create();
    echo();
}

void
Client::create() {
}

void
Client::close_socket() {
}

void
Client::echo() {
//	string send = "put matt subjectOne 52\nmessage line one\nmessage line two\nmessage line three";
//	bool success = send_request(send);
//	success = handleResponse("send");

    bool done = false;
    while(!done) {

		cout << "% ";
		string line;
		getline(cin,line);
//		line = "read matt 1";
		// append a newline
		string type;
		string sendRequest;
		if(line.find("send") == 0) {
			type = "send";
			sendRequest = parseSendMessage(line);
		} else if (line.find("list") == 0) {
			type = "list";
			line += "\n";
			sendRequest = line;
		} else if (line.find("read") == 0) {
			line.erase(0, 4);
			type = "read";
			line += "\n";
			sendRequest = "get" + line;
		} else if (line.find("quit") == 0) {
			done = true;
			continue;
		} else {
			cout << "I don't recognize that command." << endl;
			continue;
		}

		// send request
		bool success = send_request(sendRequest);
		// break if an error occurred
		if (not success)
			break;
		// get a response
		success = handleResponse(type);
		// break if an error occurred
		if (not success)
			break;
    }
    close_socket();
}

string
Client::parseSendMessage(string line) {
	//get rid of 'send'
	line.erase(0, 5);
	string user = line.substr(0, line.find(' '));
	line.erase(0, line.find(' ') + 1);
	string subject = line;

	string messages;
	bool stillReading = true;
	cout << "- Type your message. End with a blank line -" << endl;
	while(stillReading) {
		string message = "";
		getline(cin, message);
		if(message == "") {
			stillReading = false;
		} else {
			messages += message + "\n";
		}
	}

	//compile string to send
	string put = "put ";
	string space = " ";
	string newLine = "\n";
	int messageSize = messages.length();
	string messageSizeStr;
	ostringstream convert;
	convert << messageSize;
	messageSizeStr = convert.str();

	string message = put + user + space + subject + space + messageSizeStr +  newLine + messages;
	return message;
}

bool
Client::send_request(string request) {
    // prepare to send request
    const char* ptr = request.c_str();
    int nleft = request.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(server_, ptr, nleft, 0)) < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                continue;
            } else {
                // an error occurred, so break out
                perror("write");
                return false;
            }
        } else if (nwritten == 0) {
            // the socket is closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return true;
}

string
Client::get_response() {
	string response = "";
    int nread = recv(server_,buf_,1024,0);
    if (nread < 0) {
        if (errno == EINTR)
            // the socket call was interrupted -- try again
//            continue;
        	get_response();
        else
            // an error occurred, so break out
        	cout << "ERROR occurred in get_response()" << endl;
            return "";
    } else if (nread == 0) {
        // the socket is closed
        	cout << "ERROR socket was closed in get_response()" << endl;
        return "";
    }
    // be sure to use append in case we have binary data
    response.append(buf_,nread);

    return response;
}

bool
Client::handleResponse(string type) {
    string response = "";
    // read until we get a newline
    if(type == "send") {
    	response = parseSendResponse();
    } else if (type == "list") {
    	response = parseListResponse();
    } else if (type == "read"){
    	response = parseReadResponse();
    } else {
    	response = "ERROR: could not validate any type";
    }
    if(response != "") {
    	cout << response << endl;
    }
    return true;
}


string
Client::parseSendResponse() {
	string response = "";
    while (response.find("\n") == string::npos) {
    	response += get_response();
    }
	if (response.find("OK") == 0) {
		return "";
	}
    return response;
}

string
Client::parseListResponse() {

	string response = get_response();
	if(response.find("list") == 0) {

		response.erase(0, 5);
		string numberStr = response.substr(0, response.find("\n"));
		int number = convertStringToInt(numberStr);
		response.erase(0, response.find('\n') + 1); //erases 'number'

		vector <string> indexesAndSubjects;
		for(int i = 0; i < number; i++) {
			if (response.find("\n") == string::npos) {
				response += get_response();
			}
			string indexAndSubject = response.substr(0, response.find("\n"));
			response.erase(0, response.find("\n") +1); //erases the indexAndSubject we just got out of it
			indexesAndSubjects.push_back(indexAndSubject);
		}
		response = "";
		for (int i = 0; i < indexesAndSubjects.size(); i++) {
			string line = indexesAndSubjects[i];
			response += line + "\n";
		}
	} else if(response.find("error") == 0) {
		while (response.find("\n") == string::npos) {
			response += get_response();
		}
	} else {
		response = "error parseListResponse came back saying neither 'list' nor 'error'";
	}
    return response;
}

string
Client::parseReadResponse() {
	string response = get_response();
	if(response.find("message") == 0) {
		response.erase(0, 8);

		bool gotSubject = false;
		while(response.find("\n") == string::npos) {
			response += get_response();
		}
		string subjectAndLength = response.substr(0, response.find('\n'));
		string subject = subjectAndLength.substr(0, subjectAndLength.find_last_of(" "));
		string lengthStr = subjectAndLength.substr(subjectAndLength.find_last_of(" "), subjectAndLength.find("\n"));
		int length = convertStringToInt(lengthStr);

		//erase the subject and length from the message
		response.erase(0, response.find('\n') + 1);
		string message = response;
		int messageSize = message.size();
		while (message.size() < length) {
			message += get_response();
		}
		response = subject + "\n" + message;
	} else if(response.find("error") == 0) {
		while (response.find("\n") == string::npos) {
			response += get_response();
		}
	}
    return response;
}

int
Client::convertStringToInt(string numberStr) {
	int number;
	stringstream convert(numberStr);
	if ( !(convert >> number) ) {
		cout << "Error converting numberStr to an int. numberStr = " + numberStr << endl;
		number = 0;
	}
	return number;
}







