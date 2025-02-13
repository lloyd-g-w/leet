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

struct eval_input {
    ast_node &ast;
    grid &g;
    pos caller_cell_pos;
};

eval_res evaluate(const eval_input &input);

class evaluator {
  public:
    evaluator(grid &g) : m_grid(g) {}

    eval_res evaluate(const eval_input &input);

  private:
    grid &m_grid;
};

}  // namespace std_cells
