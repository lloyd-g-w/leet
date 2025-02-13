#pragma once

#ifdef DEBUG
#include <iostream>
#endif

#include <memory>
#include <variant>

#include "../common.hpp"
#include "../tokenizer/tokenizer.hpp"

namespace std_cells {
struct ast_struct;
typedef std::unique_ptr<ast_struct> ast_node;
typedef std::deque<std::variant<tok, ast_node>> q_ast;

struct ast_struct {
    enum class type {
        // OPERATORS
        ADD,
        SUB,
        MUL,
        DIV,
        EXP,
        POS,
        NEG,
        // FUNCTIONS
        SUM,
        PRODUCT,
        // VALUES
        FUNCTION,
        OPERATOR,
        INT,
        DECIMAL,
        STRING,
        CELL_REFERENCE
    } type;
    std::variant<double, str> value;
    str value;
    std::vector<ast_node> children;

#ifdef DEBUG
    void print(const std_cells::str &prefix = "", bool is_tail = true) const {
        std::cout << prefix << (is_tail ? "└── " : "├── ") << value << " : "
                  << type_enum_to_string(type) << std::endl;

        for (size_t i = 0; i < children.size(); i++) {
            bool last_child = (i == children.size() - 1);
            print_ast(children[i], prefix + (is_tail ? "    " : "│   "),
                      last_child);
        }
    }

    void print_sexpr() const {
        std::cout << "(" << value;
        for (const auto &child : children) {
            std::cout << " ";
            print_ast_sexpr(child);
        }
        std::cout << ")";
    }

  private:
    void print_ast(const ast_node &node, const std_cells::str &prefix = "",
                   bool is_tail = true) const {
        std::cout << prefix << (is_tail ? "└── " : "├── ") << node->value
                  << " : " << type_enum_to_string(node->type) << std::endl;

        for (size_t i = 0; i < node->children.size(); i++) {
            bool last_child = (i == node->children.size() - 1);
            print_ast(node->children[i], prefix + (is_tail ? "    " : "│   "),
                      last_child);
        }
    }

    void print_ast_sexpr(const std_cells::ast_node &node) const {
        std::cout << "(" << node->value;
        for (const auto &child : node->children) {
            std::cout << " ";
            print_ast_sexpr(child);
        }
        std::cout << ")";
    }

    const str type_enum_to_string(enum type t) const {
        switch (t) {
            case type::FUNCTION: return "FN";
            case type::OPERATOR: return "OP";
            case type::INT: return "INT";
            case type::DECIMAL: return "DEC";
            case type::STRING: return "STR";
            case type::CELL_REFERENCE: return "REF";
            default: return "UNKNOWN";
        }
    }
#endif
};

ast_node parse_tokens(q_tok &tokens);

}  // namespace std_cells
