#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    int rc = fork();

    if (rc < 0) {
        std::cerr << "Fork failed\n";
        return 1;
    } 
    else if (rc == 0) {
        close(STDOUT_FILENO);
        open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

        char *myargs[] = {
            const_cast<char *>("wc"),
            const_cast<char *>("../CounterLoops.cpp"),
            nullptr
        };

        execvp(myargs[0], myargs);
    } 
    else {
        int wc = wait(nullptr);
    }

    return 0;
}