#include "headers/grid.hpp"

#include "headers/cell.hpp"
#include "headers/window.hpp"

Grid::Grid(int rows, int cols, Window &window) : window(window) {
    this->rows = rows;
    this->cols = cols;

    for (auto row = 0; row < rows; row++) {
        for (auto col = 0; col < cols; col++) {
            cells[row][col] = Cell(row, col);
        }
    }
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

            ImGui::Begin(
                "Cells", nullptr,
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                    ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoNav);

            const double CELL_WIDTH = 50.0;
            for (auto row = 0; row < rows; row++) {
                for (auto col = 0; col < cols; col++) {
                    std::string label =
                        "##" + std::to_string(row) + "-" + std::to_string(col);
                    ImGui::SetNextItemWidth(CELL_WIDTH);

                    ImGui::InputText(label.c_str(),
                                     cells[row][col].raw_value.data(),
                                     sizeof(cells[row][col].raw_value.data()));

                    if (col < cols - 1)
                        ImGui::SameLine();
                }
            }
            ImGui::End();
        }
    }
}
