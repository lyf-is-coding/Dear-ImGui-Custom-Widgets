#include "AnimatedCircle.h"

void ImGui::RenderAnimatedCheckmark(ImVec2 center, ImU32 color, float size, float thickness, float aniDuration)
{
    // Setup
    ImDrawList* front = ImGui::GetForegroundDrawList();

    static double time_start = GImGui->Time;
    float position = (float)(GImGui->Time - time_start) / aniDuration;

    center.x = center.x - (float)size / 12;
    center.y = center.y + (float)size / 5 * 2;
    ImVec2 left(center.x - size / 2.2f, center.y - size / 3);
    ImVec2 right(center.x + size / 1.8f, center.y - size * 0.8f);

    static float xP1 = (float)left.x;
    static float xP2 = (float)center.x;

    // Phase 1: left -> center
    if (xP1 <= center.x)
    {
        xP1 += position;

        float x0 = left.x; float y0 = left.y;
        float x1 = center.x; float y1 = center.y;

        // Linear interpolation magic
        float y = y0 + ((y1 - y0) / (x1 - x0)) * (xP1 - x0);

        ImVec2 points[2]{ left, ImVec2(xP1, y) };
        front->AddPolyline(points, 2, color, 0, thickness);
    }
    else
    // Phase 2: center -> right
    if (xP2 <= right.x && xP1 >= center.x)
    {
        xP2 += position;
        
        float x0 = center.x; float y0 = center.y;
        float x1 = right.x; float y1 = right.y;

        //float y = y0 - ((y0 - y1) / (x0 - x1)) * (x0 - xP2);
        float y = y0 + ((y1 - y0) / (x1 - x0)) * (xP2 - x0);

        ImVec2 points[3]{ left, center, ImVec2(xP2, y) };
        front->AddPolyline(points, 3, color, 0, thickness);
    }
    else
    {
        ImVec2 points[3]{ left, center, right };
        front->AddPolyline(points, 3, color, 0, thickness);
    }
}

void ImGui::AnimatedCircle(float value, ImU32 backColor, ImU32 foreColor, ImU32 completeColor)
{
    // Setup
    ImDrawList* front = GetForegroundDrawList(); // also you can use GetWindowDrawList() or GetBackgroundDrawList()
    ImVec2 center = GetMainViewport()->GetCenter();

    float arc_size = (2.f * IM_PI) * value; // 0 <= x <= 2*pi
    
    const float radius = 35.f;
    const float thickness = 8.f;
    const float ani_duration = 0.25f;

    static double time_start = GImGui->Time;
    static float position = 0.0f;
    position = (float)(GImGui->Time - time_start) / ani_duration;

    // Background
    front->AddCircle(center, radius, backColor, 0, thickness);

    // Foreground
    front->PathClear();
    front->PathArcTo(center, radius, position, position + arc_size);
    front->PathStroke(arc_size >= 2.0f * IM_PI ? completeColor : foreColor, 0, thickness);

    // Render animated checkmark when circle is filled
    if (arc_size < 2.f * IM_PI)
    {
        std::string percent_value = std::to_string((int)(value * 100));
        if ((int)(value * 100) <= 9)
            percent_value = '0' + percent_value;
        
        ImVec2 textSize = CalcTextSize(percent_value.c_str());

        // Percent text
        front->AddText(GetFont(), radius, ImVec2(center.x - textSize.x/3 - radius/3, center.y - textSize.y/2 - radius/3), foreColor, percent_value.c_str());
    }
    else
        RenderAnimatedCheckmark(center, completeColor, radius, thickness, ani_duration);
}