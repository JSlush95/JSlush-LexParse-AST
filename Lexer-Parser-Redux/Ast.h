#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <iostream>

enum class ASTNodeType {
    PROGRAM, DECLARATION, ASSIGNMENT, EXPRESSION, IF_STATEMENT,
    FOR_LOOP, WHILE_LOOP, DO_WHILE_LOOP, SWITCH_STATEMENT, CASE_CLAUSE, BLOCK,
    BINARY_OPERATOR, UNARY_OPERATOR, VARIABLE, LITERAL, FUNCTION_DEFINITION
};

struct ASTNode {
    ASTNodeType type;
    std::vector<ASTNode> children;
    std::string value;

    void print(int indent = 0) const {
        for (int i = 0; i < indent; ++i) std::cout << "  ";
        std::cout << toString(type) << ": " << value << "\n";
        for (const auto& child : children) {
            child.print(indent + 1);
        }
    }

private:
    std::string toString(ASTNodeType type) const {
        switch (type) {
        case ASTNodeType::PROGRAM: return "Program";
        case ASTNodeType::DECLARATION: return "Declaration";
        case ASTNodeType::ASSIGNMENT: return "Assignment";
        case ASTNodeType::EXPRESSION: return "Expression";
        case ASTNodeType::IF_STATEMENT: return "IfStatement";
        case ASTNodeType::FOR_LOOP: return "ForLoop";
        case ASTNodeType::WHILE_LOOP: return "WhileLoop";
        case ASTNodeType::DO_WHILE_LOOP: return "DoWhileLoop";
        case ASTNodeType::SWITCH_STATEMENT: return "SwitchStatement";
        case ASTNodeType::CASE_CLAUSE: return "CaseClause";
        case ASTNodeType::BLOCK: return "Block";
        case ASTNodeType::BINARY_OPERATOR: return "BinaryOperator";
        case ASTNodeType::UNARY_OPERATOR: return "UnaryOperator";
        case ASTNodeType::VARIABLE: return "Variable";
        case ASTNodeType::LITERAL: return "Literal";
        case ASTNodeType::FUNCTION_DEFINITION: return "FunctionDefinition";
        default: return "Unknown";
        }
    }
};

#endif // AST_H
