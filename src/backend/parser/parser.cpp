#include "parser.hpp"
#include <iostream>

namespace std_cells {

// BEGIN PROTOTYPES
static ast_node parse(q_ast &queue);
static void parse_arguments(ast_node &parent, q_ast &queue);
static ast_node parse_generic(std::variant<tok, ast_node> &item);
static ast_node parse_number(std::variant<tok, ast_node> &item);

static q_ast construct_ast_queue(q_tok &tokens);
static q_ast pop_sub_queue(q_ast &queue, const size_t starting_index,
                           const size_t closing_index);
static size_t get_closing_paren_index(const q_ast &queue,
                                      const size_t starting_index);
// END PROTOTYPES

ast_node parse_tokens(q_tok &tokens) {
    q_ast queue = construct_ast_queue(tokens);
    return parse(queue);
}

static ast_node parse(q_ast &queue) {
    // Cell references

    // Cell ranges

    // Function
    for (size_t i = 0; i < queue.size(); i++) {
        if (std::holds_alternative<tok>(queue.at(i))) {
            auto &token = std::get<tok>(queue.at(i));
            if (token.type == tok::type::FUNCTION) {
                if (!std::holds_alternative<tok>(queue.at(i + 1)))
                    throw exception::cell_exception(
                        "Error in function argument parsing");

                const int closing_paren_index =
                    get_closing_paren_index(queue, i + 2);

                ast_node identifier = std::make_unique<ast_struct>();
                identifier->type = ast_struct::type::FUNCTION;
                identifier->value = token.value;

                // Pop the arguments into a sub queue, excluding the parentheses
                q_ast sub_queue =
                    pop_sub_queue(queue, i + 2, closing_paren_index - 1);

                parse_arguments(identifier, sub_queue);

                // Erase the parentheses and the function name
                queue.erase(queue.begin() + i, queue.begin() + i + 3);

                queue.insert(queue.begin() + i, std::move(identifier));
            }
        }
    }

    // Parentheses
    for (size_t i = 0; i < queue.size(); i++) {
        if (std::holds_alternative<tok>(queue.at(i))) {
            auto &token = std::get<tok>(queue.at(i));
            if (token.value == "(") {
                q_ast sub_queue = pop_sub_queue(
                    queue, i + 1, get_closing_paren_index(queue, i + 1) - 1);

                // Erase the parentheses
                queue.erase(queue.begin() + i, queue.begin() + i + 2);

                ast_node grouping = parse(sub_queue);
                queue.insert(queue.begin() + i, std::move(grouping));
            }
        }
    }

    // Unary operators
    // TODO: Refactor this
    for (size_t i = 0; i < queue.size(); i++) {
        if (std::holds_alternative<tok>(queue.at(i))) {
            auto &token = std::get<tok>(queue.at(i));
            if (token.value == "+" || token.value == "-") {
                if (i == 0) {
                    ast_node unary = std::make_unique<ast_struct>();
                    unary->type = ast_struct::type::OPERATOR;
                    unary->value = token.value == "+" ? "POS" : "NEG";

                    unary->children.push_back(parse_number(queue.at(i + 1)));
                    queue.erase(queue.begin() + i, queue.begin() + i + 2);
                    queue.insert(queue.begin() + i, std::move(unary));
                } else if (std::holds_alternative<tok>(queue.at(i - 1))) {
                    auto &prev = std::get<tok>(queue.at(i - 1));
                    if (prev.type != tok::type::INT &&
                        prev.type != tok::type::DECIMAL) {
                        ast_node unary = std::make_unique<ast_struct>();
                        unary->type = ast_struct::type::OPERATOR;
                        unary->value = token.value == "+" ? "POS" : "NEG";

                        unary->children.push_back(
                            parse_number(queue.at(i + 1)));
                        queue.erase(queue.begin() + i, queue.begin() + i + 2);
                        queue.insert(queue.begin() + i, std::move(unary));
                    }
                }
            }
        }
    }

    // Exponents
    for (size_t i = 0; i < queue.size(); i++) {
        if (std::holds_alternative<tok>(queue.at(i))) {
            auto &token = std::get<tok>(queue.at(i));
            if (token.value == "^") {
                ast_node exponent = std::make_unique<ast_struct>();
                exponent->type = ast_struct::type::OPERATOR;
                exponent->value = "POW";
                exponent->children.push_back(parse_number(queue.at(i - 1)));
                exponent->children.push_back(parse_number(queue.at(i + 1)));

                queue.erase(queue.begin() + i - 1, queue.begin() + i + 2);
                queue.insert(queue.begin() + i - 1, std::move(exponent));
                i--;
            }
        }
    }

    // Multiplication and Division
    for (size_t i = 0; i < queue.size(); i++) {
        if (std::holds_alternative<tok>(queue.at(i))) {
            auto &token = std::get<tok>(queue.at(i));
            if (token.value == "*" || token.value == "/") {
                ast_node mul_div = std::make_unique<ast_struct>();
                mul_div->type = ast_struct::type::OPERATOR;
                mul_div->value = token.value == "*" ? "MUL" : "DIV";

                mul_div->children.push_back(parse_number(queue.at(i - 1)));
                mul_div->children.push_back(parse_number(queue.at(i + 1)));

                queue.erase(queue.begin() + i - 1, queue.begin() + i + 2);
                queue.insert(queue.begin() + i - 1, std::move(mul_div));
                i--;
            }
        }
    }

    // Addition and Subtraction
    for (size_t i = 0; i < queue.size(); i++) {
        if (std::holds_alternative<tok>(queue.at(i))) {
            auto &token = std::get<tok>(queue.at(i));
            if (token.value == "+" || token.value == "-") {
                ast_node add_sub = std::make_unique<ast_struct>();
                add_sub->type = ast_struct::type::OPERATOR;
                add_sub->value = token.value == "+" ? "ADD" : "SUB";

                add_sub->children.push_back(parse_number(queue.at(i - 1)));
                add_sub->children.push_back(parse_number(queue.at(i + 1)));

                queue.erase(queue.begin() + i - 1, queue.begin() + i + 2);
                queue.insert(queue.begin() + i - 1, std::move(add_sub));
                i--;
            }
        }
    }

    if (queue.size() != 1)
        throw exception::cell_exception("Invalid expression - too many tokens");

    // If the last token is a number or string just return it
    if (std::holds_alternative<tok>(queue.front()))
        return parse_generic(queue.front());

    return std::move(std::get<ast_node>(queue.front()));
}

static void parse_arguments(ast_node &parent, q_ast &queue) {
    // Parse the first n-1 arguments
    int inner_func_end = -1;
    for (int i = 0; i < queue.size(); i++) {
        if (!std::holds_alternative<tok>(queue.at(i))) {
            continue;
        }

        auto &token = std::get<tok>(queue.at(i));

        if (i > inner_func_end && token.type == tok::type::FUNCTION) {
            inner_func_end = get_closing_paren_index(queue, i + 2);
        }

        if (i > inner_func_end && token.value == ",") {
            if (i == 1) {
                parent->children.push_back(parse_generic(queue.at(i - 1)));

                // Erase the comma and the argument
                queue.erase(queue.begin(), queue.begin() + i + 1);
            } else {
                q_ast sub_queue = pop_sub_queue(queue, 0, i - 1);

                // Print the sub queue
                parent->children.push_back(parse(sub_queue));
                // Erase the comma
                queue.erase(queue.begin());
            }

            i = -1;
            inner_func_end = -1;
        }
    }

    // Parse the last argument
    if (queue.size() == 1)
        parent->children.push_back(parse_generic(queue.front()));
    else {
        q_ast sub_queue = pop_sub_queue(queue, 0, queue.size() - 1);
        parent->children.push_back(parse(sub_queue));
    }
}

static ast_node parse_generic(std::variant<tok, ast_node> &item) {
    if (std::holds_alternative<ast_node>(item)) {
        return std::move(std::get<ast_node>(item));
    }

    auto &token = std::get<tok>(item);
    ast_node generic = std::make_unique<ast_struct>();
    if (token.type == tok::type::INT)
        generic->type = ast_struct::type::INT;
    else if (token.type == tok::type::DECIMAL)
        generic->type = ast_struct::type::DECIMAL;
    else if (token.type == tok::type::STRING)
        generic->type = ast_struct::type::STRING;
    else if (token.type == tok::type::CELL_REFERENCE)
        generic->type = ast_struct::type::CELL_REFERENCE;
    else
        throw exception::cell_exception("Invalid generic type");

    generic->value = token.value;

    return generic;
}

static ast_node parse_number(std::variant<tok, ast_node> &item) {
    if (std::holds_alternative<ast_node>(item)) {
        return std::move(std::get<ast_node>(item));
    }

    auto &token = std::get<tok>(item);
    ast_node number = std::make_unique<ast_struct>();
    if (token.type == tok::type::INT)
        number->type = ast_struct::type::INT;
    else if (token.type == tok::type::DECIMAL)
        number->type = ast_struct::type::DECIMAL;
    else if (token.type == tok::type::CELL_REFERENCE)
        number->type = ast_struct::type::CELL_REFERENCE;
    else
        throw exception::cell_exception("Invalid number type");

    number->value = token.value;

    return number;
}

static q_ast construct_ast_queue(q_tok &tokens) {
    q_ast ast_queue;

    while (!tokens.empty()) {
        ast_queue.push_back(std::move(tokens.front()));
        tokens.pop_front();
    }

    return ast_queue;
}

// Pop the closed sub queue [starting_index, final_index]
static q_ast pop_sub_queue(q_ast &queue, const size_t starting_index,
                           const size_t final_index) {
    q_ast sub_queue;

    for (size_t i = starting_index; i <= final_index; i++) {
        sub_queue.push_back(std::move(queue.at(i)));
    }

    queue.erase(queue.begin() + starting_index,
                queue.begin() + final_index + 1);

    return sub_queue;
}

static size_t get_closing_paren_index(const q_ast &queue,
                                      const size_t starting_index) {
    size_t count = 1;
    for (size_t i = starting_index; i < queue.size(); i++) {
        if (std::holds_alternative<tok>(queue.at(i))) {
            auto &token = std::get<tok>(queue.at(i));
            if (token.value == "(") {
                count++;
            } else if (token.value == ")") {
                count--;
            }
        }
        if (count == 0)
            return i;
    }

    throw exception::cell_exception("No closing parenthesis found");
}

}  // namespace std_cells
