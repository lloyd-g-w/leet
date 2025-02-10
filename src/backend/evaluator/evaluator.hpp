#pragma once

#include "../common.hpp"
#include "../grid/grid.hpp"
#include "../parser/parser.hpp"

namespace std_cells {
struct eval_res {
    cell::type type;
    double number = INFINITY;
    str string;
};

eval_res evaluate(const ast_node &ast, grid &g, pos caller_cell_pos);
}  // namespace std_cells
