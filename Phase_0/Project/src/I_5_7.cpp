#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <fcntl.h>
#include <errno.h>

extern char **environ;

using std::cin;
using std::cout;
using std::cerr;

int saved;

void six_seven(){
    dup2(saved , STDOUT_FILENO);
    close(saved);
    cout << "Pass test\n";
    exit(1);
}

signed main(){
    saved = dup(STDOUT_FILENO);
    close(STDOUT_FILENO);
    pid_t c = fork() , cur = getpid();
    char err[] = "Fork fail\n";
    char chi[] = "Proccess child\n";
    char par[] = "Process parent\n";

    if (c < 0){
        if (write(1 , err , 100) == -1) six_seven();
        exit(2);
    }
    
    if (c == 0){
        if (write(1 , chi , 100) == -1) six_seven();
    } else {
        if (write(1 , par , 100) == -1) six_seven();
    }
}