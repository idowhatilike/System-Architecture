#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>

using std::cin;
using std::cout;
using std::cerr;

signed main(){
    int fd[2];
    char buffer[67] = {0};//six seven $-$ :3
    if (pipe(fd) < 0){
        cerr << "Pipe init failed\n";
        exit(1);
    }

    pid_t c = fork();
    pid_t cur = getpid();
    
    if (c < 0){
        cerr << "Fork failed\n";
        exit(2);
    }
    
    if (c == 0){
        const char *mess = "hello";
        cout << "hello" << '\n';
        close(fd[0]);
        write(fd[1] , mess , 6);
        close(fd[1]);
        return 0;
    }

    close(fd[1]);
    read(fd[0] , buffer , sizeof(buffer));
    close(fd[0]); 
                  
    for (char c : buffer) if (c) cout << c << "1"; cout << '\n';
    cout << "goodbye\n";
    return 0;
}