#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

using std::cin;
using std::cout;
using std::cerr;

signed main(){
    cout << "Hello " << "(Pid : " << getpid() << " )\n";
    int var = 367;

    pid_t child = fork();

    if (child < 0){
        cerr << "Fork failed\n";
        exit(1);
    }

    var = 18;

    if (child == 0){
        cout << var << '\n';
        cout << "Child Id : " << getpid() << '\n';
        return 0;
    }

    cout << var << '\n';
    cout << "Parent of " << child << " is " << getpid() << '\n';
}