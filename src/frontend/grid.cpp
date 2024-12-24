#include "grid.hpp"
#include <cstring>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "window.hpp"

Grid::Grid(int rows, int cols, Window &window) : window(window) {
    this->rows = rows;
    this->cols = cols;
    // Create cells
    for (auto row = 0; row < rows; row++) {
        std::vector<Grid::cell_data> cell_row;

        auto row_label = Grid::label_data(row);
        row_labels.push(row_label);

        for (auto col = 0; col < cols; col++) {
            auto cell_data = Grid::cell_data({row, col});
            cell_row.push_back(cell_data);
        }

        cells.push_row(cell_row);
    }

    // Create col labels
    for (auto col = 0; col < cols; col++) {
        auto col_label = Grid::label_data(col);
        col_labels.push(col_label);
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

        {
            // Make the window fill the entire screen
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

            ImGui::PushStyleColor(ImGuiCol_WindowBg,
                                  colours.get("dark_grey").imu32());

            ImGui::Begin(
                "Grid", nullptr,
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                    ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoNav);

            ImGui::PopStyleColor();

            // Draw col labels and then rows
            Grid::draw_col_labels();
            while (num_rows_drawn < rows) {
                Grid::draw_row();
            }

            ImGui::End();

            Grid::reset_for_next_draw();
        }
    }
}
