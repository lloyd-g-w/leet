#include "grid.hpp"
#include <cstring>

#include "../backend/cell.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_internal.h"
#include "imgui_wrappers.hpp"
#include "window.hpp"

Grid::Grid(int rows, int cols, Window &window) : window(window) {
    this->rows = rows;
    this->cols = cols;

    // Create cells
    for (auto row = 0; row < rows; row++) {
        std::vector<Grid::cell_data> cell_row;

        for (auto col = 0; col < cols; col++) {
            auto cell_data = Grid::cell_data({row, col});
            cell_row.push_back(cell_data);
        }

        cells.push_row(cell_row);
    }

    // Set style and get font
    ImGuiStyle &style = ImGui::GetStyle();
    style.ItemSpacing = ImVec2(1.0f, 1.0f);

    ImGuiIO &io = ImGui::GetIO();
    jetbrains_mono = io.Fonts->AddFontFromFileTTF(
        "src/fonts/JetBrainsMono-Regular.ttf",
        DEFAULT_CELL_HEIGHT - ImGui::GetStyle().FramePadding.y * 2.0f -
            ImGui::GetStyle().FrameBorderSize * 2.0f);

    io.FontDefault = jetbrains_mono;

    colours.add_preset(Colours::BASIC_COLOURS);
};

void Grid::draw() {
    if (!window.should_close()) {
        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImU32 COLOR_DARK_GREY = colours.get("dark_grey").imu32();
        ImU32 COLOR_WHITE = colours.get("white").imu32();
        ImU32 COLOR_BLACK = colours.get("black").imu32();

        {
            // Make the window fill the entire screen
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

            ImGui::PushStyleColor(ImGuiCol_WindowBg, COLOR_DARK_GREY);

            ImGui::Begin(
                "Cells", nullptr,
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                    ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoNav);

            ImGui::PopStyleColor();

            // Set general cell style
            ImGui::PushStyleColor(ImGuiCol_FrameBg, COLOR_WHITE);
            ImGui::PushStyleColor(ImGuiCol_Text, COLOR_BLACK);
            ImGui::PushItemWidth(DEFAULT_CELL_WIDTH);

            Grid::draw_col_labels();

            while (num_rows_drawn < rows) {
                Grid::draw_row();
            }

            // Pop cell style
            ImGui::PopItemWidth();
            ImGui::PopStyleColor(2);

            ImGui::End();

            Grid::reset_for_next_draw();
        }
    }
}

void Grid::draw_col_labels() {
    for (auto col = -1; col < cols; col++) {
        if (col == -1) {
            ImGui::Dummy(ImVec2(DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT));
            ImGui::SameLine(0.0f, 1.0f);
            continue;
        }

        std::string columnLabel(1, 'A' + col);

        ImGui::PushStyleColor(ImGuiCol_Button, colours.get("grey").imu32());
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              colours.get("blue").imu32());
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              colours.get("grey").imu32());
        ImGui::PushStyleColor(ImGuiCol_Text, colours.get("white").imu32());

        // Make dynamic sizes
        ImGui::Button(columnLabel.c_str(),
                      ImVec2(DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT));

        ImGui::PopStyleColor(4);

        if (col < cols - 1)
            ImGui::SameLine(0.0f, 1.0f);
    }
}

void Grid::draw_row_label() {
    ImGui::PushStyleColor(ImGuiCol_Button, colours.get("grey").imu32());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colours.get("blue").imu32());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, colours.get("grey").imu32());
    ImGui::PushStyleColor(ImGuiCol_Text, colours.get("white").imu32());

    // Make dynamic sizes
    ImGui::Button(std::to_string(num_rows_drawn+1).c_str(),
                  ImVec2(DEFAULT_CELL_WIDTH, DEFAULT_CELL_HEIGHT));

    ImGui::PopStyleColor(4);
    ImGui::SameLine(0.0f, 1.0f);
}

void Grid::draw_cell() {
    int row = num_rows_drawn;
    int col = num_cols_drawn++;

    Position cell_pos = {row, col};

    std::string label =
        "##" + std::to_string(cell_pos.x) + "-" + std::to_string(cell_pos.y);

    // Highlight active cell
    if (active_cell == cell_pos) {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, colours.get("blue").imu32());
    }

    // Create cell
    ImGui::InputDynamicText(label.c_str(), &cells.at(row, col)->buffer);

    // Pop style
    if (active_cell == cell_pos) {
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }

    // Make a single click do nothing
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
        this->active_cell = {row, col};
        if (ImGui::IsItemActive()) {
            ImGui::ClearActiveID();
        }
    }
    // Make raw value visible if double clicked
    if (ImGui::IsItemHovered() &&
        ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
        cells.at(row, col)->buffer = cells.at(row, col)->cell.raw_value;
        ImGui::SetKeyboardFocusHere(-1);
    }

    // Parse the cell value upon deactivation
    if (ImGui::IsItemDeactivated()) {
        cells.at(row, col)->cell.raw_value = cells.at(row, col)->buffer;
        if (cells.at(row, col)->cell.parse()) {
            cells.at(row, col)->buffer =
                cells.at(row, col)->cell.computed_value;
        }
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
