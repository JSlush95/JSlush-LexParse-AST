#include "Parser.h"
#include <stdexcept>
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {}

ASTNode Parser::parse() {
    ASTNode programNode = { ASTNodeType::PROGRAM, {}, "" };
    while (currentToken().type != TokenType::END_OF_FILE) {
        try {
            programNode.children.push_back(parseStatement());
        }
        catch (const std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << " at line " << currentToken().line << ", column " << currentToken().column << std::endl;
            advance();
        }
    }
    return programNode;
}

Token Parser::currentToken() const {
    return tokens[pos];
}

Token Parser::advance() {
    return tokens[pos++];
}

ASTNode Parser::parseStatement() {
    Token token = currentToken();
    if (token.type == TokenType::KEYWORD) {
        if (token.value == "int" || token.value == "double" || token.value == "string" || token.value == "public" || token.value == "private" || token.value == "void" || token.value == "class") {
            return parseDeclaration();
        }
        else if (token.value == "if") {
            return parseIfStatement();
        }
        else if (token.value == "for") {
            return parseForLoop();
        }
        else if (token.value == "while") {
            return parseWhileLoop();
        }
        else if (token.value == "do") {
            return parseDoWhileLoop();
        }
        else if (token.value == "switch") {
            return parseSwitchStatement();
        }
        else if (token.value == "break" || token.value == "continue") {
            advance(); // Consume the break/continue token
            return { ASTNodeType::EXPRESSION, {}, token.value };
        }
    }
    else if (token.type == TokenType::COMMENT) {
        advance(); // Skip comment
        return parseStatement(); // Continue to the next statement
    }
    else if (token.type == TokenType::IDENTIFIER) {
        return parseAssignment();
    }
    else if (token.type == TokenType::INTEGER_LITERAL || token.type == TokenType::DOUBLE_LITERAL || token.type == TokenType::STRING_LITERAL) {
        return parseExpression();
    }
    else if (token.type == TokenType::PUNCTUATOR) {
        if (token.value == ";") {
            advance(); // Skip semicolon
            return parseStatement();
        }
        else if (token.value == ":") {
            advance(); // Skip colon
            return parseStatement();
        }
        else if (token.value == "=") {
            advance(); // Skip equals sign
            return parseAssignment();
        }
        else if (token.value == "}") {
            advance();  // Skip {
            return ASTNode();   // Nested brackets, return from current "block"
        }
        else {
            std::string errorMsg = "Unexpected punctuator: " + token.value;
            throw std::runtime_error(errorMsg);
        }
    }
    // Throw an exception with details about the unknown statement
    std::string errorMsg = "Unknown statement: Token value = " + token.value + ", Token type = " + std::to_string(static_cast<int>(token.type));
    throw std::runtime_error(errorMsg);
}

ASTNode Parser::parseDeclaration() {
    Token typeToken = advance();
    if (typeToken.value == "class") {
        return parseClassDeclaration();
    }
    Token identifierToken = advance();
    ASTNode declarationNode = { ASTNodeType::DECLARATION, {}, typeToken.value + " " + identifierToken.value };
    if (currentToken().type == TokenType::OPERATOR && currentToken().value == "=") {
        advance(); // Skip '='
        declarationNode.children.push_back(parseExpression());
    }
    if (currentToken().type == TokenType::PUNCTUATOR && currentToken().value == "(") {
        return parseFunctionDefinition(declarationNode);
    }
    if (currentToken().type == TokenType::PUNCTUATOR && currentToken().value == ";") {
        advance(); // Skip ';'
    }
    if (currentToken().type == TokenType::PUNCTUATOR && currentToken().value == "{") {

    }
    return declarationNode;
}

ASTNode Parser::parseClassDeclaration() {
    Token classToken = advance();
    Token className = advance();
    advance(); // Skip '{'
    ASTNode classNode = { ASTNodeType::DECLARATION, {}, classToken.value + " " + className.value };
    while (currentToken().type != TokenType::PUNCTUATOR || currentToken().value != "}") {
        Token memberToken = currentToken();
        if (memberToken.value == "public" || memberToken.value == "private" || memberToken.value == "protected") {
            advance(); // Skip the access specifier
            if (currentToken().type == TokenType::PUNCTUATOR && currentToken().value == ":") {
                advance(); // Skip the ':'
            }
            continue;
        }
        classNode.children.push_back(parseStatement());
    }
    advance(); // Skip '}'
    return classNode;
}

ASTNode Parser::parseFunctionDefinition(ASTNode declarationNode) {
    advance(); // Skip '('
    while (currentToken().type != TokenType::PUNCTUATOR || currentToken().value != ")") {
        advance(); // Skip parameter list for simplicity
    }
    advance(); // Skip ')'
    ASTNode functionNode = { ASTNodeType::FUNCTION_DEFINITION, {declarationNode, parseBlock()}, "" };
    return functionNode;
}

ASTNode Parser::parseAssignment() {
    Token identifierToken = advance();
    if (currentToken().type == TokenType::PUNCTUATOR && currentToken().value == "=") {
        advance(); // Skip '='
        ASTNode assignmentNode = { ASTNodeType::ASSIGNMENT, {parseExpression()}, identifierToken.value };
        if (currentToken().type == TokenType::PUNCTUATOR && currentToken().value == ";") {
            advance(); // Skip ';'
        }
        return assignmentNode;
    }
    throw std::runtime_error("Expected '=' after identifier");
}

ASTNode Parser::parseExpression() {
    ASTNode left = parsePrimary();
    return parseBinaryOpRHS(0, left);
}

ASTNode Parser::parsePrimary() {
    Token token = advance();
    if (token.type == TokenType::INTEGER_LITERAL || token.type == TokenType::DOUBLE_LITERAL || token.type == TokenType::STRING_LITERAL) {
        return { ASTNodeType::LITERAL, {}, token.value };
    }
    else if (token.type == TokenType::IDENTIFIER) {
        return { ASTNodeType::VARIABLE, {}, token.value };
    }
    else if (token.type == TokenType::PUNCTUATOR && token.value == "(") {
        ASTNode expr = parseExpression();
        if (currentToken().type == TokenType::PUNCTUATOR && currentToken().value == ")") {
            advance();
        }
        else {
            throw std::runtime_error("Expected ')'");
        }
        return expr;
    }
    throw std::runtime_error("Expected a primary expression");
}

int Parser::getTokenPrecedence() {
    if (currentToken().type != TokenType::OPERATOR) return -1;

    std::string op = currentToken().value;
    if (op == "=") return 1;
    if (op == "||") return 2;
    if (op == "&&") return 3;
    if (op == "==" || op == "!=") return 4;
    if (op == "<" || op == ">" || op == "<=" || op == ">=") return 5;
    if (op == "+" || op == "-") return 6;
    if (op == "*" || op == "/") return 7;
    return -1;
}

ASTNode Parser::parseBinaryOpRHS(int exprPrec, ASTNode lhs) {
    while (true) {
        int tokPrec = getTokenPrecedence();
        if (tokPrec < exprPrec) return lhs;

        Token opToken = advance();
        ASTNode rhs = parsePrimary();

        int nextPrec = getTokenPrecedence();
        if (tokPrec < nextPrec) {
            rhs = parseBinaryOpRHS(tokPrec + 1, rhs);
        }

        lhs = { ASTNodeType::BINARY_OPERATOR, {lhs, rhs}, opToken.value };
    }
}

ASTNode Parser::parseIfStatement() {
    advance(); // Skip 'if'
    advance(); // Skip '('
    ASTNode condition = parseExpression();
    advance(); // Skip ')'
    ASTNode ifNode = { ASTNodeType::IF_STATEMENT, {condition, parseBlock()}, "" };
    if (currentToken().value == "else") {
        advance(); // Skip 'else'
        if (currentToken().value == "if") {
            ifNode.children.push_back(parseIfStatement());
        }
        else {
            ifNode.children.push_back(parseBlock());
        }
    }
    return ifNode;
}

ASTNode Parser::parseForLoop() {
    advance(); // Skip 'for'
    advance(); // Skip '('
    ASTNode initialization = parseStatement();
    ASTNode condition = parseExpression();
    advance(); // Skip ';'
    ASTNode increment = parseStatement();
    advance(); // Skip ')'
    ASTNode forNode = { ASTNodeType::FOR_LOOP, {initialization, condition, increment, parseBlock()}, "" };
    return forNode;
}

ASTNode Parser::parseWhileLoop() {
    advance(); // Skip 'while'
    advance(); // Skip '('
    ASTNode condition = parseExpression();
    advance(); // Skip ')'
    ASTNode whileNode = { ASTNodeType::WHILE_LOOP, {condition, parseBlock()}, "" };
    return whileNode;
}

ASTNode Parser::parseDoWhileLoop() {
    advance(); // Skip 'do'
    ASTNode doNode = { ASTNodeType::DO_WHILE_LOOP, {parseBlock()}, "" };
    advance(); // Skip 'while'
    advance(); // Skip '('
    doNode.children.push_back(parseExpression());
    advance(); // Skip ')'
    advance(); // Skip ';'
    return doNode;
}

ASTNode Parser::parseSwitchStatement() {
    advance(); // Skip 'switch'
    advance(); // Skip '('
    ASTNode switchNode = { ASTNodeType::SWITCH_STATEMENT, {parseExpression()}, "" };
    advance(); // Skip ')'
    advance(); // Skip '{'
    while (currentToken().type == TokenType::KEYWORD && currentToken().value == "case") {
        switchNode.children.push_back(parseCaseClause());
    }
    if (currentToken().type == TokenType::KEYWORD && currentToken().value == "default") {
        switchNode.children.push_back(parseDefaultClause());
    }
    advance(); // Skip '}'
    return switchNode;
}

ASTNode Parser::parseCaseClause() {
    advance(); // Skip 'case'
    ASTNode caseNode = { ASTNodeType::CASE_CLAUSE, {parseExpression(), parseBlock()}, "" };
    return caseNode;
}

ASTNode Parser::parseDefaultClause() {
    advance(); // Skip 'default'
    advance(); // Skip ':'
    ASTNode defaultNode = { ASTNodeType::CASE_CLAUSE, {parseBlock()}, "" };
    return defaultNode;
}

ASTNode Parser::parseBlock() {
    advance(); // Skip '{'
    ASTNode blockNode = { ASTNodeType::BLOCK, {}, "" };
    while (currentToken().value != "}") {
        blockNode.children.push_back(parseStatement());
    }
    advance(); // Skip '}'
    return blockNode;
}
