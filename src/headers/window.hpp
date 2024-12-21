#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <GLFW/glfw3.h>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN

class Window {
  public:
    Window();
    ~Window();

  public:
    void run();
    void render();
    bool should_close();

  private:
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    GLFWwindow *window;
    ImGui_ImplVulkan_InitInfo init_info = {};
    ImGuiIO *io;

    ImVector<const char *> extensions;
    uint32_t extensions_count = 0;
    const char **glfw_extensions;


    ImGui_ImplVulkanH_Window *wd;
    VkSurfaceKHR surface;
    int w, h;
};
