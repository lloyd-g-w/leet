#pragma once

#include <queue>

#include "../../window_renderer/window.hpp"
#include "../common.hpp"
#include "leet/src/backend/grid/grid.hpp"
#include "imgui.h"

namespace gui {

// ==================== TYPES ==================== //

struct cell_props_t {
    // Variables
    bool is_editing = false;
    bool is_focused = false;
    str buffer;
};

struct label_props_t {
    // Variables
    dim_t dimensions;
    bool is_editing = false;
    bool is_focused = false;

    // Funcs
    label_props_t(dim_t dims) : dimensions(dims) {}
};

struct label_group_t {
    // Variables
    std::vector<label_props_t> data;

    // Funcs
    label_group_t(int size, dim_t default_dims) : data(size, default_dims) {}
    label_props_t *at(int index) {
        return &data.at(index);
    }
};

enum axis_t {
    ROW,
    COL
};

enum draw_type {
    TEXT,
    LINE,
    RECT,
    RECT_FILLED,
    CLIP_RECT,
};

struct draw_info {
    draw_type type;
    colours::colour colour;
    str text;
    ImVec2 p1, p2;
    bool is_push = true;
    float thickness = 1.0f;
    float rounding = 0.0f;
    ImDrawFlags flags = ImDrawFlags_None;
};

// ==================== CONSTS ==================== //
constexpr float DEFAULT_CELL_WIDTH = 120;
constexpr float DEFAULT_CELL_HEIGHT = 45;
constexpr float DEFAULT_CELL_SPACING = 2;

// ==================== GRID CLASS ==================== //
class grid {
  public:
    grid(std_cells::grid &cell_grid, window &window);

    // Main methods
    void draw();

    // Getters
    int get_scale();
    dim_t get_grid_dimensions();
    pos get_active_cell();

    // Helpers
    float next_populated_dist(pos pos, float max_dist = INFINITY);
    bool is_cell_set(pos pos);

  private:
    // ==================== MEMBER VARIABLES ==================== //
    // Main grid data
    std_cells::grid &m_cell_grid;
    label_group_t m_col_labels;
    label_group_t m_row_labels;

    // Grid properties
    int m_rows, m_cols;
    float m_grid_height;
    float m_grid_width;
    int m_scale_percent = 100;
    pos m_active_cell = NULL_POS;

    // Scrolling
    static constexpr float SCROLL_SPEED = 30.0f;
    float m_scroll_x = 0.0;
    float m_scroll_y = 0.0;

    // Drawing
    std::queue<draw_info> m_draw_queue;

    // Setup variables
    ImFont *m_font;
    palette m_colours;
    window &m_window;

    // ==================== METHODS ==================== //
    // Drawing
    void draw_col_labels(int start_col, int end_col);
    void draw_row_label(int row_index);
    void draw_row(int row_index, int start_col, int end_col);
    void draw_cell(std_cells::pos cell_pos);
    void draw_single_queue();
    void draw_full_queue();

    // Style
    void push_label_style(bool is_active);
    void pop_label_style();
    void push_cell_style();
    void pop_cell_style();

    // Setters
    void set_column_width(int col, float width);
    void set_row_height(int row, float height);

    // Getters
    dim_t get_default_dims();
    float get_default_spacing();

    // Calculators
    rect_coords calc_visible_coords();
    float calc_grid_height();
    float calc_grid_width();
    float scale(float value);

    // Event handling
    void handle_scrolling();
    void handle_resize(axis_t axis, int index, label_props_t *label_data);
};

}  // namespace gui
