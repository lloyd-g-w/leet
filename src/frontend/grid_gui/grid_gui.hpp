#pragma once

#include <array>

#include "../utils/colours.hpp"
#include "../window_renderer/window.hpp"
#include "holy-sheet/src/backend/cells/cells.hpp"
#include "imgui.h"

struct dim_t {
    float width, height;
};

// Constants
#define NULL_POS {-1, -1}
const int COL_LABEL = -1;
const int ROW_LABEL = -1;
const float DEFAULT_CELL_WIDTH = 75.0;
const float DEFAULT_CELL_HEIGHT = 30.0;

typedef struct gui_cell_data {
    // Variables
    bool is_editing = false;
    bool is_focused = false;

} gui_cell_data;

typedef struct gui_label_data {
    // Variables
    dim_t dimensions;
    bool is_editing = false;
    bool is_focused = false;

    // Constructor
    gui_label_data(dim_t dims = {DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT})
        : dimensions(dims) {}
} gui_label_data;

typedef struct label_details_t {
    std::vector<gui_label_data> data;

    // Constructor
    label_details_t(int size) : data(size) {}

    gui_label_data *at(int index) {
        return &data.at(index);
    }
} label_details_t;

// -------------------- GRID CLASS --------------------
class grid_gui {
  public:
    // Variables
    cells_std::pos m_active_cell = NULL_POS;

    // Main functions
    grid_gui(cells_std::cell_grid &cell_grid, window &window);
    void draw();

    // Utils
    float next_populated_dist(cells_std::pos pos, float max_dist = INFINITY);
    bool is_cell_set(cells_std::pos pos);

  private:
    // Scrolling
    static constexpr float SCROLL_SPEED = 30.0f;
    float m_scroll_x = 0.0;
    float m_scroll_y = 0.0;

    // Main data
    int m_rows, m_cols;
    cells_std::cell_grid &m_cell_grid;
    label_details_t m_gui_col_labels;
    label_details_t m_gui_row_labels;

    // Grid properties
    float m_grid_height;
    float m_grid_width;

    // Setup variables
    ImFont *m_font;
    colour_space::colour_group m_colours = colour_space::colour_group(
        {colour_space::colour_group::preset::HOLY_SHEET});
    window &m_window;

    // Private utils
    void draw_col_labels(int start_col, int end_col);
    void draw_row_label(int row_index);
    void draw_row(int row_index, int start_col, int end_col);
    void draw_cell(cells_std::pos cell_pos);
    void set_column_width(int col, float width);
    void set_row_height(int row, float height);
    std::array<cells_std::pos, 2> calc_visible_coords();
    float calc_grid_height();
    float calc_grid_width();
    void handle_scrolling();
};
