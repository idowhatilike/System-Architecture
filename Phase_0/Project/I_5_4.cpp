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
        exit(2);
    }
    
    char *argv[] = {
        const_cast<char *>("../CounterLoops") , 
        const_cast<char *>("./i54.output") , 
        nullptr
    };
    
    if (c == 0){
        close(STDOUT_FILENO);
        open("./i54.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

        execl("./CounterLoops" , "./i54.output" , nullptr);
        // có thể thay "../CounterLoops" -> argv[0] :))
        execlp("CounterLoops" , "./i54.output" , nullptr);
        execle("./CounterLoops" , "./i54.output" , nullptr , environ);

        execv("./CounterLoops" , argv);
        execvp("CounterLoops" , argv);
        execve("./CounterLoops" , argv , environ);

        perror("Exec failed");
        cout << "This shouldn't print here\n";
    } else {
        cout << "HEHE \n";
    }
}