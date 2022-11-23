//
// Created by abdoulayedia on 22.11.2022
//
#pragma once

#include "../Utilities/Singleton.h"
#include "../Core/classes.h"

namespace se
{
    // Grid
    class Grid
    {
    public:
        template <typename T>
        static void RenderGrid(Window *window, std::vector<T> &collection, int cell_size = AUTO_GRID_SIZE, int max_elements_per_row = AUTO_GRID_SIZE, bool wrap = true, const std::string &ID = "Grid", const std::string &label = "")
        {

            // Display all glyphs of the fonts in separate pages of 256 characters
            if (ImGui::TreeNode("Glyphs", "Glyphs (%d)", cell_size))
            {
                ImDrawList *draw_list = ImGui::GetWindowDrawList();
                const float cell_spacing = ImGui::GetStyle().ItemSpacing.y;

                // Draw a 16x16 grid of glyphs
                ImVec2 base_pos = ImGui::GetCursorScreenPos();
                size_t size = collection.size();
                for (unsigned int n = 0; n < size; n++)
                {
                    ImVec2 cell_p1(base_pos.x + (n % max_elements_per_row) * (cell_size + cell_spacing), base_pos.y + (n / max_elements_per_row) * (cell_size + cell_spacing));
                    ImVec2 cell_p2(cell_p1.x + cell_size, cell_p1.y + cell_size);
                    draw_list->AddRect(cell_p1, cell_p2, true ? IM_COL32(255, 255, 255, 100) : IM_COL32(255, 255, 255, 50));

                    if (ImGui::IsMouseHoveringRect(cell_p1, cell_p2))
                    {
                        ImGui::BeginTooltip();
                        // ImGui text of the coordinates of the cell
                        ImGui::Text("Coords: (%d, %d)", (int)(cell_p1.x - base_pos.x) / (int)(cell_size + cell_spacing), (int)(cell_p1.y - base_pos.y) / (int)(cell_size + cell_spacing));
                        ImGui::EndTooltip();

                        if (ImGui::IsMouseClicked(0))
                        {
                            // invert the boolean value of the cell when clicked
                            collection[n] = !collection[n];
                        }
                    }

                    if (collection[n])
                    {
                        // draw the glyph in the cell
                        draw_list->AddRectFilled(cell_p1, cell_p2, IM_COL32(255, 255, 255, 50));
                    }
                    else
                    {
                        draw_list->AddRectFilled(cell_p1, cell_p2, IM_COL32(255, 255, 255, 255));
                    }
                }
                ImGui::Dummy(ImVec2((cell_size + cell_spacing) * max_elements_per_row, (cell_size + cell_spacing) * max_elements_per_row));

                ImGui::TreePop();
            }
        }

    private:
    };

}