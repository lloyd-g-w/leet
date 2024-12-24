#include "grid.hpp"
#include "imgui_wrappers.hpp"

void Grid::draw_col_labels() {
    ImGui::Dummy(ImVec2(DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT));
    ImGui::SameLine(0.0f, 1.0f);

    for (auto col = 0; col < cols; col++) {
        std::string columnLabel = Grid::num_to_alpha(col + 1);

        ImGui::PushStyleColor(ImGuiCol_Button, colours.get("grey").imu32());
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              colours.get("blue").imu32());
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              colours.get("grey").imu32());
        ImGui::PushStyleColor(ImGuiCol_Text, colours.get("white").imu32());

        // Make dynamic sizes
        float *label_width = &col_labels.at(col)->dimensions.width;

        ImGui::Button(columnLabel.c_str(),
                      ImVec2(*label_width, DEFAULT_CELL_HEIGHT));

        ImVec2 buttonMin = ImGui::GetItemRectMin();
        ImVec2 buttonMax = ImGui::GetItemRectMax();

        bool on_right_edge =
            ImGui::IsMouseHoveringRect(ImVec2(buttonMax.x - 3.0f, buttonMin.y),
                                       ImVec2(buttonMax.x + 3.0f, buttonMax.y));

        if (on_right_edge) {
            ImGui::SetMouseCursor(
                ImGuiMouseCursor_ResizeEW);  // Change cursor to resizer

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                col_labels.at(col)->is_editing = true;
            }
        }

        if (col_labels.at(col)->is_editing) {
            if (ImGui::GetMousePos().x > buttonMin.x + 6.0f) {
                set_column_width(col, ImGui::GetMousePos().x - buttonMin.x);
            }
            ImGui::SetMouseCursor(
                ImGuiMouseCursor_ResizeEW);  // Change cursor to resizer
        }

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            col_labels.at(col)->is_editing = false;
        }

        ImGui::PopStyleColor(4);

        if (col < cols - 1) {
            ImGui::SameLine(0.0f, 1.0f);
        }
    }
}

void Grid::draw_row_label() {
    ImGui::PushStyleColor(ImGuiCol_Button, colours.get("grey").imu32());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colours.get("blue").imu32());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, colours.get("grey").imu32());
    ImGui::PushStyleColor(ImGuiCol_Text, colours.get("white").imu32());

    int row = num_rows_drawn;
    auto row_label = row_labels.at(row);
    float *row_height = &row_label->dimensions.height;

    // Make dynamic sizes
    ImGui::Button(std::to_string(num_rows_drawn + 1).c_str(),
                  ImVec2(DEFAULT_CELL_WIDTH, *row_height));

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
        if (ImGui::GetMousePos().y > buttonMin.y + 6.0f) {
            set_row_height(row, ImGui::GetMousePos().y - buttonMin.y);
        }
        ImGui::SetMouseCursor(
            ImGuiMouseCursor_ResizeNS);  // Change cursor to resizer
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        row_label->is_editing = false;
    }

    ImGui::PopStyleColor(4);
    ImGui::SameLine(0.0f, 1.0f);
}

void Grid::draw_cell() {
    int row = num_rows_drawn;
    int col = num_cols_drawn++;

    Position cell_pos = {row, col};
    auto cell_data = cells.at(row, col);

    std::string label = "##" + std::to_string(cell_pos.row) + "-" +
                        std::to_string(cell_pos.col);

    // Create cell
    ImGui::PushStyleColor(ImGuiCol_Button, colours.get("white").imu32());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colours.get("white").imu32());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, colours.get("white").imu32());
    ImGui::PushStyleColor(ImGuiCol_Text, colours.get("black").imu32());

    float cell_width = cell_data->dimensions.width;
    float cell_height = cell_data->dimensions.height;

    // Ensure there is a white rectangle behind the cell
    ImDrawList *drawList = ImGui::GetWindowDrawList();

    ImVec2 rect_min = ImGui::GetCursorPos();  // Top left
    ImVec2 rect_max = ImVec2(rect_min.x + cell_width,
                             rect_min.y + cell_height);  // Bottom right

    drawList->AddRectFilled(rect_min, rect_max, colours.get("white").imu32());

    if (cell_data->is_editing) {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, colours.get("white").imu32());

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

        std::string overlay = cell_data->cell.is_computed() ?
                                  cell_data->cell.computed_value :
                                  cell_data->cell.raw_value;

        ImVec2 textSize = ImGui::CalcTextSize(overlay.c_str());
        ImVec2 textPos =
            ImVec2(buttonMin.x + 5.0f, buttonCenter.y - textSize.y * 0.5f);

        float dist = Grid::next_populated_cell_dist(cell_pos);

        ImVec2 clip_rect_max =
            ImVec2(buttonMax.x + Grid::next_populated_cell_dist(cell_pos) -
                       ImGui::GetStyle().FramePadding.x,
                   buttonMax.y);

        drawList->PushClipRect(buttonMin, clip_rect_max, false);

        // Draw Text on Top of the Button
        drawList->AddText(textPos, colours.get("black").imu32(),
                          overlay.c_str());

        // Make raw value visible if double clicked
        if (ImGui::IsItemHovered() &&
            ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            cell_data->is_editing = true;
        }
    }

    // Pop the styles
    ImGui::PopStyleColor(4);

    // Highlight active cell
    if (active_cell == cell_pos) {
        drawList->AddRect(rect_min, rect_max, colours.get("blue").imu32(), 0.0f,
                          ImDrawFlags_None, 2.0f);
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
        active_cell = cell_pos;
    }

    if (col < cols - 1)
        ImGui::SameLine(0.0f, 1.0f);
}

void Grid::draw_row() {
    Grid::draw_row_label();
    while (num_cols_drawn < cols) {
        Grid::draw_cell();
    }
    num_rows_drawn++;
    num_cols_drawn = 0;
}

float Grid::next_populated_cell_dist(Position pos) {
    float dist = 0.0;
    for (auto curr_col = pos.col + 1; curr_col < cols; curr_col++) {
        auto curr_cell = cells.at(pos.row, curr_col);
        if (curr_cell->is_populated()) {
            return dist;
        }
        dist += curr_cell->dimensions.width;
    }

    return dist;
}

void Grid::set_column_width(int col, float width) {
    col_labels.at(col)->dimensions.width = width;
    for (auto row = 0; row < rows; row++) {
        cells.at(row, col)->dimensions.width = width;
    }
}

void Grid::set_row_height(int row, float height) {
    row_labels.at(row)->dimensions.height = height;
    for (auto col = 0; col < cols; col++) {
        cells.at(row, col)->dimensions.height = height;
    }
}

std::string Grid::num_to_alpha(int num) {
    std::string res;
    while (num > 0) {
        num--;  // Adjust to make 1 → A instead of 0 → A
        res.insert(res.begin(), 'A' + (num % 26));
        num /= 26;
    }
    return res;
}
