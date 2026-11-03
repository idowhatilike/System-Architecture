#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

using simpleCmd = vector <string>;

enum struct TOKEN_TYPE {
    WORD , 
    REDIRECT_IN ,
    REDIRECT_OUT ,
    REDIRECT_APPEND ,
    AND , 
    PIPE , 
    SEQUENCE ,  
};

struct Token {
    TOKEN_TYPE type;
    string value;
};

enum class REDIRECT_TYPE {
    INPUT , 
    OUTPUT , 
    APPEND , 
    NONE
};

struct Redirect {
    REDIRECT_TYPE type;
    string filename;
};

struct Command {
    simpleCmd args;
    vector <Redirect> reds;
};

constexpr auto IN = REDIRECT_TYPE::INPUT;
constexpr auto OUT = REDIRECT_TYPE::OUTPUT;
constexpr auto APP = REDIRECT_TYPE::APPEND;
constexpr auto NONE = REDIRECT_TYPE::NONE;

constexpr auto WORD = TOKEN_TYPE::WORD;
constexpr auto AND = TOKEN_TYPE::AND;
constexpr auto PIPE = TOKEN_TYPE::PIPE;
constexpr auto SEQUENCE = TOKEN_TYPE::SEQUENCE;
constexpr auto RD_IN = TOKEN_TYPE::REDIRECT_IN;
constexpr auto RD_OUT = TOKEN_TYPE::REDIRECT_OUT;
constexpr auto RD_APP = TOKEN_TYPE::REDIRECT_APPEND;

using PipeLine = vector <Command>;
using AndChain = vector <PipeLine>;
using Program = vector <AndChain>;

void tokenizeWord(
    const string& cur_word , 
    vector<Token>& result , 
    const int& inQuotes);

bool splitLine(
    const string& Line , 
    vector<Token>& result);

void testSplitLine(const string& line);

inline int non_empty(Command& cmd){
    return !cmd.args.empty() || !cmd.reds.empty();
}

inline void _clear(Command& cmd){
    cmd.args.clear();
    cmd.reds.clear();
}