#pragma once
#include "../imgui.h"
#include "../imgui_internal.h"
#include <string>

#ifndef _ANIMATEDCIRCLE_H_

namespace ImGui
{
	IMGUI_API void RenderAnimatedCheckmark(ImVec2 center, ImU32 color, float size, float thickness, float aniDuration);
	// Value: 0.0f -> 1.0f (0% -> 100%)
	IMGUI_API void AnimatedCircle(float value, ImU32 backColor = IM_COL32(200, 20, 20, 40), ImU32 foreColor = IM_COL32(200, 20, 20, 255), ImU32 completeColor = IM_COL32(70, 230, 66, 255));
}

#endif