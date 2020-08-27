#include "imgui.h"

#include "menubar.h"

void ShowMenuBar(bool* console_enabled, bool* register_viewer_enabled)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Debug"))
        {
            if (ImGui::MenuItem("Console", NULL, console_enabled)) {}
            if (ImGui::MenuItem("Register Viewer", NULL, register_viewer_enabled)) {}
//            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
