//
// Created by gaetz on 08/04/2023.
//

#ifndef GDRAW_IM_RENDERER_H
#define GDRAW_IM_RENDERER_H

#include "raylib.h"
#include "Defines.hpp"

namespace gmath {
    class Rect;
}

using gmath::Rect;

namespace gdraw::im {
    GAPI i32 GuiSlider(const Rect& rect, const str& textLeft, const str& textRight, f32 value, f32 minValue, f32 maxValue);
    GAPI i32 GuiSliderBar(const Rect& rect, const str& textLeft, const str& textRight, f32 value, f32 minValue, f32 maxValue);
    GAPI i32 GuiProgressBar(const Rect& rect, const str& textLeft, const str& textRight, f32 value, f32 minValue, f32 maxValue);
}

#endif //GDRAW_IM_RENDERER_H
