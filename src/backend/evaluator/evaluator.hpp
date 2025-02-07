#pragma once

#include <optional>

#include "../common.hpp"
#include "../grid/grid.hpp"
#include "../parser/parser.hpp"

namespace std_cells {
struct eval_res {
    cell::type type;
    double number = INFINITY;
    str string;
};

eval_res evaluate(const ast_node &ast, std::optional<grid> g = std::nullopt);
}  // namespace std_cells
