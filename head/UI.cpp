//
// Created by smers on 05.04.2022.
//

#include "UI.h"

UI::Button::Button( Vector2<float> size, string tex_path) {
    shape.setSize(size);
    tex.loadFromFile(tex_path);
    shape.setTexture(&tex);
}


void UI::Button::draw(RenderWindow &window,float pos1, float pos2) {
    shape.setPosition(pos1, pos2);
    window.draw(shape);
}

bool UI::Button::is_clicked(float pos1, float pos2) {
    if (shape.getLocalBounds().contains(pos1, pos2)) {
        if (Mouse::isButtonPressed(Mouse::Left)) {
            return true;
        }
    }
    return false;
}


