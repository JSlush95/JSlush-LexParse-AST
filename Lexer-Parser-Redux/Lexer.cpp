#include "Lexer.h"
#include <cctype>
#include <unordered_map>
#include <unordered_set>

Lexer::Lexer(const std::string& input) : input(input), pos(0), line(1), column(0) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (pos < input.size()) {
        char current = input[pos];
        if (std::isspace(current)) {
            advance();
            continue;
        }
        if (std::isalpha(current) || current == '_') {
            tokens.push_back(identifier());
            continue;
        }
        if (std::isdigit(current)) {
            tokens.push_back(number());
            continue;
        }
        if (current == '"') {
            tokens.push_back(string());
            continue;
        }
        if (current == '/' && peek() == '/') {
            tokens.push_back(comment());
            continue;
        }
        tokens.push_back(punctuatorOrOperator());
    }
    tokens.push_back({ TokenType::END_OF_FILE, "", line, column });
    return tokens;
}

void Lexer::advance() {
    if (input[pos] == '\n') {
        line++;
        column = 0;
    }
    else {
        column++;
    }
    pos++;
}

char Lexer::peek() const {
    return pos + 1 < input.size() ? input[pos + 1] : '\0';
}

Token Lexer::identifier() {
    size_t start = pos;
    while (std::isalnum(input[pos]) || input[pos] == '_') {
        advance();
    }
    std::string value = input.substr(start, pos - start);
    return { isKeyword(value) ? TokenType::KEYWORD : TokenType::IDENTIFIER, value, line, column };
}

Token Lexer::number() {
    size_t start = pos;
    while (std::isdigit(input[pos])) {
        advance();
    }
    if (input[pos] == '.') {
        advance();
        while (std::isdigit(input[pos])) {
            advance();
        }
        return { TokenType::DOUBLE_LITERAL, input.substr(start, pos - start), line, column };
    }
    return { TokenType::INTEGER_LITERAL, input.substr(start, pos - start), line, column };
}

Token Lexer::string() {
    size_t start = pos;
    advance(); // Skip opening quote
    while (input[pos] != '"') {
        advance();
    }
    advance(); // Skip closing quote
    return { TokenType::STRING_LITERAL, input.substr(start + 1, pos - start - 2), line, column };
}

Token Lexer::comment() {
    size_t start = pos;
    while (input[pos] != '\n') {
        advance();
    }
    return { TokenType::COMMENT, input.substr(start, pos - start), line, column };
}

Token Lexer::punctuatorOrOperator() {
    std::string value(1, input[pos]);
    advance();

    // Handle compound operators
    if (value == "=" && peek() == '=') {
        value += peek();
        advance();
    }
    else if (value == "!" && peek() == '=') {
        value += peek();
        advance();
    }
    else if (value == "<" && peek() == '=') {
        value += peek();
        advance();
    }
    else if (value == ">" && peek() == '=') {
        value += peek();
        advance();
    }
    else if (value == "&" && peek() == '&') {
        value += peek();
        advance();
    }
    else if (value == "|" && peek() == '|') {
        value += peek();
        advance();
    }

    // Define what we consider as punctuators
    static const std::unordered_set<std::string> punctuators = {
        "(", ")", "{", "}", "[", "]", ";", ",", ".", ":", "?", "=", "+", "-", "*", "/", "%", "<", ">", "&", "|", "!"
    };

    return { punctuators.count(value) ? TokenType::PUNCTUATOR : TokenType::UNKNOWN, value, line, column };
}

bool Lexer::isKeyword(const std::string& value) {
    static const std::unordered_map<std::string, bool> keywords = {
        {"public", true}, {"private", true}, {"int", true}, {"double", true},
        {"string", true}, {"if", true}, {"else", true}, {"for", true},
        {"while", true}, {"do", true}, {"switch", true}, {"case", true},
        {"default", true}, {"break", true}
    };
    return keywords.find(value) != keywords.end();
}
