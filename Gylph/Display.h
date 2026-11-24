#pragma once
#include "css.h"
#include "Layout.h"

struct SolidColor {
    Color color;
    Rect rect;
};

struct DisplayCommand {
    std::variant<SolidColor> color;

};

using DisplayList = std::vector<DisplayCommand>;


void RenderBackGround(DisplayList& list, LayoutBox& layout) {
    if (auto color = layout.GetColor("background")) {
        list.emplace_back(SolidColor{*color, layout.dimension.BorderBox()});
    }

}
void RenderBorder(DisplayList& list, LayoutBox& layout) {
    auto color = layout.GetColor("border-color");
    if (!color) { return; };

    auto& dimension = layout.dimension;
    auto border_box = dimension.BorderBox();

    list.emplace_back(SolidColor{ *color, Rect{
       .x = border_box.x,
       .y = border_box.y,
       .width = dimension.border.left,
       .height = border_box.height,
        } });

    list.emplace_back(SolidColor{ *color, Rect{
        .x =  border_box.x + border_box.width - dimension.border.right,
        .y =  border_box.y,
        .width =  dimension.border.right,
        .height = border_box.height,
        } });

    // Top border
    list.emplace_back(SolidColor{*color, Rect{
        .x =  border_box.x,
        .y =  border_box.y,
        .width =  border_box.width,
        .height = dimension.border.top,
        }});

    // Bottom border
    list.emplace_back(SolidColor{ *color, Rect{
        .x =  border_box.x,
        .y =  border_box.y + border_box.height - dimension.border.bottom,
        .width =  border_box.width,
        .height = dimension.border.bottom,
        } });
}
void RenderLayoutBox(DisplayList& list, LayoutBox& layout) {
    RenderBackGround(list, layout);
    RenderBorder(list, layout);

    for (auto& child : layout.children) {
        RenderLayoutBox(list, child);
    }
}

DisplayList BuildDisplayList(LayoutBox& root) {
    DisplayList list;
    RenderLayoutBox(list, root);
    return list;
}