#pragma once

#include <string>

#include "imgui.h"

namespace ImGui {
int InputDynamicText(const char *label, std::string *buf,
                     ImGuiInputTextFlags flags = 0,
                     ImGuiInputTextCallback callback = NULL,
                     void *user_data = NULL);
int InputDynamicTextMultiline(const char *label, std::string *buf,
                              const ImVec2 &size = ImVec2(0, 0),
                              ImGuiInputTextFlags flags = 0,
                              ImGuiInputTextCallback callback = NULL,
                              void *user_data = NULL);
int InputSizedDynamicText(const char *label, std::string *buf,
                          const ImVec2 &size = ImVec2(0, 0),
                          ImGuiInputTextFlags flags = 0,
                          ImGuiInputTextCallback callback = NULL,
                          void *user_data = NULL);
}  // namespace ImGui
