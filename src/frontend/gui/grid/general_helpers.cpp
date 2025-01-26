#include "grid.hpp"
#include "imgui.h"
#include "imgui_internal.h"

using namespace gui;
// ================================================ //
// ==================== PUBLIC ==================== //
// ================================================ //

// ==================== GETTERS ==================== //

// ================================================= //
// ==================== PRIVATE ==================== //
// ================================================= //

// ==================== SETTERS ==================== //

// ==================== GETTERS ==================== //
dim_t grid::get_default_dims() {
    float scale_factor = m_scale_percent / 100.0;
    return {DEFAULT_CELL_WIDTH * scale_factor,
            DEFAULT_CELL_HEIGHT * scale_factor};
}

float grid::get_default_spacing() {
    return DEFAULT_CELL_SPACING * (m_scale_percent / 100.0);
}

// ==================== CALCULATORS ==================== //
rect_coords grid::calc_visible_coords() {
    dim_t default_dims = get_default_dims();
    float scroll_y = m_scroll_y;
    float scroll_x = m_scroll_x;
    float window_size_y = ImGui::GetWindowSize().y;
    float window_size_x = ImGui::GetWindowSize().x;
    float sum = 0.0;

    // Rows
    int first_visible_row = default_dims.height;
    for (int row = 0; row < m_rows; row++) {
        sum += m_row_labels.at(row)->dimensions.height;
        first_visible_row = row;
        if (sum > scroll_y) {
            break;
        }
    }

    sum = 0.0;
    int last_visible_row = first_visible_row;
    for (int row = first_visible_row; row < m_rows; row++) {
        sum += m_row_labels.at(row)->dimensions.height;
        last_visible_row = row;
        if (sum * m_scale_percent / 100.0 > window_size_y) {
            break;
        }
    }

    // Columns
    sum = default_dims.width;
    int first_visible_col = 0;
    for (int col = 0; col < m_cols; col++) {
        sum += m_col_labels.at(col)->dimensions.width;
        first_visible_col = col;
        if (sum > scroll_x) {
            break;
        }
    }

    sum = 0.0;
    int last_visible_col = first_visible_col;
    for (int col = first_visible_col; col < m_cols; col++) {
        sum += m_col_labels.at(col)->dimensions.width;
        last_visible_col = col;
        if (sum * m_scale_percent / 100.0 > window_size_x) {
            break;
        }
    }

    return {pos(first_visible_row, first_visible_col),
            pos(last_visible_row, last_visible_col)};
}

float grid::calc_grid_height() {
    float height = get_default_dims().height;
    for (int row = 0; row < m_rows; row++) {
        height += m_row_labels.at(row)->dimensions.height;
    }
    return height;
}

float grid::calc_grid_width() {
    float width = get_default_dims().width;
    for (int col = 0; col < m_cols; col++) {
        width += m_col_labels.at(col)->dimensions.width;
    }
    return width;
}

float grid::scale(float value) {
    return value * (m_scale_percent / 100.0);
}

// ==================== EVENT HANDLERS ==================== //
void grid::handle_scrolling() {
    if (ImGui::IsWindowHovered()) {
        float wheely = ImGui::GetIO().MouseWheel;
        float wheelx = ImGui::GetIO().MouseWheelH;
        m_scroll_y -= wheely * SCROLL_SPEED;
        m_scroll_y = ImClamp(m_scroll_y, 0.0f, m_grid_height);
        m_scroll_x -= wheelx * SCROLL_SPEED;
        m_scroll_x = ImClamp(m_scroll_x, 0.0f, m_grid_width);
    }
}

void grid::handle_resize(axis_t axis, int index, label_props_t *label_data) {
    ImVec2 button_min = ImGui::GetItemRectMin();
    ImVec2 button_max = ImGui::GetItemRectMax();
    float tolerance = scale(3.0f);

    switch (axis) {
        case ROW: {
            bool on_bottom_edge = ImGui::IsMouseHoveringRect(
                ImVec2(button_min.x, button_max.y - tolerance),
                ImVec2(button_max.x, button_max.y + tolerance));

            if (on_bottom_edge) {
                if (ImGui::IsWindowHovered()) {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                        label_data->is_editing = true;
                    }
                }
            }

            float min_height = scale(6.0f);

            if (label_data->is_editing) {
                // Change cursor to resizer
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);

                // Draw line at cursor and across the row
                colour colour =
                    ImGui::GetMousePos().y - button_min.y >= min_height ?
                        m_colours.get("active_blue") :
                        m_colours.get("red");

                draw_info resize_line{};
                resize_line.type = LINE;
                resize_line.colour = colour;
                resize_line.p1 = ImVec2{button_min.x, ImGui::GetMousePos().y};
                resize_line.p2 =
                    ImVec2{ImGui::GetWindowSize().x, ImGui::GetMousePos().y};
                resize_line.thickness = scale(1.5f);

                m_draw_queue.push(resize_line);
            }

            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) &&
                label_data->is_editing) {
                set_row_height(
                    index, std::max(min_height,
                                    (ImGui::GetMousePos().y - button_min.y)) *
                               (1 / scale(1.0f)));
                label_data->is_editing = false;
            }
            break;
        }

        case COL: {
            bool on_right_edge = ImGui::IsMouseHoveringRect(
                ImVec2(button_max.x - tolerance, button_min.y),
                ImVec2(button_max.x + tolerance, button_max.y));

            if (on_right_edge) {
                if (ImGui::IsWindowHovered()) {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
                    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                        label_data->is_editing = true;
                    }
                }
            }

            float min_width = scale(6.0f);

            if (label_data->is_editing) {
                // Change cursor to resizer
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);

                colour colour =
                    ImGui::GetMousePos().x - button_min.x >= min_width ?
                        m_colours.get("active_blue") :
                        m_colours.get("red");

                // Draw vertical line at mouse cursor and down the column
                draw_info resize_line{};
                resize_line.type = LINE;
                resize_line.colour = colour;
                resize_line.p1 = ImVec2{ImGui::GetMousePos().x, button_min.y};
                resize_line.p2 =
                    ImVec2{ImGui::GetMousePos().x, ImGui::GetWindowSize().y};
                resize_line.thickness = scale(1.5f);

                m_draw_queue.push(resize_line);
            }

            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) &&
                label_data->is_editing) {
                set_column_width(
                    index, std::max(min_width,
                                    (ImGui::GetMousePos().x - button_min.x) *
                                        (1 / scale(1.0f))));
                label_data->is_editing = false;
            }
            break;
        }

        default: break;
    }
}

