#include "grid_gui.hpp"
#include <chrono>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"

grid_gui::grid_gui(cells_std::cell_grid &cell_grid, window &window)
    : m_window(window), m_cell_grid(cell_grid),
      m_gui_col_labels(m_cell_grid.cols()),
      m_gui_row_labels(m_cell_grid.rows()) {
    m_rows = m_cell_grid.rows();
    m_cols = m_cell_grid.cols();

    m_grid_height = (m_rows + 1) * DEFAULT_CELL_HEIGHT;
    m_grid_width = (m_cols + 1) * DEFAULT_CELL_WIDTH;

    // Set ImGui style
    ImGuiStyle &style = ImGui::GetStyle();
    ImGuiIO &io = ImGui::GetIO();

    style.ItemSpacing = ImVec2(1.0f, 1.0f);

    // Load font and set as default
    m_font = io.Fonts->AddFontFromFileTTF(
        "src/fonts/JetBrainsMono-Regular.ttf",
        DEFAULT_CELL_HEIGHT - ImGui::GetStyle().FramePadding.y * 2.0f -
            ImGui::GetStyle().FrameBorderSize * 2.0f);
    io.FontDefault = m_font;
}

// Eventually make this render only those rows and columns
// that are actually visible
void grid_gui::draw() {
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
                    ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoNav |
                    ImGuiWindowFlags_NoScrollbar |
                    ImGuiWindowFlags_NoScrollWithMouse);

            ImGui::PopStyleColor();

            std::array<cells_std::pos, 2> visible_coords =
                calc_visible_coords();

            handle_scrolling();

            // Draw col labels and then rows
            draw_col_labels(visible_coords[0].col, visible_coords[1].col);
            for (int row = visible_coords[0].row; row <= visible_coords[1].row;
                 row++) {
                draw_row(row, visible_coords[0].col, visible_coords[1].col);
            }
            ImGui::End();
        }
    }
}
