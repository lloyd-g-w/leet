#define DEBUG

#include <iostream>
#include <string>

#include "../parser/parser.cpp"
#include "../tokenizer/tokenizer.cpp"

using namespace std;

static void print_ast(std_cells::ast_node &node,
                      const std_cells::str &prefix = "", bool is_tail = true);
static void print_ast_sexpr(const std_cells::ast_node &node);
static std_cells::str type_enum_to_string(enum std_cells::ast_struct::type t);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input>" << std::endl;
        return EXIT_FAILURE;
    }

    string input = string(argv[1]);
    std::cout << "INPUT: " << '\n' << input << '\n' << std::endl;

    auto res = std_cells::tokenize(input);
    std::cout << "TOKENS (" << res.size() << "):" << std::endl;
    std::cout << '[';
    cout << '"' << res.front().value << '"';
    int skip = 0;
    for (auto token : res) {
        if (skip++ == 0) {
            continue;
        }
        cout << ", ";
        cout << '"' << token.value << '"';
    }
    std::cout << ']' << "\n" << std::endl;

    std::cout << "AST (GRAPH):" << std::endl;
    auto ast = std_cells::parse_tokens(res);
    ast->print();
    std::cout << std::endl;

    std::cout << "AST (S-EXPR):" << std::endl;
    ast->print_sexpr();
    std::cout << std::endl;

    return 0;
}

static void print_ast(std_cells::ast_node &node, const std_cells::str &prefix,
                      bool is_tail) {
    std::cout << prefix << (is_tail ? "└── " : "├── ") << node->value << " : "
              << type_enum_to_string(node->type) << std::endl;

    for (size_t i = 0; i < node->children.size(); i++) {
        bool last_child = (i == node->children.size() - 1);
        print_ast(node->children[i], prefix + (is_tail ? "    " : "│   "),
                  last_child);
    }
}

static void print_ast_sexpr(const std_cells::ast_node &node) {
    std::cout << "(" << node->value;
    for (const auto &child : node->children) {
        std::cout << " ";
        print_ast_sexpr(child);
    }
    std::cout << ")";
}

static std_cells::str type_enum_to_string(enum std_cells::ast_struct::type t) {
    using type = enum std_cells::ast_struct::type;
    switch (t) {
        case type::FUNCTION: return "FN";
        case type::OPERATOR: return "OP";
        case type::INT: return "INT";
        case type::FLOAT: return "FLT";
        case type::STRING: return "STR";
        case type::CELL_REFERENCE: return "REF";
        default: return "UNKNOWN";
    }
}
