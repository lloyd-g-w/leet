#include "grid.hpp"
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"

using namespace gui;

grid::grid(std_cells::grid &cell_grid, window &window)
    : m_window(window), m_cell_grid(cell_grid),
      m_col_labels(m_cell_grid.get_cols(),
                   {get_default_dims().width, get_default_dims().height}),
      m_row_labels(m_cell_grid.get_rows(),
                   {get_default_dims().width, get_default_dims().height}) {
    // Setup grid dimensions
    dim_t default_dims = get_default_dims();
    float default_spacing = get_default_spacing();
    m_col_labels = label_group_t(m_cell_grid.get_cols(), default_dims);
    m_row_labels = label_group_t(m_cell_grid.get_rows(), default_dims);
    m_rows = m_cell_grid.get_rows();
    m_cols = m_cell_grid.get_cols();
    m_grid_height = (m_rows + 1) * default_dims.height;
    m_grid_width = (m_cols + 1) * default_dims.width;

    // Set ImGui style
    ImGuiStyle &style = ImGui::GetStyle();
    ImGuiIO &io = ImGui::GetIO();

    style.ItemSpacing = ImVec2(default_spacing, default_spacing);

    float font_size =
        default_dims.height - ImGui::GetStyle().FramePadding.y * 2.0f;

    // Load font and set as default - at 10x scale
    m_font = io.Fonts->AddFontFromFileTTF(
        "src/frontend/fonts/JetBrainsMono-Regular.ttf", font_size * 10);
    m_font->FontSize = font_size;
    io.FontDefault = m_font;
    io.FontGlobalScale = 1.0 / 10;

    // Add some default colours
    this->m_colours.add_preset(colours::palette::HOLY_SHEET);
}

void grid::draw() {
    if (!m_window.should_close()) {
        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            m_font->Scale = m_scale_percent / 100.0;
            ImGui::PushFont(m_font);
            // Make the window fill the entire screen
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

            // Set the background colour
            colour bg_colour = m_colours.get("dark_grey");
            ImGui::PushStyleColor(ImGuiCol_WindowBg, bg_colour.imgui());

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

            rect_coords visible_coords = calc_visible_coords();

            float default_spacing = get_default_spacing();
            auto style = ImGui::GetStyle();
            style.ItemSpacing = ImVec2(default_spacing, default_spacing);

            handle_scrolling();

            // Draw col labels and then rows
            draw_col_labels(visible_coords[0].col, visible_coords[1].col);
            for (int row = visible_coords[0].row; row <= visible_coords[1].row;
                 row++) {
                draw_row(row, visible_coords[0].col, visible_coords[1].col);
            }

            // Draw the windows draw custom draw queue
            draw_full_queue();

            ImGui::End();
            ImGui::PopFont();
        }

        {
            // START TEMP SCALE
            m_font->Scale = 1;
            ImGui::PushFont(m_font);

            ImGui::Begin("Info", nullptr);

            ImGui::SetNextItemWidth(DEFAULT_CELL_WIDTH);
            ImGui::SliderInt("##zoom%", &m_scale_percent, 50, 250, "%d %");

            // Fps and frame time
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::Text("Frame time: %.3f ms/frame",
                        1000.0f / ImGui::GetIO().Framerate);

            ImGui::End();
            ImGui::PopFont();
            // END TEMP SCALE
        }
    }
}
