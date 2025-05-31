#pragma once

#include <array>

#include "../utils/colours/colours.hpp"
#include "lead/src/backend/common.hpp"

namespace gui {
// Standard typedefs
using str = std_cells::str;
using pos = std_cells::pos;
const pos NULL_POS = std_cells::NULL_POS;
struct dim_t {
    float width, height;
};
typedef std::array<pos, 2> rect_coords;

// Colour typedefs
using colour = colours::colour;
using palette = colours::palette;
using colour_preset = colours::palette::preset;

}  // namespace gui
