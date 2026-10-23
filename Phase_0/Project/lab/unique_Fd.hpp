#pragma once 

#include <utility>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>

using std::cerr;

class uniqueFd {
    private :
        int _fd; 
    public :    
        ~uniqueFd(){
           if (_fd >= 0) close(_fd);
        };
        uniqueFd() : _fd(-1) {}

        int get() const {return _fd;}
        int release() {return std::exchange(_fd , -1);}
        void reset(int new_fd = -1) {if (_fd >= 0) close(_fd) ; _fd = new_fd;}

        uniqueFd& operator = (const uniqueFd&) = delete;
        uniqueFd(const uniqueFd&) = delete;

        uniqueFd& operator = (uniqueFd&& ot) noexcept {
            if (this != &ot){
                reset(); 
                _fd = std::exchange(ot._fd , -1);
            }
            return *this;
        };
        uniqueFd(uniqueFd&& ot) noexcept : _fd(std::exchange(ot._fd , -1)) {};

};

class scopedPipe {
    private :
        uniqueFd _r;//read_end
        uniqueFd _w;//write_end
    public :
        scopedPipe(){
            int pf[2];
            if (pipe(pf) == -1){
                perror("pipe failed");
                exit(1);
            }
            _r.reset(pf[0]) , _w.reset(pf[1]);
        }
        int write(const char& c){
            int id = _w.get();
            if (id == -1){
                cerr << "Cant write\n";
                return -1;
            }
            ::write(id , &c , 1);
            return 0;
        }
        char read() const{
            int id = _r.get();
            if (id == -1){
                cerr << "Cant read\n";
                return -1;
            }
            char result;
            ::read(id , &result , 1);
            return result;
        }
        void close_r(){
            _r.reset();
        }
        void close_w(){
            _w.reset();
        }
};