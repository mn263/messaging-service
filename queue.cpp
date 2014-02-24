#include "queue.h"

Queue::Queue() {
	sem_init(&s ,0 ,1 );
	sem_init(&n,0 ,0 );
	sem_init(&e,0 , 200);	// I will likely want to change this later
}

Queue::~Queue() {
}

void
Queue::append(int client) {
//	cout << "we Called append in Queue class" << endl;
	sem_wait(&e);
	sem_wait(&s);
	queue.push(client);
	sem_post(&s);
	sem_post(&n);
}

int
Queue::take () {
//	cout << "we Called take in Queue class" << endl;
	sem_wait(&n);
	sem_wait(&s);
	int item = queue.front();
	queue.pop();
	sem_post(&s);
	sem_post(&e);
	return item ;

}
