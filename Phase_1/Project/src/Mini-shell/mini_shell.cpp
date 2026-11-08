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
#include <fcntl.h>
#include <csignal>
#include <array>
#include <string_view>

using std::cin;
using std::cout;
using std::cerr;
using std::getline;
using std::getenv;

constexpr int MAX_PATH_LENGTH = PATH_MAX;

const char* home_env = getenv("HOME");
const string home_path = home_env ? home_env : "";

constexpr int INFD = STDIN_FILENO;
constexpr int OUTFD = STDOUT_FILENO;

constexpr const char* COLOR_GREEN = "\033[1;32m";
constexpr const char* COLOR_ORANGE = "\033[1;33m";
constexpr const char* COLOR_RESET  = "\033[0m";

extern char** environ;

constexpr std::array<std::string_view, 9> builtin = {
    "cd",
    "help",
    "exit",
    "printenv",
    "setenv",
    "unsetenv",
    "history",
    "echo",
    "clear"
};

string external[] = {
    "ls" ,
    "cat" , 
    "grep" , 
    "mkdir" 
};

volatile std::sig_atomic_t end;
volatile std::sig_atomic_t sig;

simpleCmd history_list;

char cwd[MAX_PATH_LENGTH];
string current_path , previous_path , dst_path;
string line;
vector <Token> result;
Program cmds;
int saved_in , saved_out;

//restore fd in/out after redirection
void restoreFd(){
    dup2(saved_in , INFD);
    dup2(saved_out , OUTFD);
    close(saved_in);
    close(saved_out);
}

//get current path without leak memory
string getCwd(){
    string result;
    if (getcwd(cwd , MAX_PATH_LENGTH)) result = cwd;
    return result;
}

inline void updatePath(){
    previous_path = current_path;
    current_path = getCwd();
}

inline int reportError(std::string_view msg){
    cerr << msg << '\n';
    restoreFd();
    return 1;
}

void mini_shell_path(){
    string user_host = "chisikudo";
    cout << COLOR_GREEN << user_host << COLOR_RESET;
    cout << " : " << COLOR_ORANGE << current_path << COLOR_RESET;
    cout << "$> ";
}

//read line of command
bool readLine(){
    sig = 0;
    mini_shell_path();
    if (!getline(cin , line)){
        if (cin.eof() && !sig){
            cout << "Goodbye homie !\n";
            return false;
        }
        cin.clear(); line.clear();
        end = 0;
    }
    return 1;
}

//devide cmd to group : ";" -> "&&"" -> "|" -> ">>" -> ">" & "<"
void parse(const vector <Token>& token,Program& cmds){
    cmds.clear();
    AndChain cur_AndChain;
    PipeLine cur_PipeLine;
    Command cur_Command;
    string value;
    auto curRedirect = NONE;
    for (const Token& i : token){
        const auto type = i.type;
        value = i.value;
        if (type == WORD){
            if (curRedirect == NONE) cur_Command.args.push_back(value);
            else {
                cur_Command.reds.push_back({curRedirect , value});
                curRedirect = NONE;
            }
            continue;
        } 
        if (curRedirect != NONE) {
            cerr << "Arguments is not valid\n";
            return;
        }
        if (type == RD_IN || type == RD_OUT || type == RD_APP){
            curRedirect = (type == RD_IN ? IN : (type == RD_OUT ? OUT : APP));
            continue;
        }
        if (type == PIPE){
            if (non_empty(cur_Command)){
                cur_PipeLine.push_back(cur_Command);
                _clear(cur_Command);
            }
            continue;
        }
        if (type == AND){
            if (non_empty(cur_Command)){
                cur_PipeLine.push_back(cur_Command);
                _clear(cur_Command);
            }
            if (cur_PipeLine.size()){
                cur_AndChain.push_back(cur_PipeLine);
                cur_PipeLine.clear();
            }
            continue;
        }
        if (non_empty(cur_Command)){
            cur_PipeLine.push_back(cur_Command);
            _clear(cur_Command);
        }
        if (cur_PipeLine.size()){
            cur_AndChain.push_back(cur_PipeLine);
            cur_PipeLine.clear();
        }
        if (cur_AndChain.empty()) continue;
        cmds.push_back(cur_AndChain);
        cur_AndChain.clear();
    }
    if (non_empty(cur_Command)){
        cur_PipeLine.push_back(cur_Command);
        _clear(cur_Command);
    }
    if (cur_PipeLine.size()){
        cur_AndChain.push_back(cur_PipeLine);
        cur_PipeLine.clear();
    }
    cmds.push_back(cur_AndChain);
    cur_AndChain.clear();
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
    cout << current_path << '\n';
}

void setenv(const string& name, const string& value){
    if (::setenv(name.c_str(), value.c_str(), 1) == -1) perror("setenv");
}

void unsetenv(const string& name) {
    if (::unsetenv(name.c_str()) == -1) perror("unsetenv");
}

void echo(const simpleCmd& list) {
    for (size_t i = 1 ; i < list.size() ; i ++) {
        if (i > 1) cout << ' ';
        cout << list[i];
    }
    cout << '\n';
}

int isBuiltin(const simpleCmd& list){
    for (auto st : builtin) if (list[0] == st) return 1;
    return 0;
}

int isExternalCmd(const simpleCmd& list){
    return !isBuiltin(list);
}

void reDirection(const vector <Redirect>& reds){
    int fd , Option;
    string filename;
    for (const Redirect& i : reds){
        const auto type = i.type;
        filename = i.filename;
        Option = (type == OUT) ? O_TRUNC : O_APPEND;
        if (type == IN){
            if ((fd = open(filename.c_str() , O_RDONLY)) == -1){
                perror(filename.c_str());
                return;
            }
            dup2(fd , INFD);
            if (fd != INFD) close(fd);
            continue;
        }
        if ((fd = open(filename.c_str() , O_WRONLY | O_CREAT | Option , 0644)) == -1){
            perror(filename.c_str());
            return;
        }
        dup2(fd , OUTFD);
        if (fd != OUTFD) close(fd);
    }
}

int BuiltInCmd(const Command& cmd){
    simpleCmd list = cmd.args;
    saved_in = dup(INFD) , saved_out = dup(OUTFD);
    reDirection(cmd.reds);

    if (list[0] == "cd"){
        dst_path.clear();
        if (list.size() > 2) return reportError("Error: Too many arguments");
        if (list.size() == 1 || list[1] == "$HOME" || list[1] == "~"){
            if (getenv("HOME") == nullptr) return reportError("Error: HOME environment variable not set");
            dst_path = getenv("HOME"); 
            if (!dst_path.empty() && !chdir(dst_path.c_str())) updatePath();
            else return reportError("Error: HOME environment variable not set");
        }
        else {
            if (list[1] == "-") dst_path = previous_path;
            else dst_path = list[1]; 
            if (!dst_path.empty() && !chdir(dst_path.c_str())) updatePath();
            else return reportError("Error: No such file or directory");
        }
        return 0;
    }
    if (list[0] == "help") help();
    if (list[0] == "exit") exit(0);
    if (list[0] == "printenv") printenv();
    if (list[0] == "setenv"){
        if (list.size() != 3) return reportError("Arguments is not valid");
        setenv(list[1] , list[2]); 
    }
    if (list[0] == "unsetenv"){
        if (list.size() != 2) return reportError("Arguments is not valid");
        unsetenv(list[1]);
    }
    if (list[0] == "echo") echo(list);
    if (list[0] == "history") history();
    if (list[0] == "pwd") pwd();
    if (list[0] == "clear") cout << "\033[2J\033[1;1H";
    restoreFd();
    return 0;
}

vector <pid_t> vChild;
int notSignleBuiltInCmd(const Command& cmd,const int& in,const int& out){
    simpleCmd list = cmd.args;
    vector <char*> ptr;
    pid_t child = fork();

    if (child < 0) return 1;
    if (child == 0){
        dup2(in , INFD);
        dup2(out , OUTFD);
        if (in != INFD) close(in); 
        if (out != OUTFD) close(out);
        reDirection(cmd.reds);
        if (isBuiltin(list)) BuiltInCmd(cmd);
        else {
            if (isBuiltin(list)){
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
        _exit(0);
    } else vChild.push_back(child);
    return 0;
}

void solve(const Program& cmds){
    end = 0;
    int num_cmd , fdin , fdout , status;
    for (const AndChain& Achain : cmds){
        int PREV_STATUS = 0;
        for (const PipeLine& line : Achain){
            if (end) break;
            num_cmd = (int)line.size();
            if (num_cmd == 1){
                if (isBuiltin(line[0].args)) PREV_STATUS = BuiltInCmd(line[0]);
                else PREV_STATUS = notSignleBuiltInCmd(line[0] , INFD , OUTFD);
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
                    fdin = !i ? INFD : pf[i - 1][0];
                    fdout = i + 1 < num_cmd ? pf[i][1] : OUTFD;
                    PREV_STATUS = notSignleBuiltInCmd(line[i] , fdin , fdout);
                }
                for (int i = 0 ; i < num_cmd - 1 ; i ++) close(pf[i][0]) , close(pf[i][1]);
            }
            while (vChild.size()){
                waitpid(vChild.back() , &status , 0);
                vChild.pop_back();
                if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) end = 1;
                if (!WIFEXITED(status)) PREV_STATUS = 1;
            }
            if (PREV_STATUS || end) break;
        }
    }
}

void signalHandler(int signum){
    sig = signum;
    switch(signum){
        case SIGINT : 
            cout << '\n';
            end = 1;
            cout << "\n[Reze] Kết thúc bằng builtin exit nhé !\n";
            break;
        case SIGTSTP :
            cout << "\n[Baka] Nhấn lộn signal rồi bylat\n";
            break;
        default :
            cout << "\n[Handler] Tín hiệu lạ : " << signum << '\n';
            break;
    }
}

int main(){
    current_path = getCwd();
    struct sigaction sa{};
    sigaction(SIGINT , nullptr , &sa);
    sigaction(SIGTSTP , nullptr , &sa);
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = signalHandler;
    sa.sa_flags &= ~SA_RESTART;
    sigaction(SIGINT ,  &sa , nullptr);
    sigaction(SIGTSTP , &sa , nullptr);
    while (readLine()){
        if (line.empty() || line.find_first_not_of(" \t") == string::npos) continue;
        splitLine(line , result);
        parse(result , cmds);
        if (cmds.size() != 1 || cmds[0].size() != 1  || cmds[0][0].size() != 1 
        || cmds[0][0][0].args[0] != "history") history_list.push_back(line);
        solve(cmds);
    }
}