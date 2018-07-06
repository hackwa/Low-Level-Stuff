#ifndef TOKENIZER
#define TOKENIZER

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <algorithm>
#include <iterator>
#include <memory>

typedef enum TokenType
{
    // Used for testing characters and defining rules
    undefined, character, special, whitespace,
    // Actual token types
    integer, real, string, identifier,
    keyword, grouping_symbol, operators, comment
} TokenType;

class token {
public:
    std::string data;
    TokenType type = undefined;
    long line_number;
};

extern std::vector<std::shared_ptr<token>> tv;

void tokenize(std::string& line);
void addToken(int start, int end, std::string& line, TokenType token_type);

#ifndef TOKMAIN
#define TOKMAIN
#else

std::vector<std::shared_ptr<token>> tv;

std::string dual_operator_list[] =
{
    "==", "!=", "<=", ">=", "+=", "-=","/=","*=", "//","++",
    "--", "%=", "&&", "||"
};

std::string keyword_list[]=
{
    "break", "case", "catch", "class", "const", "continue", "debugger",
    "default", "delete", "do", "else", "export", "extends", "finally",
    "for", "function", "if", "import", "in", "instanceof", "new",
    "return", "super", "switch", "this", "throw", "try", "typeof",
    "var", "void", "while", "with", "yield"
};


char grouping_symbol_list[]=
{
    '{','}','(',')','[',']'
};

long line_number = 1;

#define ascii_quotes_single 39
#define ascii_quotes_double 34

#define ascii_space 32
#define ascii_tab 9

#define ascii_A 65
#define ascii_a 97
#define ascii_Z 90
#define ascii_z 122
#define ascii_0 48
#define ascii_9 57

#define ascii_dot 46

#endif
#endif
