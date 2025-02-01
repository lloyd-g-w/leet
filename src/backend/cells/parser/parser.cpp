#include <memory>

#include "../common.hpp"
#include "../tokenizer/tokenizer.hpp"

namespace std_cells {

struct ast_node {
    enum class type {
        OPERATOR,
        INT,
        FLOAT,
        STRING,
        IDENTIFIER,
    } type;

    str value;
    std::vector<std::unique_ptr<ast_node>> children;
};

void parse(v_tok tokens) {
    ast_node head;

    for (auto &token : tokens) {
        switch (token.type) {
            case tok::type::PUNCTUATION:
                break;
            case tok::type::OPERATOR:
                break;
            case tok::type::INT:
                break;
            case tok::type::FLOAT:
                break;
            case tok::type::STRING:
                break;
            case tok::type::IDENTIFIER:
                break;
        }
    }

}

}  // namespace std_cells
