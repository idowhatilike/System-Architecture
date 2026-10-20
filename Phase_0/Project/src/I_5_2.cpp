#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    int child = fork();

    close(STDOUT_FILENO);
    open("./i52.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

    if (child < 0) {
        std::cerr << "Fork failed\n";
        return 1;
    } 
    else if (child == 0) {
        std::cout << "Child Id : " << getpid() << '\n';
    } 
    else {
        std::cout << "Parent of " << child << " is " << getpid() << '\n';
    }

    return 0;
}