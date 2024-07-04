#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "Token.h"
#include "Ast.h"

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    ASTNode parse();

private:
    std::vector<Token> tokens;
    size_t pos;

    Token currentToken() const;
    Token advance();

    ASTNode parseStatement();
    ASTNode parseDeclaration();
    ASTNode parseAssignment();
    ASTNode parseExpression();
    ASTNode parsePrimary();
    int getTokenPrecedence();
    ASTNode parseBinaryOpRHS(int exprPrec, ASTNode lhs);
    ASTNode parseIfStatement();
    ASTNode parseForLoop();
    ASTNode parseWhileLoop();
    ASTNode parseDoWhileLoop();
    ASTNode parseSwitchStatement();
    ASTNode parseCaseClause();
    ASTNode parseDefaultClause();
    ASTNode parseBlock();
    ASTNode parseClassDeclaration();
    ASTNode parseFunctionDefinition(ASTNode declarationNode);
};

#endif // PARSER_H
