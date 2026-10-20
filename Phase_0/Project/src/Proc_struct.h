#ifdef Proc
#define Proc

#pragma once

#include <bits/stdc++.h>//lazy implement :))v

struct File;
struct Inode;
struct Trapframe;

struct context {
    uint32_t esp{0};
    uint32_t ebx{0};
    uint32_t ecx{0};
    uint32_t edx{0};
    uint32_t esi{0};
    uint32_t edi{0};
    uint32_t ebp{0};
}

enum proc_states{
    unused , embryo , sleeping ,
    ready , running , blocked , zombie
}

struct proc{
    char* mem{nullptr};                     
    std::size_t sz{0};                      
    char* kst{nullptr};                  
    
    ProcessState state{ProcessState::Unused};
    int pid{-1};                             
    Process* parent{nullptr};                
    
    void* chan{nullptr};                    
    bool killed{false};                      
    
    std::array<File*, MAX_OPEN_FILES> ofile{}; 
    Inode* cwd{nullptr};                   
    
    Context context{};                       
    Trapframe* tf{nullptr};
}

#endif