#include "headers/grid.hpp"

class Cells {
  public:
    char *raw_val(size_t row, size_t col) {
        return cell_buffers.at(row).at(col).raw_val;
    }

    char *calculated_val(size_t row, size_t col) {
        return cell_buffers.at(row).at(col).calculated_val;
    }

  private:
    static constexpr int MAX_INPUT_SIZE = 512;
    struct Cell {
        char raw_val[MAX_INPUT_SIZE] = "";
        char calculated_val[MAX_INPUT_SIZE] = "";
    };

    std::vector<std::vector<Cell>> cell_buffers;
};

class Grid {
  public:
    int MAX_ROWS;
    int MAX_COLS;

    Grid(const int max_rows, const int max_cols,
         std::unique_ptr<Window> window) {
        MAX_ROWS = max_rows;
        MAX_COLS = max_cols;

        window = std::move(window);

        cells = Cells();
    }

    void run() {
        if (!window->should_close()) {
            window->run();

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
                for (auto row = 0; row < MAX_ROWS; row++) {
                    for (auto col = 0; col < MAX_COLS; col++) {
                        std::string label = "##" + std::to_string(row) + "-" +
                                            std::to_string(col);
                        ImGui::SetNextItemWidth(CELL_WIDTH);

                        ImGui::InputText(label.c_str(), cells.raw_val(row, col),
                                         sizeof(cells.raw_val(row, col)));

                        if (col < MAX_COLS - 1)
                            ImGui::SameLine();
                    }
                }
                ImGui::End();
            }

            window->render();
        }
    }

  private:
    std::unique_ptr<Window> window;
    Cells cells;
};
