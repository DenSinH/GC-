#pragma once

#include <stdint.h>
#include <inttypes.h>

#include "register_viewer.h"

#include "imgui.h"

#define MAX_REGISTER_NAME_LENGTH 16

typedef struct s_register_data {
    char* name;
    const void* value;
    bool islong;
} s_register_data;

struct RegisterViewer
{
    std::list<s_register_data> Registers;
    int columns;

    RegisterViewer()
    {
        this->columns = 8;
    }
    ~RegisterViewer()
    {

    }

    void AddRegister(char* reg, const void* value, bool islong) {
        s_register_data data = {
                .name = new char[MAX_REGISTER_NAME_LENGTH],
                .value = value,
                .islong = islong
        };

        strcpy_s(data.name, MAX_REGISTER_NAME_LENGTH, reg);

        Registers.push_back(data);
    }

    void Draw(bool* p_open)
    {
        ImGui::SetNextWindowSizeConstraints(ImVec2(-1, 0),    ImVec2(-1, FLT_MAX));
        ImGui::SetNextWindowSize(ImVec2(columns * 15, 600), ImGuiCond_FirstUseEver);

        if (!ImGui::Begin("Register viewer", p_open))
        {
            ImGui::End();
            return;
        }

        // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
        // So e.g. IsItemHovered() will return true when hovering the title bar.
        // Here we create a context menu only available from the title bar.
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Close Register Viewer"))
                *p_open = false;
            ImGui::EndPopup();
        }

        // display items starting from the bottom
        ImGui::Columns(columns, "registers", true);  // 3-ways, no border
        int currentwidth = columns;

        char label[64];
        std::list<s_register_data> :: iterator register_iter;
        for (register_iter = this->Registers.begin(); register_iter != this->Registers.end(); ++register_iter) {
            if (!register_iter->value) {
                ImGui::Separator();
                continue;
            }

            if (currentwidth == columns && register_iter->islong) {
                currentwidth >>= 1;
                ImGui::Columns(currentwidth);
            }
            else if (currentwidth < columns && !register_iter->islong) {
                currentwidth = columns;
                ImGui::Columns(currentwidth);
            }

            // name
            sprintf(label, "%s", register_iter->name);
            if (ImGui::Selectable(label)) {}
            ImGui::NextColumn();

            // value
            if (!register_iter->islong) {
                sprintf(label, "%08x", *((uint32_t*)register_iter->value));

                if (ImGui::Selectable(label)) {}
                ImGui::NextColumn();
            }
            else {
                sprintf(label, "%016" PRIx64, *((uint64_t*)register_iter->value));

                if (ImGui::Selectable(label)) {}
                ImGui::NextColumn();
            }

        }

        ImGui::Columns(1);

        ImGui::End();
    }
};