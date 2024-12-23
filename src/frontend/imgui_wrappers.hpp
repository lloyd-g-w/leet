#pragma once

#include <string>

#include "imgui.h"

namespace ImGui {
int InputDynamicText(const char *label, std::string *buf,
                     ImGuiInputTextFlags flags = 0,
                     ImGuiInputTextCallback callback = NULL,
                     void *user_data = NULL);
}
