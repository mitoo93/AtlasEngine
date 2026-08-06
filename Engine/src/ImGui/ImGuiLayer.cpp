#include "AtlasEngine/ImGui/ImGuiLayer.h"

#include "AtlasEngine/Core/Application.h"
#include "AtlasEngine/Core/Window.h"

#include <imgui.h>
#include <iostream>
#include <filesystem>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace AtlasEngine
{

ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer")
{
}

ImGuiLayer::~ImGuiLayer()
{
}

void ImGuiLayer::OnAttach()
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    std::cout
        << "Ini filename: "
        << (io.IniFilename ? io.IniFilename : "null")
        << '\n';

    std::cout
        << "Working directory: "
        << std::filesystem::current_path()
        << '\n';


    ImGui::StyleColorsDark();

    Window& window =
        Application::Get().GetWindow();

    GLFWwindow* nativeWindow =
        window.GetNativeWindow();

    ImGui_ImplGlfw_InitForOpenGL(
        nativeWindow,
        true);

    ImGui_ImplOpenGL3_Init("#version 330");

    
}

void ImGuiLayer::OnDetach()
{
    ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();
}

void ImGuiLayer::Begin()
{
    ImGui_ImplOpenGL3_NewFrame();

    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    /* static bool demo = true;
    ImGui::ShowDemoWindow(&demo); */
}

void ImGuiLayer::End()
{
    ImGuiIO& io = ImGui::GetIO();

    Window& window =
        Application::Get().GetWindow();

    io.DisplaySize = ImVec2(
        static_cast<float>(window.GetWidth()),
        static_cast<float>(window.GetHeight()));

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(
        ImGui::GetDrawData());
}

}