#include "semaphore.h"

#include <iostream>
#include <vector>
#include <queue>

using namespace std;
class Queue {

public:
	Queue();
	virtual ~Queue();

	void append(int);
	int take();
private:
	sem_t s , n, e ;
	std::queue <int> queue;
};
