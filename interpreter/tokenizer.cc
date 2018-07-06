/*
 * Ascii Encoding
 * Characters - 65->90, 97->122
 * Numbers - 48->57,
 * Whitespace - 32,9
 * Allowed - 33 to 126 minus Characters and Whitespace
 */

/* Rules -
 * Variable names not start with numbers
 * No multiline comments for now
 * No multiline strings ot statements
 */

#define TOKMAIN
#include"tokenizer.h"

static void add_token(int start, int end, std::string& line, TokenType token_type)
{
    std::shared_ptr<token> T = std::make_shared<token>();
    T->data = line.substr(start,end-start+1);
    T->type = token_type;
    T->line_number = line_number;
    tv.push_back(T);
    // For Debugging
    std::string token_string="undefined";
    if (token_type == integer)         token_string = "int";
    if (token_type == real)            token_string = "float";
    if (token_type == comment)         token_string = "comment";
    if (token_type == string)          token_string = "string";
    if (token_type == identifier)      token_string = "identifier";
    if (token_type == keyword)         token_string = "keyword";
    if (token_type == grouping_symbol) token_string = "grouping symbol";
    if (token_type == operators)       token_string = "operator";
    std::cout<< line_number << " " << token_string << " : " << T->data << std::endl;
}

void exit_error(std::string error)
{
    std::cout << "ERR: " << error << " at line " << line_number << std::endl;
    exit(-1);
}

static inline TokenType get_character_type(char input_character)
{
    char c = input_character;
    // Compare ASCII codes
    if (c < 32 || c == 127)
        exit_error("Unknown character encountered");
    if ((c >= ascii_A && c <= ascii_Z) || (c >= ascii_a && c <= ascii_z))
        return character;
    if (c >= ascii_0 && c <= ascii_9)
        return integer;
    if (c == ascii_space || c == ascii_tab)
        return whitespace;
    return special;
}

static inline int string_scan(int position, std::string& line)
{
    int begin = position++;
    int line_end = line.length() - 1;
    /*
     * Rule - Single Quotes match with single
     * and double Quotes with double.
     */
    while (line[position] != line[begin]) {
        ++position;
        if (position > line_end)
            exit_error("Unmatched quotes");
    }
    /* 
     * Rule - Next Character after string should be
     * either whitespace or special character
     */
    TokenType next = whitespace;
    if (position < line_end)
        next = get_character_type(line[position+1]);
    if (next != whitespace && next != special)
        exit_error("String tokenize error");
    add_token(begin+1, position-1, line, string);
    return position;
}

static inline int number_scan(int position, std::string& line)
{
    int begin = position;
    TokenType numType = integer;
    int line_end = line.length() - 1;
    int i = position;
    /*
     * Rule - Numbers can be either integers or
     * floats.
     */
    while (i <= line_end) {
        if (line[i] == ascii_dot) {
            numType = real;
        }
        else
            if (get_character_type(line[i]) != integer)
                break;
        ++i;
    }
    /*
     * Rule - Identifiers cannot be preceded by numbers.
     */
    if ((i <= line_end) && (get_character_type(line[i]) == character))
        exit_error("Identifiers cannot begin with numbers");
    add_token(begin, i-1, line, numType);
    return --i;
}

static inline void special_scan(int position, std::string& line)
{
    TokenType specialToken = operators;
    for (int j=0; j < 6; j++) {
        if (line[position] == grouping_symbol_list[j])
            specialToken = grouping_symbol;
    }
    add_token(position, position, line, specialToken);
}

static inline int identifier_scan(int position, std::string& line)
{
    int line_end = line.length() - 1;
    TokenType identifier_type = identifier;
    TokenType current_char_type = whitespace;
    int  i = position;
    /*
     * Rule - Identifiers contain numbers/characters
     * and no underscores (for now).
     */
    while (i <= line_end) {
        current_char_type = get_character_type(line[i]);
        if (current_char_type != character &&
            current_char_type != integer) {
            break;
        }
        ++i;
    }
    i -= 1;
    /*
     * Rule - Keywords are special case of identifiers
     */
    bool exists = std::find(
            std::begin(keyword_list),
            std::end(keyword_list),
            line.substr(position, i-position+1)
            ) != std::end(keyword_list);
    if (exists)
        identifier_type = keyword;
    add_token(position, i, line, identifier_type);
    return i;
}

void tokenize(std::string& line)
{
    int line_end = line.length() - 1;
    // Goto first non whitespace character
    int i = 0; // Current Position
    while (line[i] == ascii_space || line[i] == ascii_tab) {
        i++;
    }

    // Start Scan
    TokenType previous = whitespace;
    TokenType current = undefined;
    TokenType next = undefined;
    bool exists;
    for (; i <= line_end; ++i) {

        // String
        if (line[i] == ascii_quotes_single || line[i] == ascii_quotes_double) {
            i = string_scan(i, line);
            previous = character;
            continue;
        }

        // Dual Operators
        current = get_character_type(line[i]);
        if (i < line_end) {
            next = get_character_type(line[i+1]);
            if (current == special && next == special) {
                exists = std::find(
                    std::begin(dual_operator_list),
                    std::end(dual_operator_list),
                    line.substr(i, 2)
                    ) != std::end(dual_operator_list);
                if (exists) {
                    if (line.substr(i, 2) == "//") {
                        // add_token(i+2,line_end,line,comment);
                        // Comment lines
                        std::cout << "comment : " <<
                        line.substr(i+2, line_end-i-1) <<"\n";
                        line_number += 1;
                        return;
                    }
                    add_token(i, i+1, line, operators);
                    i += 1;
                    previous = special;
                    continue;
                }
            }
        }

        // Everything Else
        switch(current) {
            case whitespace:
                break;

            case character:
                if (previous == whitespace || previous == special)
                    i = identifier_scan(i, line);
                break;

            case integer:
                if (previous != character)
                    i = number_scan(i, line);
                break;

            case special:
                special_scan(i, line);
                break;

            default:
                exit_error("Error in tokenize");
        }
        previous = current;
    }
    line_number += 1;
}
