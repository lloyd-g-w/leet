#include "../../wrappers/imgui_wrappers.hpp"
#include "grid.hpp"
#include "imgui.h"
#include "imgui_internal.h"
using namespace gui;

void grid::draw_col_labels(int start_col, int end_col) {
    // Create a dummy cell to align the column labels
    dim_t default_dims = get_default_dims();
    ImGui::Dummy(ImVec2(default_dims.width, default_dims.height));
    ImGui::SameLine();

    for (auto col_index = start_col; col_index <= end_col; col_index++) {
        // Get label data
        bool is_active = m_active_cell.col == col_index;
        label_props_t *label_data = m_col_labels.at(col_index);
        str label_text = std_cells::grid::num_to_alpha(col_index + 1);

        // Create button
        push_label_style(is_active);
        ImGui::Button(
            label_text.c_str(),
            ImVec2(scale(label_data->dimensions.width), default_dims.height));
        pop_label_style();

        // Handle events
        handle_resize(COL, col_index, label_data);

        if (col_index < end_col) {
            ImGui::SameLine();
        }
    }
}

void grid::draw_row_label(int row_index) {
    // Get label data
    dim_t default_dims = get_default_dims();
    label_props_t *label_data = m_row_labels.at(row_index);
    bool is_active = m_active_cell.row == row_index;

    // Create button
    push_label_style(is_active);
    ImGui::Button(
        std::to_string(row_index + 1).c_str(),
        ImVec2(default_dims.width, scale(label_data->dimensions.height)));
    pop_label_style();

    // Handle events
    handle_resize(ROW, row_index, label_data);

    ImGui::SameLine();
}

void grid::draw_cell(pos cell_pos) {
    // Get cell data
    bool is_set = is_cell_set(cell_pos);
    float cell_width = scale(m_col_labels.at(cell_pos.col)->dimensions.width);
    float cell_height = scale(m_row_labels.at(cell_pos.row)->dimensions.height);

    // Get gui data for the cell if it exists
    cell_props_t *gui_data;
    if (is_set) {
        std_cells::cell *cell = &m_cell_grid.get_cell_mut(cell_pos);
        if (cell->has_user_data())
            gui_data = &cell->get_user_data_mut<cell_props_t>();
    } else {
        gui_data = new cell_props_t();
    }

    str cell_label = "##" + std::to_string(cell_pos.row) + "-" +
                     std::to_string(cell_pos.col);

    ImVec2 rect_min = ImGui::GetCursorScreenPos();
    ImVec2 rect_max = ImVec2(rect_min.x + cell_width, rect_min.y + cell_height);

    // Draw the cell
    if (gui_data->is_editing) {
        str &buffer = m_cell_grid.get_cell_mut(cell_pos).get_raw_mut();

        // This ensures there is a white backgroud
        // even when the textbox isnt the correct height
        push_cell_style();
        ImGui::InputSizedDynamicText(cell_label.c_str(), &buffer,
                                     ImVec2(cell_width, cell_height));
        pop_cell_style();

        if (gui_data->is_focused) {
            // This check is required as setting focus
            // does not activate item in the same frame
            if (ImGui::IsItemDeactivated()) {
                if (buffer.c_str()[0] == '\0') {
                    m_cell_grid.delete_cell(cell_pos);
                } else {
                    // Parse cell?
                    gui_data->is_editing = false;
                    gui_data->is_focused = false;
                }
            }
        } else {
            ImGui::SetKeyboardFocusHere(-1);
            gui_data->is_focused = true;
        }
    } else {
        push_cell_style();
        ImGui::Button(cell_label.c_str(), ImVec2(cell_width, cell_height));
        pop_cell_style();

        ImVec2 button_min = ImGui::GetItemRectMin();
        ImVec2 button_max = ImGui::GetItemRectMax();
        ImVec2 button_center = ImVec2((button_min.x + button_max.x) * 0.5f,
                                      (button_min.y + button_max.y) * 0.5f);

        str overlay =
            is_set ? m_cell_grid.get_cell_mut(cell_pos).get_raw() : "";

        if (overlay != "") {
            float padding = scale(ImGui::GetStyle().FramePadding.x);

            ImVec2 text_size = ImGui::CalcTextSize(overlay.c_str());
            ImVec2 text_pos = ImVec2(button_min.x + padding,
                                     button_center.y - text_size.y * 0.5f);

            float dist = next_populated_dist(cell_pos, text_size.x);

            auto clip_rect = draw_info();
            clip_rect.type = CLIP_RECT;
            clip_rect.p1 = button_min;
            clip_rect.p2 = ImVec2{button_max.x + dist - padding, button_max.y};

            auto text_item = draw_info();
            text_item.type = TEXT;
            text_item.p1 = text_pos;
            text_item.colour = m_colours.get("black");
            text_item.text = overlay;

            m_draw_queue.push(clip_rect);
            m_draw_queue.push(text_item);
            clip_rect.is_push = false;
            m_draw_queue.push(clip_rect);
        }

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

    float highlight_width = scale(0.25f);

    // Highlight active cell
    if (m_active_cell == cell_pos) {
        ImGui::GetWindowDrawList()->AddRect(
            {rect_min.x + highlight_width, rect_min.y + highlight_width},
            {rect_max.x - highlight_width, rect_max.y - highlight_width},
            m_colours.get("active_blue").imgui(), 0.0f, ImDrawFlags_None,
            highlight_width * 2 + 2.0f);
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
            ImGui::SameLine();
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

void grid::draw_single_queue() {
    auto item = m_draw_queue.front();

    switch (item.type) {
        case LINE: {
            ImGui::GetWindowDrawList()->AddLine(
                item.p1, item.p2, item.colour.imgui(), item.thickness);
            break;
        }
        case TEXT: {
            ImGui::GetWindowDrawList()->AddText(item.p1, item.colour.imgui(),
                                                item.text.c_str());
            break;
        }
        case RECT_FILLED: {
            ImGui::GetWindowDrawList()->AddRectFilled(
                item.p1, item.p2, item.colour.imgui(), item.rounding,
                item.flags);
            break;
        }
        case RECT: {
            ImGui::GetWindowDrawList()->AddRect(
                item.p1, item.p2, item.colour.imgui(), item.rounding,
                item.flags, item.thickness);
            break;
        }
        case CLIP_RECT: {
            if (!item.is_push) {
                ImGui::PopClipRect();
                break;
            }

            ImGui::GetWindowDrawList()->PushClipRect(item.p1, item.p2, false);
            break;
        }
        default: break;
    }

    m_draw_queue.pop();
};

void grid::draw_full_queue() {
    while (!m_draw_queue.empty()) {
        draw_single_queue();
    }
};

// ==================== STYLE ==================== //
void grid::push_label_style(bool is_active) {
    colour text_colour = m_colours.get("white");
    colour button_colour =
        is_active ? m_colours.get("active_blue") : m_colours.get("grey");

    ImGui::PushStyleColor(ImGuiCol_Button, button_colour.imgui());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_colour.imgui());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_colour.imgui());
    ImGui::PushStyleColor(ImGuiCol_Text, text_colour.imgui());
}

void grid::pop_label_style() {
    ImGui::PopStyleColor(4);
}

void grid::push_cell_style() {
    colour text_colour = m_colours.get("black");
    colour bg_colour = m_colours.get("white");

    ImGui::PushStyleColor(ImGuiCol_Button, bg_colour.imgui());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, bg_colour.imgui());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, bg_colour.imgui());
    ImGui::PushStyleColor(ImGuiCol_FrameBg, bg_colour.imgui());
    ImGui::PushStyleColor(ImGuiCol_Text, text_colour.imgui());
}

void grid::pop_cell_style() {
    ImGui::PopStyleColor(5);
}
