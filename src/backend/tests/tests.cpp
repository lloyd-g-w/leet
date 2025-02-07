#define DEBUG

#include <iostream>
#include <string>

/*#include "../evaluator/evaluator.cpp"*/
#include "../maths/maths.cpp"
#include "../parser/parser.cpp"
#include "../tokenizer/tokenizer.cpp"

using namespace std;

static void print_ast(std_cells::ast_node &node,
                      const std_cells::str &prefix = "", bool is_tail = true);
static void print_ast_sexpr(const std_cells::ast_node &node);
static std_cells::str type_enum_to_string(enum std_cells::ast_struct::type t);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input>" << endl;
        return EXIT_FAILURE;
    }

    string input = string(argv[1]);
    cout << "INPUT: " << '\n' << input << '\n' << endl;

    auto res = std_cells::tokenize(input);
    cout << "TOKENS (" << res.size() << "):" << endl;
    cout << '[';
    cout << '"' << res.front().value << '"';
    int skip = 0;
    for (auto token : res) {
        if (skip++ == 0) {
            continue;
        }
        cout << ", ";
        cout << '"' << token.value << '"';
    }
    cout << ']' << "\n" << endl;

    cout << "AST (GRAPH):" << endl;
    auto ast = std_cells::parse_tokens(res);
    ast->print();
    cout << endl;

    cout << "AST (S-EXPR):" << endl;
    ast->print_sexpr();
    cout << endl;

    /*auto eval = std_cells::evaluate(ast);*/
    /**/
    /*cout << endl;*/
    /*cout << "EVALUATION RESULT:" << endl;*/
    /*if (eval.type == std_cells::cell::type::INT ||*/
    /*    eval.type == std_cells::cell::type::DECIMAL) {*/
    /*    cout << eval.number << endl;*/
    /*} else {*/
    /*    cout << eval.string << endl;*/
    /*}*/
    /*cout << endl;*/
    return EXIT_SUCCESS;
}
