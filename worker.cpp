#include "worker.h"

Worker::Worker(Users* users) {
	users_ = users;
	buflen_ = 1024;
	buf_ = new char[buflen_ + 1];
}

Worker::~Worker() {
	delete buf_;
}


void Worker::handle(int client) {
//	cout << "we are in the handle method" << endl;
	// loop to handle all requests
	while (1) {
		// get a response
		string response = handle_request(client);
		// break if client is done or an error occurred
		if (response.empty())
			break;
		// send response
		bool success = send_response(client, response);
		// break if an error occurred
		if (not success) {
			break;
//			cout << "send_response was unsuccessful" << endl;
		}
	}
	close(client);
}

string Worker::handle_request(int client) {
//	cout << "we are in the handle_request method" << endl;
//	cout << "client --> " << client << endl;
	string request = get_request(client);
//	cout << "request --> " + request << endl;

	string response;
	if (request.find("put") == 0) {
		response = parsePut(client, request);
	} else if (request.find("list") == 0) {
		response = parseList(client, request);
	} else if (request.find("get") == 0) {
		response = parseGet(client, request);
	} else if (request.find("reset") == 0) {
		users_->reset();
		response = "OK\n";
	} else if (request == "error happened" || request.empty()) {
		return "";
	} else {
		response = "error I don't recognize that command.\n";
	}

	return response;
}

string Worker::parsePut(int client, string request) {
	request.erase(0, 4);

	//get [name]
	while (request.find(" ") == string::npos) {
		if (request.find("\n") != string::npos) {
			return "error the 'put' syntax was invalid.\n";
		}
		request += get_request(client);
	}
	string userName = request.substr(0, request.find(' '));
	request.erase(0, request.find(' ') + 1);

	//get [subject] and [length]
	while (request.find("\n") == string::npos) {
		request += get_request(client);
	}
	//cut out only the subject and length
	string subjectAndLength = request.substr(0, request.find("\n"));
	//cut out the subject and leave the length
	string subject = subjectAndLength.substr(0,
			subjectAndLength.find_last_of(" "));
	//get [length]
	string lengthStr = subjectAndLength.substr(
			subjectAndLength.find_last_of(" ") + 1,
			subjectAndLength.find("\n"));
	request.erase(0, request.find("\n") + 1);
	int length = users_->convertStringToInt(lengthStr);
	if (length == -1) {
		return "error invalid length\n";
	}
	//get [message]
	string message = request;
	while (message.size() < length) {
		message += get_request(client);
	}
	if (message.size() != length) {
		return "error stated length and message length are not equal\n";
	}
	users_->addMessageAndSubjectToUser(userName, subject, message);
	return "OK\n";
}

string Worker::parseList(int client, string request) {
	request.erase(0, 5);
	if (request.empty() || request.find(" ") != string::npos) {
		return "error invalid list syntax\n";
	}

	//get [name]
	while (request.find("\n") == string::npos) {
		request += get_request(client);
	}
	string userName = request.substr(0, request.find('\n'));
	if (userName.find(" ") != string::npos) {
		return "error There was a space found in the username\n";
	} else {

		vector<string> subjectsByUser = users_->getSubjectsByUserName(userName);

		int size = subjectsByUser.size();

		if (size == 0) {
			return "list 0\n";
		}

		string sizeStr;
		ostringstream convert;
		convert << size;
		sizeStr = convert.str();

		string list = "list ";
		string response = list + sizeStr + "\n";

		for (int i = 1; i <= subjectsByUser.size(); i++) {
			ostringstream indexconvert;
			indexconvert << i;
			string index = indexconvert.str();
			string subject = subjectsByUser[i - 1] + "\n";
			response += index + " " + subject;
		}
		return response;
	}
}

string Worker::parseGet(int client, string request) {
	request.erase(0, 4);

	if (request.empty()) {
		return "error there needs to be a name and index.\n";
	}

	//get [name]
	while (request.find("\n") == string::npos) {
		request += get_request(client);
	}
	if (request.find(" ") == string::npos) {
		return "error there needs to be a name and index.\n";
	}
	string userName = request.substr(0, request.find(' '));
	request.erase(0, request.find(' ') + 1);
	string indexStr = request.substr(0, request.find("\n"));
	int index = users_->convertStringToInt(indexStr);
	if (index == -1) {
		return "error invalid length\n";
	}
	string response = "message ";
	string subject = users_->getSubjectForUser(userName, index - 1) + " ";
	string message = users_->getMessageForUser(userName, index - 1);

	if (subject == "error index out of bounds"
			|| message == "error index out of bounds") {
		return "error index out of bounds.\n";
	}
	int messageLength = message.size();
	ostringstream convert;
	convert << messageLength;
	string length = convert.str() + "\n";

	response += subject + length + message;
	return response;
}

string Worker::get_request(int client) {
	string request = "";
	int nread = recv(client, buf_, 1024, 0);
	if (nread < 0) {
		if (errno == EINTR) {
			// the socket call was interrupted -- try again
			return get_request(client);
		} else
			// an error occurred, so break out
			return "";
	} else if (nread == 0) {
		// the socket is closed
		return "";
	}
	// be sure to use append in case we have binary data
	request.append(buf_, nread);
	// a better server would cut off anything after the newline and
	// save it in a cache
	return request;
}

bool Worker::send_response(int client, string response) {

	// prepare to send response
	const char* ptr = response.c_str();
	int nleft = response.length();
	int nwritten;
	// loop to be sure it is all sent
	while (nleft) {
		if ((nwritten = send(client, ptr, nleft, 0)) < 0) {
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
