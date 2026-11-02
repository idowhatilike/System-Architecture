#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

enum struct TOKEN_TYPE {
    WORD , 
    AND , 
    PIPE , 
    SEQUENCE , 
};

struct Token {
    TOKEN_TYPE type;
    string value;
};

void tokenizeWord(const string& cur_word , vector<Token>& result , const int& inQuotes);

bool splitLine(const string& Line , vector<Token>& result);

void testSplitLine(const string& line);