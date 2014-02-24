#include "queue.h"
#include "worker.h"
#include "users.h"


using namespace std;

class Server {

public:
    Server(int);
    ~Server();

    void run();
    void handle(int);

private:
    int server_;

protected:
    virtual void create();
    void serve();

    int port_;
    Queue queue_;
};
