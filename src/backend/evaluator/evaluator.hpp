#pragma once

#include <optional>

#include "../common.hpp"
#include "../grid/grid.hpp"
#include "../parser/parser.hpp"

namespace std_cells {
str evaluate(const ast_node &ast, std::optional<grid> g = std::nullopt);
}
