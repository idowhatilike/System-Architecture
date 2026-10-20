/*

- Dung write : 10^6 * 2 * 200 = 400 * 10^6 (ext CPU cycles)
- Dung fwrite : ground_up(10^6 / 8192) * 2 * 200 = 49200 (ext CPU cycles)
-> Time(write) = Time(fwrite) * 8130,08 

Nhung thuc te chi tot hon 32 lan vi giam cac thao tac ghi nho le nhung fwrite van 
can ghi vao buffer va kiem tra dieu kien buffer

*/


#include <chrono>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    FILE* f = fopen("/dev/null", "w");
    if (!f) return 1;
    const size_t BUF_SIZE = 8192;
    char buffer[BUF_SIZE];
    char c = 'x';
    if (setvbuf(f, buffer, _IOFBF, BUF_SIZE)) return fclose(f), 1;

    auto st = chrono::steady_clock::now();
    for (int i = 1; i <= 1000000; i++) write(fileno(f) , &c , 1);
    auto ed = chrono::steady_clock::now();
    long long time_write = chrono::duration<double, micro>(ed - st).count();

    st = chrono::steady_clock::now();
    for (int i = 1; i <= 1000000; i++) fwrite(&c , 1, 1, f);
    fflush(f);
    ed = chrono::steady_clock::now();
    long long time_fwrite = chrono::duration<double, micro>(ed - st).count();

    cout << fixed << setprecision(6)
    << "write:  " << time_write << " us\n"
    << "fwrite: " << time_fwrite << " us\n"
    << "Ratio:  " << time_write / time_fwrite << "x\n";

    fclose(f);
}
