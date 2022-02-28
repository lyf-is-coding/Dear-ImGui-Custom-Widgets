#pragma once
#include "../imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui_internal.h"

#ifndef _CHECKBOX1_H_

namespace ImGui
{
	IMGUI_API void RenderBoxBorder(ImVec2 p_min, ImVec2 p_max, ImU32 border, ImU32 border_shadow, float rounding);
	IMGUI_API void RenderFilledBox(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, float rounding);
	IMGUI_API bool AnimatedCheckbox(const char* label, bool* v);
}

#endif