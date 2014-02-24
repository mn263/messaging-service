#include "users.h"

Users::Users() {
	sem_init(&lock ,0 ,1 );
}


Users::~Users() {
}


string Users::getMessageForUser(string userName, int index) {
	vector<string> messages = getMessagesByUserName(userName);
	if (messages.size() > index) {
		string retMessage = messages[index];
		return retMessage;
	} else {
		return "error index out of bounds";
	}
}


string Users::getSubjectForUser(string userName, int index) {
	vector<string> subjects = getSubjectsByUserName(userName);
	if (subjects.size() > index) {
		string retMessage = subjects[index];
		return retMessage;
	} else {
		return "error index out of bounds";
	}
}

vector<string> Users::getMessagesByUserName(string userName) {
	sem_wait(&lock);
	vector<string> retVector;
	retVector = userMessagesMap[userName];
	sem_post(&lock);
	return retVector;
}

vector<string> Users::getSubjectsByUserName(string userName) {
	sem_wait(&lock);
	vector<string> retVector;
	retVector = userSubjectsMap[userName];
	sem_post(&lock);
	return retVector;
}

int Users::convertStringToInt(string numberStr) {
	for (int i = 0; i < numberStr.size(); i++) {
		char letter = numberStr[i];
		if (isalpha(letter)) {
			cout << "error converting string to an int. string = " + numberStr
					<< endl;
			return -1;
		}
	}
	int number;
	stringstream convert(numberStr);
	if (!(convert >> number)) {
		cout << "error converting string. string = " + numberStr << endl;
		number = 0;
	}
	return number;
}

void Users::createNewUser(string userName) {
	sem_wait(&lock);
	vector<string> messages;
	vector<string> subjects;
	userMessagesMap[userName] = messages;
	userSubjectsMap[userName] = subjects;
	sem_post(&lock);
}

void Users::addMessageAndSubjectToUser(string userName, string subject, string message) {
	sem_wait(&lock);
	userSubjectsMap[userName].push_back(subject);
	userMessagesMap[userName].push_back(message);
	sem_post(&lock);

}

void Users::reset() {
	sem_wait(&lock);
	userSubjectsMap.clear();
	userMessagesMap.clear();
	sem_post(&lock);
}
