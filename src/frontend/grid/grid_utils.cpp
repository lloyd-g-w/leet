#include "../wrappers/imgui_wrappers.hpp"
#include "grid.hpp"
#include "imgui_internal.h"

using namespace grid_space;

void grid::draw_col_labels() {
    ImGui::Dummy(ImVec2(DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT));
    ImGui::SameLine(0.0f, 1.0f);

    for (auto col = 0; col < m_cols; col++) {
        str columnLabel = num_to_alpha(col + 1);

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

        // Make dynamic sizes
        float *label_width = &m_col_labels.at(col)->dimensions.width;

        ImGui::Button(columnLabel.c_str(),
                      ImVec2(*label_width, DEFAULT_CELL_HEIGHT));

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
                m_col_labels.at(col)->is_resizing = true;
            }
        }

        if (m_col_labels.at(col)->is_resizing) {
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
            m_col_labels.at(col)->is_resizing) {
            set_column_width(
                col, std::max(6.0f, ImGui::GetMousePos().x - buttonMin.x));
            m_col_labels.at(col)->is_resizing = false;
        }

        ImGui::PopStyleColor(4);

        if (col < m_cols - 1) {
            ImGui::SameLine(0.0f, 1.0f);
        }
    }
}

void grid::draw_row_label() {
    ImGui::PushStyleColor(ImGuiCol_Button, m_colours.get("grey").imgui());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          m_colours.get("grey").imgui());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_colours.get("grey").imgui());
    ImGui::PushStyleColor(ImGuiCol_Text, m_colours.get("white").imgui());

    int row = m_num_rows_drawn;
    auto row_label = m_row_labels.at(row);
    float *row_height = &row_label->dimensions.height;

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
    ImGui::Button(std::to_string(m_num_rows_drawn + 1).c_str(),
                  ImVec2(DEFAULT_CELL_WIDTH, *row_height));

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
            row_label->is_resizing = true;
        }
    }

    if (row_label->is_resizing) {
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
        row_label->is_resizing) {
        set_row_height(row,
                       std::max(6.0f, ImGui::GetMousePos().y - buttonMin.y));
        row_label->is_resizing = false;
    }

    ImGui::PopStyleColor(4);
    ImGui::SameLine(0.0f, 1.0f);
}

void grid::draw_cell() {
    int row = m_num_rows_drawn;
    int col = m_num_cols_drawn++;

    pos cell_pos = {row, col};
    auto cell_data = m_cells.at(row, col);

    str label = "##" + std::to_string(cell_pos.row) + "-" +
                std::to_string(cell_pos.col);

    // Create cell
    ImGui::PushStyleColor(ImGuiCol_Button, m_colours.get("white").imgui());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          m_colours.get("white").imgui());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          m_colours.get("white").imgui());
    ImGui::PushStyleColor(ImGuiCol_Text, m_colours.get("black").imgui());

    float cell_width = cell_data->dimensions.width;
    float cell_height = cell_data->dimensions.height;

    // Ensure there is a white rectangle behind the cell
    ImVec2 rect_min = ImGui::GetCursorScreenPos();
    ImVec2 rect_max = ImVec2(rect_min.x + cell_width, rect_min.y + cell_height);

    ImGui::GetWindowDrawList()->AddRectFilled(rect_min, rect_max,
                                              m_colours.get("white").imgui(),
                                              0.0f, ImDrawFlags_None);

    // Draw the cell
    if (cell_data->is_editing) {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, m_colours.get("white").imgui());

        ImGui::SetNextItemWidth(cell_width);
        ImGui::InputDynamicText(label.c_str(), &cell_data->cell.raw_value);

        ImGui::PopStyleColor();

        if (cell_data->is_focused) {
            // Parse the cell value upon deactivation
            // This check is required as setting focus
            // does not instantly activate item
            if (ImGui::IsItemDeactivated()) {
                if (cell_data->cell.raw_value.c_str()[0] == '\0') {
                    cell_data->cell.raw_value.clear();
                }
                cell_data->cell.parse();
                cell_data->is_editing = false;
                cell_data->is_focused = false;
            }
        } else {
            ImGui::SetKeyboardFocusHere(-1);
            cell_data->is_focused = true;
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

        str overlay = cell_data->cell.is_computed() ?
                          cell_data->cell.computed_value :
                          cell_data->cell.raw_value;

        ImVec2 textSize = ImGui::CalcTextSize(overlay.c_str());
        ImVec2 textPos =
            ImVec2(buttonMin.x + 5.0f, buttonCenter.y - textSize.y * 0.5f);

        float dist = next_populated_dist(cell_pos);

        ImVec2 clip_rect_max =
            ImVec2(buttonMax.x + next_populated_dist(cell_pos) -
                       ImGui::GetStyle().FramePadding.x,
                   buttonMax.y);

        drawList->PushClipRect(buttonMin, clip_rect_max, false);

        // Draw Text on Top of the Button
        drawList->AddText(textPos, m_colours.get("black").imgui(),
                          overlay.c_str());

        drawList->PopClipRect();

        // Make raw value visible if double clicked
        if (ImGui::IsItemHovered() &&
            ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            cell_data->is_editing = true;
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

    if (col < m_cols - 1)
        ImGui::SameLine(0.0f, 1.0f);
}

void grid::draw_row() {
    draw_row_label();
    while (m_num_cols_drawn < m_cols) {
        draw_cell();
    }
    m_num_rows_drawn++;
    m_num_cols_drawn = 0;
}

float grid::next_populated_dist(pos pos) {
    float dist = 0.0;
    for (auto curr_col = pos.col + 1; curr_col < m_cols; curr_col++) {
        auto curr_cell = m_cells.at(pos.row, curr_col);
        if (curr_cell->is_populated()) {
            return dist;
        }
        dist += curr_cell->dimensions.width;
    }

    return dist;
}

void grid::set_column_width(int col, float width) {
    m_col_labels.at(col)->dimensions.width = width;
    for (auto row = 0; row < m_rows; row++) {
        m_cells.at(row, col)->dimensions.width = width;
    }
}

void grid::set_row_height(int row, float height) {
    m_row_labels.at(row)->dimensions.height = height;
    for (auto col = 0; col < m_cols; col++) {
        m_cells.at(row, col)->dimensions.height = height;
    }
}

str grid::num_to_alpha(int num) {
    str res;
    while (num > 0) {
        num--;  // Adjust to make 1 → A instead of 0 → A
        res.insert(res.begin(), 'A' + (num % 26));
        num /= 26;
    }
    return res;
}
