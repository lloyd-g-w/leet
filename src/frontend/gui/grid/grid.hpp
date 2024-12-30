#pragma once

#include "../../window_renderer/window.hpp"
#include "../common.hpp"
#include "holy-sheet/src/backend/cells/cells.hpp"
#include "imgui.h"

namespace gui {

class grid {
  public:
    // Variables
    pos m_active_cell = NULL_POS;

    // Main functions
    grid(std_cells::grid &cell_grid, window &window);
    void draw();

    // Utils
    float next_populated_dist(pos pos, float max_dist = INFINITY);
    bool is_cell_set(pos pos);

  private:
    float SCALE_FACTOR = 1.5;
    float DEFAULT_CELL_WIDTH = 75.0 * SCALE_FACTOR;
    float DEFAULT_CELL_HEIGHT = 30.0 * SCALE_FACTOR;
    float DEFAULT_CELL_SPACING = 1.0 * SCALE_FACTOR;

    // Typedefs
    struct cell_props_t {
        // Variables
        bool is_editing = false;
        bool is_focused = false;
    };

    struct label_props_t {
        // Variables
        dim_t dimensions;
        bool is_editing = false;
        bool is_focused = false;

        // Constructor
        label_props_t(dim_t dims) : dimensions(dims) {}
    };

    struct label_group_t {
        std::vector<label_props_t> data;

        // Constructor
        label_group_t(int size, dim_t default_dims)
            : data(size, default_dims) {}

        label_props_t *at(int index) {
            return &data.at(index);
        }
    };

    // Scrolling
    static constexpr float SCROLL_SPEED = 30.0f;
    float m_scroll_x = 0.0;
    float m_scroll_y = 0.0;

    // Main data
    std_cells::grid &m_cell_grid;
    label_group_t m_col_labels;
    label_group_t m_row_labels;

    // Grid properties
    int m_rows, m_cols;
    float m_grid_height;
    float m_grid_width;

    // Setup variables
    ImFont *m_font;
    palette m_colours = {colour_preset::HOLY_SHEET};
    window &m_window;

    // Private utils
    void draw_col_labels(int start_col, int end_col);
    void draw_row_label(int row_index);
    void draw_row(int row_index, int start_col, int end_col);
    void draw_cell(std_cells::pos cell_pos);
    void set_column_width(int col, float width);
    void set_row_height(int row, float height);
    rect_coords calc_visible_coords();
    float calc_grid_height();
    float calc_grid_width();
    void handle_scrolling();
};

}  // namespace gui
