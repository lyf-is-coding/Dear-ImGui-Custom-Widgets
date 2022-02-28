#include "Checkbox1.h"

void ImGui::RenderBoxBorder(ImVec2 p_min, ImVec2 p_max, ImU32 border, ImU32 border_shadow, float rounding)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    const float border_size = 1.0f;
    window->DrawList->AddRect(p_min + ImVec2(1, 1), p_max + ImVec2(1, 1), border_shadow, rounding, 0, border_size);
    window->DrawList->AddRect(p_min, p_max, border, rounding, 0, border_size);
}

void ImGui::RenderFilledBox(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, float rounding)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding);
}

bool ImGui::AnimatedCheckbox(const char* label, bool* v)
{
    // Prepare
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    const ImGuiID id = window->GetID(label);
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const float square_sz = GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;

    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    const ImRect square_bb(pos, pos + ImVec2(square_sz, square_sz));

    //Register
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id)) return false;

    // Interaction
    bool hovered, held;
    bool pressed = ButtonBehavior(square_bb, id, &hovered, &held, ImGuiButtonFlags_PressedOnClick);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    // Calculate animation
    const float ani_duration = 0.2f;
    float ani_progress = 1.0f;
    if (g.LastActiveId == id) ani_progress = ImClamp(g.LastActiveIdTimer / ani_duration, 0.0f, 1.0f);

    ImVec4 box_vec4_col = GetStyleColorVec4(ImGuiCol_FrameBg);
    ImVec4 check_vec4_col = GetStyleColorVec4(ImGuiCol_CheckMark);
    ImVec4 border_col = GetStyleColorVec4(ImGuiCol_Border);
    ImVec4 border_shadow_col = GetStyleColorVec4(ImGuiCol_BorderShadow);
    const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));

    // Render fade-in animation: filled box, check mark
    // Render fade-out animation: box border
    if (*v)
    {
        box_vec4_col.w *= ani_progress;
        RenderFilledBox(square_bb.Min, square_bb.Max, ColorConvertFloat4ToU32(box_vec4_col), style.FrameRounding);

        check_vec4_col.w *= ani_progress;
        RenderCheckMark(window->DrawList, square_bb.Min + ImVec2(pad, pad), ColorConvertFloat4ToU32(check_vec4_col), square_sz - pad * 2.0f);

        ani_progress = 1 - ani_progress;

        border_col.w *= ani_progress;
        border_shadow_col.w *= ani_progress;
        RenderBoxBorder(square_bb.Min, square_bb.Max, ColorConvertFloat4ToU32(border_col), ColorConvertFloat4ToU32(border_shadow_col), style.FrameRounding);
    }
    // Render fade-in animation: box border
    // Render fade-out animation: filled box, check mark
    else
    {
        border_col.w *= ani_progress;
        border_shadow_col.w *= ani_progress;
        RenderBoxBorder(square_bb.Min, square_bb.Max, ColorConvertFloat4ToU32(border_col), ColorConvertFloat4ToU32(border_shadow_col), style.FrameRounding);

        ani_progress = 1 - ani_progress;

        box_vec4_col.w *= ani_progress;
        RenderFilledBox(square_bb.Min, square_bb.Max, ColorConvertFloat4ToU32(box_vec4_col), style.FrameRounding);

        check_vec4_col.w *= ani_progress;
        RenderCheckMark(window->DrawList, square_bb.Min + ImVec2(pad, pad), ColorConvertFloat4ToU32(check_vec4_col), square_sz - pad * 2.0f);
    }

    // Render label
    if (label_size.x > 0.0f)
    {
        ImVec2 label_pos = ImVec2(square_bb.Max.x + style.ItemInnerSpacing.x, square_bb.Min.y + style.FramePadding.y);
        RenderText(label_pos, label);
    }

    return true;
}