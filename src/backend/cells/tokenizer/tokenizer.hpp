#include "../common.hpp"

namespace std_cells {

struct tok {
    enum class type {
        PUNCTUATION,
        OPERATOR,
        INT,
        FLOAT,
        STRING,
        IDENTIFIER
    } type;
    str value;
};

typedef std::vector<tok> v_tok;
v_tok tokenize(str string);
}  // namespace std_cells
