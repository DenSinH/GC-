#pragma once

#include <stdint.h>
#include <inttypes.h>

#include <SDL.h>
#include "imgui.h"

#define MAX_REGISTER_NAME_LENGTH 16
#define MAX_REGISTER_TAB_LENGTH 32

typedef struct s_register_data {
    char* name;
    const void* value;
    bool islong;
} s_register_data;


typedef struct s_register_tab {
    std::list<s_register_data> registers;
    char* name;
} s_register_tab;

typedef struct RegisterViewer
{
    std::list<s_register_tab> RegisterTabs;
    int columns;

    RegisterViewer()
    {
        this->columns = 8;
    }
    ~RegisterViewer()
    {

    }

    void AddRegisterTab(const char* name) {
        s_register_tab tab = {
                .name = new char[MAX_REGISTER_TAB_LENGTH]
        };
        strcpy_s(tab.name, MAX_REGISTER_NAME_LENGTH, name);
        RegisterTabs.push_back(tab);
    }

    void AddRegister(const char* reg, const void* value, bool islong, int tab) {
        s_register_data data = {
                .name = new char[MAX_REGISTER_NAME_LENGTH],
                .value = value,
                .islong = islong
        };

        strcpy_s(data.name, MAX_REGISTER_NAME_LENGTH, reg);

        auto RegisterTab = RegisterTabs.begin();
        std::advance(RegisterTab, tab);
        RegisterTab->registers.push_back(data);
    }

    void Draw(bool* p_open)
    {
        ImGui::SetNextWindowSizeConstraints(ImVec2(-1, 0),    ImVec2(-1, FLT_MAX));
        ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_Once);

        if (!ImGui::Begin("Register Viewer", p_open))
        {
            ImGui::End();
            return;
        }

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Close Register Viewer"))
                *p_open = false;
            ImGui::EndPopup();
        }

        char label[64];
        ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None);
        for (auto RegisterTab : RegisterTabs) {
            if (ImGui::BeginTabItem(RegisterTab.name)) {

                ImGui::Columns(columns, "Registers", true);  // 3-ways, no border
                int currentwidth = columns;

                for (auto reg : RegisterTab.registers) {
                    if (!reg.value) {
                        ImGui::Columns(1);
                        ImGui::Columns(currentwidth);
                        ImGui::Separator();
                        continue;
                    }

                    if (currentwidth == columns && reg.islong) {
                        currentwidth >>= 1;
                        ImGui::Columns(currentwidth);
                    }
                    else if (currentwidth < columns && !reg.islong) {
                        currentwidth = columns;
                        ImGui::Columns(currentwidth);
                    }

                    // name
                    sprintf(label, "%s", reg.name);
                    if (ImGui::Selectable(label)) {}
                    ImGui::NextColumn();

                    // value
                    if (!reg.islong) {
                        sprintf(label, "%08x", *((uint32_t*)reg.value));

                        if (ImGui::Selectable(label)) {
                            SDL_SetClipboardText(label);
                        }
                        ImGui::NextColumn();
                    }
                    else {
                        sprintf(label, "%016" PRIx64, *((uint64_t*)reg.value));

                        if (ImGui::Selectable(label)) {
                            SDL_SetClipboardText(label);
                        }
                        ImGui::NextColumn();
                    }
                }

                ImGui::Columns(1);
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();

        ImGui::End();
    }
} RegisterViewer;