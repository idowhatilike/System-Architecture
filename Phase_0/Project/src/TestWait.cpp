#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>

using std::cerr;
using std::cin;
using std::cout;

int main() {
    cout << "Hello world (pid : " << getpid() << ")\n";

    pid_t rc = fork();

    if (rc < 0) {
        cerr << "Fork failed\n";
        exit(1);
    } else if (rc == 0) {
        cout << "Hello, I am child (pid : " << getpid() << ")\n";
    } else {
        pid_t wc = wait(nullptr);
        cout << "Hello, I am parent of " << rc
             << " (wc : " << wc << ") (pid : " << getpid() << ")\n";
    }

    return 0;
}