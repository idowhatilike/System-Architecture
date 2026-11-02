#include <bits/stdc++.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

int main(){
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0){
        std::cerr << "Pipe creation failed" << std::endl;
        return 1;
    }

    if (fork() == 0){//cmd 1
        close(pipe_fd[0]); dup2(pipe_fd[1], STDOUT_FILENO); close(pipe_fd[1]);
        char* args[] = {(char*)"/bin/ls", (char*)"-l", (char*)"/proc/self/fd", nullptr};
        execve("/bin/ls", args, environ);
        _exit(1);
    }
    if (fork() == 0){//cmd 2
        close(pipe_fd[1]); dup2(pipe_fd[0], STDIN_FILENO); close(pipe_fd[0]);
        char* args[] = {(char*)"/usr/bin/wc", (char*)"-l", nullptr};
        execve("/usr/bin/wc", args, environ);
        _exit(1);
    }

    close(pipe_fd[0]); 
    close(pipe_fd[1]);
    wait(nullptr); wait(nullptr);
}