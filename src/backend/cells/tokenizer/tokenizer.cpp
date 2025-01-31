#include <cctype>
#include <sstream>

#include "../common.hpp"

namespace std_cells {

static v_str tokenize(str string);
static bool isparen(char c);
static bool isquote(char c);
static bool isoperator(char c);
static str tokenize_number(std::stringstream &ss);
static str tokenize_string(std::stringstream &ss);
static str tokenize_identifier(std::stringstream &ss);

// Tokenize a string | can throw an exception
v_str tokenize(str string) {
    v_str tokens;

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
            tokens.push_back(str(1, c));
            ss.ignore();
        } else if (isquote(c)) {
            tokens.push_back(str(1, c));
            tokens.push_back(tokenize_string(ss));
            ss >> c;
            tokens.push_back(str(1, c));
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

static str tokenize_number(std::stringstream &ss) {
    str token;
    char c;

    while ((c = ss.peek()) != EOF) {
        if (isdigit(c) || c == '.') {
            token += c;
            ss.ignore();
        } else {
            break;
        }
    }

    return token;
}

static str tokenize_string(std::stringstream &ss) {
    str token;
    char c;

    char quote_type;
    ss >> quote_type;

    while ((c = ss.peek()) != EOF) {
        if (c == quote_type) {
            if (token.back() == '\\') {
                token.pop_back();
                token += c;
                ss.ignore();
            } else {
                break;
            }
        } else {
            token += c;
            ss.ignore();
        }
    }

    if (c == EOF)
        throw std_cells::exception::invalid_syntax(
            "Expected closing quote for string");

    return token;
}

static str tokenize_identifier(std::stringstream &ss) {
    str token;
    char c;

    while ((c = ss.peek()) != EOF) {
        if (isalnum(c)) {
            token += c;
            ss.ignore();
        } else {
            break;
        }
    }

    return token;
}

}  // namespace std_cells
