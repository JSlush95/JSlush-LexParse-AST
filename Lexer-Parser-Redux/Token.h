#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    KEYWORD, IDENTIFIER, OPERATOR, INTEGER_LITERAL, DOUBLE_LITERAL, STRING_LITERAL,
    PUNCTUATOR, COMMENT, END_OF_FILE, UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
};

#endif // TOKEN_H
