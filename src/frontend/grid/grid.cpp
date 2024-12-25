#include "grid.hpp"

#include "imgui_impl_glfw.h"
using namespace grid_space;

grid::grid(int rows, int cols, window &window) : m_window(window) {
    // Set up grid
    m_rows = rows;
    m_cols = cols;

    for (auto col = 0; col < cols; col++) {
        m_col_labels.push(label_data(col));
    }

    // Create grid of cells
    for (auto row = 0; row < rows; row++) {
        // Add label for this row
        m_row_labels.push(label_data(row));

        // Add cells for this row
        v_cell_data cell_row;
        for (auto col = 0; col < cols; col++) {
            cell_row.push_back(cell_data({row, col}));
        }
        m_cells.push_row(cell_row);
    }

    // Get style and io
    ImGuiStyle &style = ImGui::GetStyle();
    ImGuiIO &io = ImGui::GetIO();

    // Set style
    style.ItemSpacing = ImVec2(1.0f, 1.0f);

    // Load font and set as default
    m_font = io.Fonts->AddFontFromFileTTF(
        "src/fonts/JetBrainsMono-Regular.ttf",
        DEFAULT_CELL_HEIGHT - ImGui::GetStyle().FramePadding.y * 2.0f -
            ImGui::GetStyle().FrameBorderSize * 2.0f);
    io.FontDefault = m_font;
};

// Eventually make this render only those rows and columns
// that are actually visible
void grid::draw() {
    if (!m_window.should_close()) {
        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            // Make the window fill the entire screen
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

            // Set the background colour
            ImGui::PushStyleColor(ImGuiCol_WindowBg,
                                  m_colours.get("dark_grey").imgui());

            // Draw the grid
            ImGui::Begin(
                "Grid", nullptr,
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoBringToFrontOnFocus |
                    ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoNav);

            ImGui::PopStyleColor();

            // Draw col labels and then rows
            draw_col_labels();
            while (m_num_rows_drawn < m_rows) {
                draw_row();
            }

            ImGui::End();

            reset_for_next_draw();
        }
    }
}
