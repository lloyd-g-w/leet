#include "imgui_wrappers.hpp"

#include "imgui.h"
#include "imgui_internal.h"

namespace ImGui {

struct InputTextUserData {
    std::string *str;                 // Buffer for dynamic resizing
    ImGuiInputTextCallback callback;  // User callback
    void *user_data;                  // User's original user_data
};

static int ResizeCallback(ImGuiInputTextCallbackData *data) {
    auto *userData = static_cast<InputTextUserData *>(data->UserData);

    IM_ASSERT(data->Buf == userData->str->c_str());
    userData->str->resize(data->BufSize);
    data->Buf = userData->str->data();
    return 0;
}

static int ExitOnReturnCallback(ImGuiInputTextCallbackData *data) {
    if (data->EventChar == '\n' || data->EventChar == '\r') {
        data->EventChar = '\0';
        ImGui::ClearActiveID();
        return 1;
    }
    return 0;
}

static int CallbackHandler(ImGuiInputTextCallbackData *data) {
    auto *userData = static_cast<InputTextUserData *>(data->UserData);

    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        ResizeCallback(data);
    }

    if (data->EventFlag == ImGuiInputTextFlags_CallbackCharFilter) {
        ExitOnReturnCallback(data);
    }

    if (userData->callback) {
        data->UserData = userData->user_data;
        return userData->callback(data);
    }

    return 0;
}

int InputDynamicText(const char *label, std::string *buf,
                     ImGuiInputTextFlags flags, ImGuiInputTextCallback callback,
                     void *user_data) {
    auto userCallbackData = std::make_pair(buf, callback);

    return ImGui::InputText(label, buf->data(), buf->size() + 1,
                            ImGuiInputTextFlags_CallbackResize | flags,
                            CallbackHandler, &userCallbackData);
}

int InputDynamicTextMultiline(const char *label, std::string *buf,
                              const ImVec2 &size, ImGuiInputTextFlags flags,
                              ImGuiInputTextCallback callback,
                              void *user_data) {
    auto userCallbackData = std::make_pair(buf, callback);

    return ImGui::InputTextMultiline(label, buf->data(), buf->size() + 1, size,
                                     ImGuiInputTextFlags_CallbackResize | flags,
                                     CallbackHandler, &userCallbackData);
}

int InputSizedDynamicText(const char *label, std::string *buf,
                          const ImVec2 &size, ImGuiInputTextFlags flags,
                          ImGuiInputTextCallback callback, void *user_data) {
    auto userCallbackData = std::make_pair(buf, callback);

    return ImGui::InputDynamicTextMultiline(
        label, buf, size, ImGuiInputTextFlags_CallbackCharFilter | flags, NULL,
        &userCallbackData);
}

}  // namespace ImGui
