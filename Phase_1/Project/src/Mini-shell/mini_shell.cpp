#include "Lexer/Lexer.hpp"

#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <vector>

using std::cin;
using std::cout;
using std::cerr;
using std::getline;
using std::string;
using std::stringstream;
using std::vector;
using std::getenv;

using Command = vector <string>;
using PipeLine = vector <Command>;
using AndChain = vector <PipeLine>;
using Program = vector <AndChain>;

constexpr int MAX_PATH_LENGTH = PATH_MAX;

const char* home_env = getenv("HOME");
const string home_path = home_env ? home_env : "";

constexpr int IN = STDIN_FILENO;
constexpr int OUT = STDOUT_FILENO;

extern char** environ;

string builtin[] = {
    "cd",
    "help",
    "exit",
    "printenv",
    "setenv",
    "unsetenv",
    "history",
    "echo"
};

string external[] = {
    "ls" ,
    "cat" , 
    "grep" , 
    "mkdir" 
};

vector <string> history_list;

char cwd[MAX_PATH_LENGTH];
string current_path , previous_path , dst_path;
string line;
vector <Token> result;
Program list;

#define endl '\n'
#define updatePath {previous_path = current_path , current_path = getCwd();}
#define ERR1 {cerr << "Error: Too many arguments" << endl; return 1;}
#define ERR2 {cerr << "Error: No such file or directory" << endl; return 1;}
#define ERR3 {cerr << "Error: HOME environment variable not set" << endl; return 1;}
#define ERR4 {cerr << "Arguments is not valid" << endl; return 1;}

//get current path without leak memory
string getCwd(){
    string result;
    if (getcwd(cwd , MAX_PATH_LENGTH)) result = cwd;
    return result;
}

//read line of command
bool readLine(){
    cout << "mini-shell-chihihihi :3 Giỡn chisikudo> " << std::flush;
    return static_cast<bool>(getline(cin , line));
}

//devide cmd to group : ";" -> "&&"" -> "|"
void parse(const vector <Token>& token,Program& list){
    list.clear();
    AndChain cur_AndChain;
    PipeLine cur_PipeLine;
    Command cur_Command;
    string value;
    for (const Token& i : token){
        const auto t = i.type;
        value = i.value;
        if (t == TOKEN_TYPE::WORD){
            cur_Command.push_back(value);
            continue;
        } 
        if (t == TOKEN_TYPE::PIPE){
            if (cur_Command.size()){
                cur_PipeLine.push_back(cur_Command);
                cur_Command.clear();
            }
            continue;
        }
        if (t == TOKEN_TYPE::AND){
            if (cur_Command.size()){
                cur_PipeLine.push_back(cur_Command);
                cur_Command.clear();
            }
            if (cur_PipeLine.size()){
                cur_AndChain.push_back(cur_PipeLine);
                cur_PipeLine.clear();
            }
            continue;
        }
        if (cur_Command.size()) cur_PipeLine.push_back(cur_Command);
    if (cur_PipeLine.size()) cur_AndChain.push_back(cur_PipeLine);
        if (cur_AndChain.empty()) continue;
        list.push_back(cur_AndChain);
        cur_AndChain.clear();
    }
    if (cur_Command.size()) cur_PipeLine.push_back(cur_Command);
    if (cur_PipeLine.size()) cur_AndChain.push_back(cur_PipeLine);
    if (cur_AndChain.size()) list.push_back(cur_AndChain);
}

void history(){
    for (size_t i = 0; i < history_list.size(); ++i)
        cout << i + 1 << "  " << history_list[i] << '\n';
}

void help(){//write by ai ^-^
    cout << "======================================================================\n";
    cout << "                    MINI-SHELL - COMMAND MANUAL                       \n";
    cout << "======================================================================\n\n";

    cout << "BUILT-IN COMMANDS:\n";
    cout << "  cd [dir]                Change working directory.\n";
    cout << "                          Supports '~' or '$HOME' (home dir), '-' (previous dir).\n";
    cout << "  pwd                     Print the current working directory.\n";
    cout << "  echo [args...]          Print arguments to standard output.\n";
    cout << "  history                 Display the list of previously executed commands.\n";
    cout << "  printenv                List all exported environment variables.\n";
    cout << "  setenv <VAR> <VAL>      Set an environment variable.\n";
    cout << "  unsetenv <VAR>          Remove an environment variable.\n";
    cout << "  help                    Show this help manual.\n";
    cout << "  exit                    Terminate and exit the shell.\n\n";

    cout << "CONTROL OPERATORS:\n";
    cout << "  |                       Pipeline: feeds stdout of left command into stdin of right command.\n";
    cout << "                          Example: ls | grep txt | wc -l\n";
    cout << "  &&                      Logical AND: runs right command only if left command succeeds.\n";
    cout << "                          Example: mkdir build && cd build\n";
    cout << "  ;                       Sequence: runs commands sequentially from left to right.\n";
    cout << "                          Example: echo \"Start\" ; ls ; echo \"End\"\n";
    cout << "  \"...\"                   Double Quotes: preserves literal values with spaces.\n";
    cout << "                          Example: echo \"hello world\"\n\n";

    cout << "EXTERNAL COMMANDS:\n";
    cout << "  Any executable available in system PATH (e.g. ls, cat, grep, mkdir, rm,...)\n";
    cout << "======================================================================\n";
}

void printenv(){
    for (char** env = environ ; *env ; env ++) cout << *env << '\n';
}

void pwd(){
    cout << current_path << endl;
}

void setenv(const string& name, const string& value){
    if (::setenv(name.c_str(), value.c_str(), 1) == -1) perror("setenv");
}

void unsetenv(const string& name) {
    if (::unsetenv(name.c_str()) == -1) perror("unsetenv");
}

void echo(const Command& cmd) {
    for (size_t i = 1; i < cmd.size(); ++i) {
        if (i > 1) cout << ' ';
        cout << cmd[i];
    }
    cout << '\n';
}

int isExternalCmd(const Command& list){
    for (string st : external) if (list[0] == st) return 1;
    return 0;
}

int BuiltInCmd(const Command& list){
    if (list[0] == "cd"){
        dst_path.clear();
        if (list.size() > 2) ERR1;
        if (list.size() == 1 || list[1] == "$HOME" || list[1] == "~"){
            if (getenv("HOME") == nullptr) ERR3;
            dst_path = getenv("HOME"); 
            if (!dst_path.empty() && !chdir(dst_path.c_str())) updatePath
            else ERR3;
        }
        else {
            if (list[1] == "-") dst_path = previous_path;
            else dst_path = list[1]; 
            if (!dst_path.empty() && !chdir(dst_path.c_str())) updatePath
            else ERR2;
        }
        return 0;
    }
    if (list[0] == "help") help();
    if (list[0] == "exit") exit(0);
    if (list[0] == "printenv") printenv();
    if (list[0] == "setenv"){
        if (list.size() != 3) ERR4;
        setenv(list[1] , list[2]); 
    }
    if (list[0] == "unsetenv"){
        if (list.size() != 2) ERR4;
        unsetenv(list[1]);
    }
    if (list[0] == "echo") echo(list);
    if (list[0] == "history") history();
    if (list[0] == "pwd") pwd();
    return 0;
}

vector <pid_t> vChild;
int ExternalCmd(const Command& list,const int& in,const int& out){
    vector <char*> ptr;
    pid_t child = fork();
    if (child < 0) return 1;
    if (child == 0){
        dup2(in , STDIN_FILENO);
        dup2(out , STDOUT_FILENO);
        if (in != IN) close(in); 
        if (out != OUT) close(out);
        vChild.push_back(getpid());

        if (!isExternalCmd(list)) BuiltInCmd(list);
        else {
            if (!isExternalCmd(list)){
                cerr << "Error : Unknow command\n";
                return 0;
            }
            char** args;
            for (const string& s : list) ptr.push_back(const_cast<char*>(s.c_str()));
            ptr.push_back(nullptr);
            args = ptr.data();
            execvp(list[0].c_str() , args);
            
            perror("execvp");
            _exit(127);
        }
    }   
    return 0;
}

void solve(const Program& list){
    int num_cmd , fdin , fdout , status;
    for (const AndChain& chain : list){
        int PREV_STATUS = 0;
        for (const PipeLine& line : chain){
            num_cmd = (int)line.size();
            if (num_cmd == 1){
                if (!isExternalCmd(line[0])) PREV_STATUS = BuiltInCmd(line[0]);
                else PREV_STATUS = ExternalCmd(line[0], IN, OUT);
            }
            else {
                vector <vector <int>> pf(num_cmd - 1 , vector <int> (2));
                for (int i = 0 ; i < num_cmd - 1 ; i ++){
                    if (pipe(pf[i].data())) PREV_STATUS = 1;
                    if (PREV_STATUS){
                        for (int j = 0 ; j < i ; j ++) close(pf[j][0]) , close(pf[j][1]);
                        break;
                    }
                }
                if (PREV_STATUS) break;
                for (int i = 0 ; i < num_cmd && !PREV_STATUS ; i ++){
                    fdin = !i ? IN : pf[i - 1][0];
                    fdout = i + 1 < num_cmd ? pf[i][1] : OUT;
                    PREV_STATUS = ExternalCmd(line[i] , fdin , fdout);
                }
                while (vChild.size()){
                    waitpid(vChild.back() , &status , 0);
                    vChild.pop_back();
                    if (!WIFEXITED(status)) PREV_STATUS = 1;
                }
                for (int i = 0 ; i < num_cmd - 1 ; i ++) close(pf[i][0]) , close(pf[i][1]);
            }
            if (PREV_STATUS) break;
        }
    }
}

int main(){
    current_path = getCwd();
    while (readLine()){
        splitLine(line , result);
        parse(result , list);
        if (list.size() != 1 || list[0].size() != 1 
            || list[0][0].size() != 1 || list[0][0][0][0] != "history") history_list.push_back(line);
        solve(list);
    }
}