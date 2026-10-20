#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>

using std::cerr;
using std::cin;
using std::cout;

int main(){
    cout << "Hello world\n";
    cout << "Get process identification ."; 
    pid_t pid = getpid();
    cout << " Process id : " << pid << '\n';

    pid_t rc = fork();
    if (rc < 0){//rc == -1 ? maybe i don't sure
        cerr << "Fork failed\n";
        exit(1);
    } else if (rc == 0){
        cout << "Id of this proccess child : ";
        cout << getpid() << '\n';
    } else {
        cout << "Id of " << rc << " proccess parent is: ";
        cout << getpid() << '\n';
    }

    return 0;
}
