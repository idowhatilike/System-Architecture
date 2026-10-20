#include <iostream>
#include <thread>
#include <string>

int counter = 0;

void worker(int loops) {
    for (int i = 0; i < loops; i++) {
        counter++;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: cpu <loops_number>\n";
        return 1;
    }

    int loops = std::stoi(argv[1]);

    std::thread t1(worker, loops);
    std::thread t2(worker, loops);

    t1.join();
    t2.join();

    std::cout << "Final value: " << counter << "\n";

    return 0;
}
