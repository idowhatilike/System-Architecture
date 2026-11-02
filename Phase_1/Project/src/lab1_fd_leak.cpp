#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    // Mở file bí mật. Thử nghiệm TH1: O_RDONLY vs TH2: O_RDONLY | O_CLOEXEC
    int secret_fd = open("/etc/hosts", O_RDONLY | O_CLOEXEC );
    std::cout << "[Parent] Secret file opened at FD: " << secret_fd << std::endl;

    pid_t pid = fork();
    if (pid == 0) {
        // Con thực thi lệnh kiểm tra danh sách FD đang mở
        char* args[] = {(char*)"/bin/ls", (char*)"-l", (char*)"/proc/self/fd", nullptr};
        execve("/bin/ls", args, environ);
        _exit(1);
    }
    waitpid(pid, nullptr, 0);
    close(secret_fd);
    return 0;
}
