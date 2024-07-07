# Lexer and Parser Documentation

###  <b>NOTE: This documentation and program is a work in progress and the implementations will contain bugs or deviate during active development.</b>

## Tokens
- Tokens are generated by the lexer and include identifiers, keywords, operators, literals (numbers, strings), and punctuation symbols. These are used by the parser to control the flow of the program.
- These tokens are implemented as a standalone class, because of the separation of responsibilities/concerns.

## Input
Input is currently handled by an argument command line typically containing a ``.txt`` file. By default, there is a provided ``input.txt`` file in the project directory with the class files.

## Compiling and Running the Parser
#### Certain IDEs can run this as well, but you must provide the command line arguments with however the IDE handles it.
#### For Bash (Linux/Mac) or Linux-compatible Windows packages (such as MingW-w64)
**Compile the Parser.cpp File:**
   Open your terminal and navigate to the directory containing `Parser.cpp`, `Lexer.cpp`, and `Token.h`.
   Use the following command to compile `Parser.cpp` along with the other necessary files:<br>
   ``g++ -std=c++14 main.cpp Parser.cpp Lexer.cpp Token.h -o parser``<br><br>
    Then type the following command to execute the ``parser`` file with ``input.txt`` located within the project directory with the class files, or whatever you choose as the argument line.<br>
    ``parser input.txt`` or ``./parser input.txt`` depending on OS.
- This command compiles ``Parser.cpp``, ``Lexer.cpp``, and ``Token.h`` into an executable named ``parser``.<br>
- The -std=c++14 flag can be changed for whatever version is desired.

## Lexer.cpp
``Lexer.cpp`` provides the lexical analysis functionality for converting input source code into tokens. It defines a Lexer class responsible for tokenizing the input string.
### <u>Constructor</u>
``Lexer(const std::string& input);``<br>
Initializes the Lexer with the input string and sets the initial position (pos), line number (line), and column number (column).<br>
### <u>Methods</u>
#### Tokenize <br>
``std::vector<Token> tokenize();``
- Tokenizes the input string and returns a vector of Token objects representing identified tokens.
- Handles whitespace, identifiers, numbers, strings, comments, and punctuators/operators

#### advance()
``void advance();``
- Moves the current position (pos) in the input string forward by one character.
- Updates line and column numbers accordingly.

#### peek()
``char peek() const;``
- Peeks ahead in the input string to retrieve the next character without advancing the position.
- Returns '\0' if there are no more characters to peek.

#### identifier()
``Token identifier();``
- Recognizes and extracts identifiers (including keywords) from the input string.
- Returns a Token object with type IDENTIFIER or KEYWORD.

#### number()
``Token number();``
- Identifies and extracts integer or double literals from the input string.
- Returns a Token object with type INTEGER_LITERAL or DOUBLE_LITERAL.

#### string()
``Token string();``
- Extracts string literals enclosed in double quotes from the input string.
- Returns a Token object with type STRING_LITERAL.

#### comment()
``Token comment();``
- Recognizes comments starting with // and extracts the entire comment line.
- Returns a Token object with type COMMENT.

#### punctuatorOrOperator()
``Token punctuatorOrOperator();``
- Identifies and extracts punctuators or operators from the input string.
- Returns a Token object with type PUNCTUATOR or UNKNOWN.

#### isKeyword(const std::string& value)
``bool isKeyword(const std::string& value);``
- Checks if a given string value is a keyword recognized by the lexer.
- Returns true if value is a keyword; otherwise, false.

## AST
### Abstract Syntax Tree (AST)
An Abstract Syntax Tree (AST) represents the hierarchical structure of parsed source code, capturing statements, expressions, declarations, and control
flow structures.
### Benefits of AST
- Environment Independence: The AST generated by the parser abstracts away specific execution environments, focusing on the language syntax/semantics.
- Language Agnostic: By parsing input into an AST, the compiler or interpreter gains flexibility to support different language syntaxes. Adjustments can be made to the parser methods (parseStatement(), parseExpression(), etc.) to accommodate specific language grammar rules, however.

### Structure of AST Nodes
AST nodes in Parser.cpp are defined as a class or struct (ASTNode) with members representing different parts of the introduced syntax.
### AST Construction
- The parser constructs an Abstract Syntax Tree (AST) recursively using methods such as parseStatement(), parseExpression(), and specific parsing methods for identifiers, numbers, strings, and binary expressions.
- Each method returns an ASTNode representing a parsed construct (statement, expression, identifier, etc.).

### Usage of AST
The AST is constructed recursively during parsing. Each method in Parser.cpp returns an ASTNode representing a parsed construct.
## Parser.cpp
``Parser.cpp`` implements the parsing logic to convert tokens generated by the lexer into an Abstract Syntax Tree (AST).
### <u>Constructor</u><r>
``Parser(const std::vector<Token>& tokens);``
- Initializes the parser with a vector of ``Token`` objects generated by the lexer.

### <u>Methods</u>
#### parse()
``ASTNode parse();``
- Initiates the parsing process and returns the root of the Abstract Syntax Tree (AST).
#### parseStatement()
``ASTNode parseStatement();``
- Parses a single statement from the token stream and returns an appropriate AST node representing the statement.

#### parseExpression()
``ASTNode parseExpression(int precedence = 0);``
- Parses an expression from the token stream, respecting operator precedence and associativity.
- Uses a precedence-based parsing strategy (precedence climbing).

#### parseIdentifier()
``ASTNode parseIdentifier();``
- Parses an identifier (variable or function name) from the token stream and returns an AST node representing it.

#### parseNumber()
``ASTNode parseNumber();``
- Parses a numeric literal (integer or double) from the token stream and returns an AST node representing the literal.

#### parseString()
``ASTNode parseString();``
- Parses a string literal from the token stream and returns an AST node representing the string.

#### parseAssignment(ASTNode lhs)
``ASTNode parseAssignment(ASTNode lhs);``
- Parses an assignment expression (=) with the left-hand side (LHS) already parsed as lhs.
- Returns an AST node representing the assignment.

#### parseBinaryExpression(ASTNode lhs, int precedence)
``ASTNode parseBinaryExpression(ASTNode lhs, int precedence);``
- Parses a binary expression (e.g., arithmetic, logical) from the token stream, starting with the left-hand side (LHS) already parsed as lhs.
- Handles operators based on the specified precedence.

#### expect(TokenType expectedType)
``void expect(TokenType expectedType);``
- Checks if the current token type matches the expected type (expectedType).
- Throws an exception (std::runtime_error) if the types do not match.

#### Error Handling
- The parser uses exceptions (std::runtime_error) for error handling. Errors are thrown for unexpected tokens during the main ``parseStatement()`` call.

### Implementation Note
- This can be adjusted based on language syntax, as the AST structure is flexible. However, the code will need to be adjusted regardless, as programming constructs may vary per language.

# Reminder/Known Issues
- The parser implementation is a work in progress and will contain bugs.
- Further testing, development, and refinement are necessary to allow correct parsing of all language constructs.
