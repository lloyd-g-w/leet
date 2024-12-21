#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

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
};
