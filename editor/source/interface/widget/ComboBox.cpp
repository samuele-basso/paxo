#include "ComboBox.h"

namespace editor
{
    namespace gui
    {

        combo_box::combo_box(const std::string& label, const char** selec, size_t count)
            : p_selec(selec), p_selected(0), p_count(count), p_label(label)
        {
        }

        bool combo_box::update()
        {
            bool ris = false;

            ImGui::PushItemWidth(100.0f);

            if (ImGui::BeginCombo(p_label.c_str(), p_selec[p_selected]))
            {
                for (int n = 0; n < p_count; n++)
                {
                    bool selected = p_selected == n;

                    if (ImGui::Selectable(p_selec[n], selected))
                    {
                        ris = true;
                        p_selected = n;
                    }

                    if (selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::PopItemWidth();

            return ris;
        }

    }
}