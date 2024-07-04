#include <iostream>
#include <fstream>  // For file input
#include <streambuf>  // For reading file content
#include "Lexer.h"
#include "Parser.h"
#include "Ast.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Failed to open input file.\n";
        return 1;
    }

    // Read the entire file into a string
    std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    Lexer lexer(code);
    auto tokens = lexer.tokenize();
    for (const auto& token : tokens) {
        std::cout << "Token: " << token.value << ", Type: " << static_cast<int>(token.type) << "\n";
    }

    Parser parser(tokens);
    auto ast = parser.parse();
    std::cout << "Parsing completed.\n";

    // Print the AST
    ast.print();

    return 0;
}
