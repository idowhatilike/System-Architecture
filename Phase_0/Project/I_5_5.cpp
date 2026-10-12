#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <fcntl.h>

extern char **environ;

using std::cin;
using std::cout;
using std::cerr;

signed main(){
    pid_t c = fork() , cur = getpid();
    
    if (c < 0){
        cerr << "Fork failed\n";
        exit(1);
    }
    
    if (c == 0){
        if (wait(nullptr) == -1){
            cerr << "Wait failed\n";
            exit(2);
        }
        cout << "Child id " << cur << '\n';
        return 0;
    } else {
        if (wait(nullptr) == -1){
            cerr << "Wait failed\n";
            exit(2);
        }
        cout << "Parent id " << cur << '\n';
        return 0;
    }
}