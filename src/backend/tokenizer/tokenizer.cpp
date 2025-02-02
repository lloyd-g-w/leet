#include "tokenizer.hpp"
#include <cctype>
#include <sstream>

namespace std_cells {

static bool isparen(char c);
static bool isquote(char c);
static bool isoperator(char c);
static tok tokenize_number(std::stringstream &ss);
static tok tokenize_string(std::stringstream &ss);
static tok tokenize_identifier(std::stringstream &ss);

// Tokenize a string | can throw an exception
q_tok tokenize(str string) {
    q_tok tokens;

    std::stringstream ss;
    ss << string;

    char c;
    ss >> c;
    if (c != '=')
        throw std_cells::exception::invalid_syntax(
            "Expected expression to start with '='");

    while ((c = ss.peek()) != EOF) {
        if (isspace(c)) {
            ss.ignore();
            continue;
        }

        if (isparen(c) || isoperator(c) || c == ',' || c == ':') {
            tokens.push_back({tok::type::PUNCTUATION, str(1, c)});
            ss.ignore();
        } else if (isquote(c)) {
            tokens.push_back(tokenize_string(ss));
        } else if (isdigit(c)) {
            tokens.push_back(tokenize_number(ss));
        } else if (isalnum(c)) {
            tokens.push_back(tokenize_identifier(ss));
        } else {
            throw std_cells::exception::invalid_syntax(
                "Unexpected character in expression");
        }
    }

    return tokens;
}

static bool isparen(char c) {
    return c == '(' || c == ')';
}

static bool isquote(char c) {
    return c == '"' || c == '\'';
}

static bool isoperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

static tok tokenize_number(std::stringstream &ss) {
    str tok_str;
    auto type = tok::type::INT;
    char c;

    int dot_count = 0;

    while ((c = ss.peek()) != EOF) {
        if (isdigit(c) || c == '.') {
            if (c == '.') {
                type = tok::type::FLOAT;
                dot_count++;
            }

            tok_str += c;
            ss.ignore();
        } else {
            break;
        }
    }

    if (dot_count > 1)
        throw std_cells::exception::invalid_syntax("Invalid number format");

    return tok{type, tok_str};
}

static tok tokenize_string(std::stringstream &ss) {
    str tok_str;
    char c;

    char quote_type;
    ss >> quote_type;
    tok_str += quote_type;

    while ((c = ss.peek()) != EOF) {
        if (c == quote_type) {
            if (tok_str.back() == '\\') {
                tok_str.pop_back();
                tok_str += c;
                ss.ignore();
            } else {
                break;
            }
        } else {
            tok_str += c;
            ss.ignore();
        }
    }

    if (c == EOF)
        throw std_cells::exception::invalid_syntax(
            "Expected closing quote for string");

    tok_str += c;
    ss.ignore();

    return tok{tok::type::STRING, tok_str};
}

static tok tokenize_identifier(std::stringstream &ss) {
    str tok_str;
    enum tok::type tok_type = tok::type::CELL_REFERENCE;
    char c;

    while ((c = ss.peek()) != EOF) {
        if (isalnum(c)) {
            tok_str += c;
            ss.ignore();
        } else {
            if (c == '(') {
                tok_type = tok::type::FUNCTION;
            }
            break;
        }
    }

    return tok{tok_type, tok_str};
}

}  // namespace std_cells
