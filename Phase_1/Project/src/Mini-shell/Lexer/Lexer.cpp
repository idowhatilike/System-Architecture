#include "Lexer.hpp"
#include <vector>

#include "Lexer.hpp"
#include <vector>

void tokenizeWord(const string& cur_word , vector<Token>& result , const int& inQuotes){
    if (cur_word.empty() && !inQuotes) return;
    if (!inQuotes){
        if (cur_word == ";"){
            result.push_back({SEQUENCE , cur_word});
            return;
        }
        if (cur_word == "|"){
            result.push_back({PIPE , cur_word});
            return;
        }
        if (cur_word == "&&"){
            result.push_back({AND , cur_word});
            return;
        }
        if (cur_word == ">"){
            result.push_back({RD_OUT , cur_word});
            return;
        }
        if (cur_word == "<"){
            result.push_back({RD_IN , cur_word});
            return;
        }
        if (cur_word == ">>"){
            result.push_back({RD_APP , cur_word});
            return;
        }
    }
    result.push_back({WORD , cur_word});
}

bool splitLine(const string& Line , vector <Token>& result){
    result.clear();
    string cur_word;
    int inQuotes = 0;
    for (int i = 0 ; i < int(Line.size()) ; i ++){
        if (inQuotes){
            if (Line[i] == '"'){
                tokenizeWord(cur_word , result , inQuotes);
                inQuotes = 0;
                cur_word.clear();
                continue;
            }
            cur_word += Line[i];
            continue;
        }
        if (Line[i] == '#'){
            tokenizeWord(cur_word , result , inQuotes);
            cur_word.clear();
            break;
        }
        if (Line[i] == ' '){
            tokenizeWord(cur_word , result , inQuotes);
            cur_word.clear();
            continue;
        }
        if (i + 1 < int(Line.size()) && Line[i] == '>' && Line[i + 1] == '>'){
            tokenizeWord(cur_word , result , inQuotes);
            cur_word = ">>"; i ++;
            tokenizeWord(cur_word , result , inQuotes);
            cur_word.clear();
            continue;
        }
        if (Line[i] == '|' || Line[i] == ';' || Line[i] == '>' || Line[i] == '<'){
            tokenizeWord(cur_word , result , inQuotes);
            cur_word = Line[i];
            tokenizeWord(cur_word , result , inQuotes);
            cur_word.clear();
            continue;
        }
        if (i + 1 < int(Line.size()) && Line[i] == '&' && Line[i + 1] == '&'){
            tokenizeWord(cur_word , result , inQuotes);
            cur_word = "&&"; i ++;
            tokenizeWord(cur_word , result , inQuotes);
            cur_word.clear();
            continue;
        }
        if (Line[i] == '"'){
            tokenizeWord(cur_word , result , inQuotes);
            inQuotes = 1;
            cur_word.clear();
            continue;
        }
        cur_word += Line[i];
    }
    if (inQuotes) return 0;
    tokenizeWord(cur_word, result, inQuotes);
    return 1;
}

#include <iostream>

void testSplitLine(const string& line){
    vector <Token> result;
    if (splitLine(line , result)){
        for (auto [u , v] : result) 
            std::cout << static_cast<int>(u) << ' ' << v << '\n';
    }
}