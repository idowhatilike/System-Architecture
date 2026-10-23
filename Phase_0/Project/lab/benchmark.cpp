#include <bits/stdc++.h>//so lazy
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include "unique_Fd.hpp"

using namespace std;

const int N = 100'0;

void TaskA(){
    auto s = chrono::steady_clock::now();
    for (int i = 1 ; i <= N * 10 ; i ++) read(STDIN_FILENO , nullptr , 0);
    auto e = chrono::steady_clock::now();

    auto time = chrono::duration<double , nano>(e - s).count();
    cout << "Task A : " << fixed << setprecision(9) << time / (N * 10) << " ns\n";
}


void TaskB(){
    FILE* f = fopen("/dev/null", "w");
    if (!f){
        cerr << "Cant open /dev/null\n";
        return;
    }

    int fd = fileno(f);
    char c = 'x';

    auto s = chrono::steady_clock::now();
    for (int i = 1; i <= N; i++) write(fd, &c, 1);
    auto e = chrono::steady_clock::now();
    auto time_w = chrono::duration<double, nano>(e - s).count();

    s = chrono::steady_clock::now();
    for (int i = 1; i <= N; i++) fwrite(&c, 1, 1, f);
    fflush(f);
    e = chrono::steady_clock::now();
    auto time_fw = chrono::duration<double, nano>(e - s).count();

    cout << "Task B : \n"
         << "Using write : " << fixed << setprecision(9) << time_w / N << " ns\n"
         << "Using fwrite : " << fixed << setprecision(9) << time_fw / N << " ns\n";

    fclose(f);
}

void TaskC(){
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    
    scopedPipe pr;
    scopedPipe ch;

    pid_t c = fork();
    if (c < 0){
        cerr << "Fork failed\n";
        return;
    }
    if (c == 0){
        cout << "Children id : " << getpid() << '\n';
        ch.close_r() , pr.close_w();
        sched_setaffinity(0, sizeof(cpu_set_t), &set);
        for (int i = 1 ; i <= N ; i ++){
            pr.read();
            ch.write('7');
        }
        ch.close_w() , pr.close_r();
        exit(0);
    }
    cout << "Parent id : " << getpid() << '\n';
    pr.close_r() , ch.close_w();
    sched_setaffinity(0, sizeof(cpu_set_t), &set);
    auto s = chrono::steady_clock::now();
    for (int i = 1 ; i <= N ; i ++){
        pr.write('6');
        ch.read();
    }
    auto e = chrono::steady_clock::now();
    wait(nullptr);
    auto time = chrono::duration<double , nano>(e - s).count();
    cout << "Task C : " << time / N << " ns\n";
    pr.close_w() , ch.close_r();
}

signed main(){

    TaskA();
    TaskB();
    TaskC();

    for (const auto& entry : std::filesystem::directory_iterator("/proc/self/fd")) {
        std::cout << "Dang mo FD: " << entry.path().filename() << '\n';
        auto target = std::filesystem::read_symlink(entry.path());
        cout << "Dang mo FD: " << entry.path().filename() << " -> " << target << '\n';
    }
}