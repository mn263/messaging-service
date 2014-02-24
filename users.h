#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <iostream>
#include <sstream>

#include <map>
#include <string>
#include <vector>

using namespace std;

class Users {

private:
	map<string, vector<string> > userSubjectsMap;
	map<string, vector<string> > userMessagesMap;

public:
	Users();
	virtual ~Users();

    void reset();
    void createNewUser(string);
    void addMessageAndSubjectToUser(string, string, string);
	string getMessageForUser(string, int);
    string getSubjectForUser(string, int);
    vector<string> getMessagesByUserName(string);
    vector<string> getSubjectsByUserName(string);
    int convertStringToInt(string);

    sem_t lock;
};
