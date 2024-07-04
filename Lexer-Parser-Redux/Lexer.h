#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "Token.h"

class Lexer {
public:
    Lexer(const std::string& input);
    std::vector<Token> tokenize();

private:
    std::string input;
    size_t pos;
    int line, column;

    void advance();
    char peek() const;
    Token identifier();
    Token number();
    Token string();
    Token comment();
    Token punctuatorOrOperator();
    bool isKeyword(const std::string& value);
};

#endif // LEXER_H
