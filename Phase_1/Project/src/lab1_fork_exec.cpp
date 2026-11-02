#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) { // Tiến trình con
        char* args[] = {(char*)"/bin/echo", (char*)"Hello from Child Process!", nullptr};
        execve("/bin/echo", args, environ);
        _exit(127); // Chỉ chạy nếu execve thất bại
    } else { // Tiến trình cha
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            std::cout << "[Parent] Child exited with code: " << WEXITSTATUS(status) << std::endl;
        }
    }
    return 0;
}
