#include "../../wrappers/imgui_wrappers.hpp"
#include "grid.hpp"
#include "imgui_internal.h"

using namespace gui;

void grid::draw_col_labels(int start_col, int end_col) {
    // Create a dummy cell to align the column labels
    ImGui::Dummy(ImVec2(DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT));
    ImGui::SameLine(0.0f, DEFAULT_CELL_SPACING);

    for (auto col = start_col; col <= end_col; col++) {
        str label_str = std_cells::grid::num_to_alpha(col + 1);

        ImGui::PushStyleColor(ImGuiCol_Button, m_colours.get("grey").imgui());
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              m_colours.get("grey").imgui());
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              m_colours.get("grey").imgui());
        ImGui::PushStyleColor(ImGuiCol_Text, m_colours.get("white").imgui());

        bool is_active = m_active_cell.col == col;

        if (is_active) {
            ImGui::PushStyleColor(ImGuiCol_Button,
                                  m_colours.get("active_blue").imgui());
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                  m_colours.get("active_blue").imgui());
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                  m_colours.get("active_blue").imgui());
            ImGui::PushStyleColor(ImGuiCol_Text,
                                  m_colours.get("white").imgui());
        }

        label_props_t *label_data = m_col_labels.at(col);

        const float &label_width = label_data->dimensions.width;

        ImGui::Button(label_str.c_str(),
                      ImVec2(label_width, DEFAULT_CELL_HEIGHT));

        if (is_active) {
            ImGui::PopStyleColor(4);
        }

        ImVec2 buttonMin = ImGui::GetItemRectMin();
        ImVec2 buttonMax = ImGui::GetItemRectMax();

        bool on_right_edge =
            ImGui::IsMouseHoveringRect(ImVec2(buttonMax.x - 3.0f, buttonMin.y),
                                       ImVec2(buttonMax.x + 3.0f, buttonMax.y));

        if (on_right_edge) {
            ImGui::SetMouseCursor(
                ImGuiMouseCursor_ResizeEW);  // Change cursor to resizer

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                label_data->is_editing = true;
            }
        }

        if (label_data->is_editing) {
            ImU32 colour = m_colours.get("active_blue").imgui();
            if (ImGui::GetMousePos().x < buttonMin.x + 6.0f) {
                colour = m_colours.get("red").imgui();
            }

            // Draw vertical line at mouse cursor and down the column
            ImGui::GetForegroundDrawList()->AddLine(
                ImVec2(ImGui::GetMousePos().x, buttonMin.y),
                ImVec2(ImGui::GetMousePos().x, ImGui::GetWindowSize().y),
                colour, 1.5f);

            // Change cursor to resizer
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
        }

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) &&
            label_data->is_editing) {
            set_column_width(
                col, std::max(6.0f, ImGui::GetMousePos().x - buttonMin.x));
            label_data->is_editing = false;
        }

        ImGui::PopStyleColor(4);

        if (col < end_col) {
            ImGui::SameLine(0.0f, 1.0f);
        }
    }
}

void grid::draw_row_label(int row_index) {
    ImGui::PushStyleColor(ImGuiCol_Button, m_colours.get("grey").imgui());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          m_colours.get("grey").imgui());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_colours.get("grey").imgui());
    ImGui::PushStyleColor(ImGuiCol_Text, m_colours.get("white").imgui());

    int row = row_index;
    label_props_t *row_label = m_row_labels.at(row);
    const float &row_height = row_label->dimensions.height;

    bool is_active = m_active_cell.row == row;

    if (is_active) {
        ImGui::PushStyleColor(ImGuiCol_Button,
                              m_colours.get("active_blue").imgui());
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              m_colours.get("active_blue").imgui());
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              m_colours.get("active_blue").imgui());
        ImGui::PushStyleColor(ImGuiCol_Text, m_colours.get("white").imgui());
    }

    // Make dynamic sizes
    ImGui::Button(std::to_string(row + 1).c_str(),
                  ImVec2(DEFAULT_CELL_WIDTH, row_height));

    if (is_active) {
        ImGui::PopStyleColor(4);
    }

    ImVec2 buttonMin = ImGui::GetItemRectMin();
    ImVec2 buttonMax = ImGui::GetItemRectMax();

    bool on_bottom_edge =
        ImGui::IsMouseHoveringRect(ImVec2(buttonMin.x, buttonMax.y - 3.0f),
                                   ImVec2(buttonMax.x, buttonMax.y + 3.0f));

    if (on_bottom_edge) {
        ImGui::SetMouseCursor(
            ImGuiMouseCursor_ResizeNS);  // Change cursor to resizer

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            row_label->is_editing = true;
        }
    }

    if (row_label->is_editing) {
        ImU32 colour = m_colours.get("active_blue").imgui();
        if (ImGui::GetMousePos().y < buttonMin.y + 6.0f) {
            colour = m_colours.get("red").imgui();
        }

        // Draw line at cursor and across the row
        ImGui::GetForegroundDrawList()->AddLine(
            ImVec2(buttonMin.x, ImGui::GetMousePos().y),
            ImVec2(ImGui::GetWindowSize().x, ImGui::GetMousePos().y), colour,
            1.5f);

        // Change cursor to resizer
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) &&
        row_label->is_editing) {
        set_row_height(row,
                       std::max(6.0f, ImGui::GetMousePos().y - buttonMin.y));
        row_label->is_editing = false;
    }

    ImGui::PopStyleColor(4);
    ImGui::SameLine(0.0f, 1.0f);
}

void grid::draw_cell(pos cell_pos) {
    str label = "##" + std::to_string(cell_pos.row) + "-" +
                std::to_string(cell_pos.col);

    bool is_set = is_cell_set(cell_pos);
    float cell_width = m_col_labels.at(cell_pos.col)->dimensions.width;
    float cell_height = m_row_labels.at(cell_pos.row)->dimensions.height;
    cell_props_t *gui_data;
    std_cells::cell *cell;

    if (is_set) {
        cell = &m_cell_grid.get_cell_mut(cell_pos);
        if (cell->has_user_data())
            gui_data = &cell->get_user_data_mut<cell_props_t>();
    } else {
        gui_data = new cell_props_t();
    }

    ImGui::PushStyleColor(ImGuiCol_Button, m_colours.get("white").imgui());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          m_colours.get("white").imgui());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          m_colours.get("white").imgui());
    ImGui::PushStyleColor(ImGuiCol_Text, m_colours.get("black").imgui());

    // Ensure there is a white rectangle behind the cell
    ImVec2 rect_min = ImGui::GetCursorScreenPos();
    ImVec2 rect_max = ImVec2(rect_min.x + cell_width, rect_min.y + cell_height);

    ImGui::GetWindowDrawList()->AddRectFilled(rect_min, rect_max,
                                              m_colours.get("white").imgui(),
                                              0.0f, ImDrawFlags_None);

    // Draw the cell
    if (gui_data->is_editing) {
        str &buffer = m_cell_grid.get_cell_mut(cell_pos).get_raw_mut();

        ImGui::PushStyleColor(ImGuiCol_FrameBg, m_colours.get("white").imgui());

        ImGui::SetNextItemWidth(cell_width);
        ImGui::InputDynamicText(label.c_str(), &buffer);

        ImGui::PopStyleColor();

        if (gui_data->is_focused) {
            // This check is required as setting focus
            // does not activate item in the same frame
            if (ImGui::IsItemDeactivated()) {
                if (buffer.c_str()[0] == '\0') {
                    m_cell_grid.delete_cell(cell_pos);
                } else {
                    gui_data->is_editing = false;
                    gui_data->is_focused = false;
                }
            }
        } else {
            ImGui::SetKeyboardFocusHere(-1);
            gui_data->is_focused = true;
        }
    } else {
        ImGui::Button(label.c_str(), ImVec2(cell_width, cell_height));

        // Step 2: Get Button Position and Size
        ImVec2 buttonMin = ImGui::GetItemRectMin();
        ImVec2 buttonMax = ImGui::GetItemRectMax();
        ImVec2 buttonCenter = ImVec2((buttonMin.x + buttonMax.x) * 0.5f,
                                     (buttonMin.y + buttonMax.y) * 0.5f);

        // Step 3: Draw Overlay Text Directly with DrawList
        ImDrawList *drawList = ImGui::GetForegroundDrawList();

        str overlay;

        if (!is_set) {
            overlay = "";
        } else {
            overlay = cell->get_raw();
        }

        ImVec2 text_size = ImGui::CalcTextSize(overlay.c_str());
        ImVec2 text_pos =
            ImVec2(buttonMin.x + 5.0f, buttonCenter.y - text_size.y * 0.5f);

        float dist = next_populated_dist(cell_pos, text_size.x);

        ImVec2 clip_rect_max = ImVec2(
            buttonMax.x + dist - ImGui::GetStyle().FramePadding.x, buttonMax.y);

        drawList->PushClipRect(buttonMin, clip_rect_max, false);

        // Draw Text on Top of the Button
        drawList->AddText(text_pos, m_colours.get("black").imgui(),
                          overlay.c_str());

        drawList->PopClipRect();

        // Make raw value visible if double clicked
        if (ImGui::IsItemHovered() &&
            ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            gui_data->is_editing = true;
            if (!is_set) {
                if (!is_set) {
                    m_cell_grid.create_cell(cell_pos);
                    m_cell_grid.get_cell_mut(cell_pos)
                        .set_user_data<cell_props_t>(*gui_data);
                }
            }
        }
    }

    // Pop the styles
    ImGui::PopStyleColor(4);

    // Highlight active cell
    if (m_active_cell == cell_pos) {
        ImGui::GetWindowDrawList()->AddRect(
            rect_min, rect_max, m_colours.get("active_blue").imgui(), 0.0f,
            ImDrawFlags_None, 2.0f);
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
        m_active_cell = cell_pos;
    }

    if (!is_set) {
        delete gui_data;
    }
}

void grid::draw_row(int row_index, int start_col, int end_col) {
    draw_row_label(row_index);
    for (auto col = start_col; col <= end_col; col++) {
        draw_cell({row_index, col});
        if (col < end_col)
            ImGui::SameLine(0.0f, 1.0f);
    }
}

float grid::next_populated_dist(pos pos, float max_dist) {
    float dist = 0.0;

    for (auto curr_col = pos.col + 1; curr_col < m_cols; curr_col++) {
        if (dist >= max_dist) {
            return max_dist;
        }
        if (is_cell_set({pos.row, curr_col})) {
            return dist;
        }

        dist += m_col_labels.at(curr_col)->dimensions.width;
    }

    return dist;
}

bool grid::is_cell_set(pos pos) {
    try {
        m_cell_grid.get_cell(pos);
        return true;
    } catch (std_cells::cell_exception e) {
        return false;
    }
}

void grid::set_column_width(int col, float width) {
    float diff = width - m_col_labels.at(col)->dimensions.width;
    m_grid_width += diff;

    m_col_labels.at(col)->dimensions.width = width;
}

void grid::set_row_height(int row, float height) {
    float diff = height - m_row_labels.at(row)->dimensions.height;
    m_grid_height += diff;

    m_row_labels.at(row)->dimensions.height = height;
}

rect_coords grid::calc_visible_coords() {
    float scroll_y = m_scroll_y;
    float scroll_x = m_scroll_x;
    float window_size_y = ImGui::GetWindowSize().y;
    float window_size_x = ImGui::GetWindowSize().x;
    float sum = 0.0;

    // Rows
    int first_visible_row = DEFAULT_CELL_HEIGHT;
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
        if (sum > window_size_y) {
            break;
        }
    }

    // Columns
    sum = DEFAULT_CELL_WIDTH;
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
        if (sum > window_size_x) {
            break;
        }
    }

    return {pos(first_visible_row, first_visible_col),
            pos(last_visible_row, last_visible_col)};
}

float grid::calc_grid_height() {
    float height = DEFAULT_CELL_HEIGHT;
    for (int row = 0; row < m_rows; row++) {
        height += m_row_labels.at(row)->dimensions.height;
    }
    return height;
}

float grid::calc_grid_width() {
    float width = DEFAULT_CELL_WIDTH;
    for (int col = 0; col < m_cols; col++) {
        width += m_col_labels.at(col)->dimensions.width;
    }
    return width;
}

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
