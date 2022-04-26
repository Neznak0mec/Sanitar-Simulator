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

void UI::Button::set_tex(string tex_path) {
    tex.loadFromFile(tex_path);
    shape.setTexture(&tex);
}


UI::Top_Bar::Top_Bar(Vector2<float> size, Vector2<float> pos, string tex_path) {
    shape.setSize(size);
    bar_tex.loadFromFile(tex_path);
    shape.setTexture(&bar_tex);
    shape.setPosition(pos);
}

void UI::Top_Bar::draw(RenderWindow &window,int sc ,int sl, int hl, int ang) {
    float x = window.getView().getCenter().x;
    float y = window.getView().getCenter().y;
    shape.setPosition(x-275, y-200 );
    // draw text
    Font font;
    font.loadFromFile("fonts/Agit_Prop.ttf");
    Text text;
    text.setFont(font);
    text.setString(to_string(sc));
    text.setCharacterSize(25);
    text.setPosition(x-120, y-196);
    text.setFillColor(Color::Black);


    window.draw(shape);
    window.draw(text);

    text.setString(to_string(sl));
    text.setPosition(x+25, y-196);
    window.draw(text);

    text.setString(to_string(hl));
    text.setPosition(x+112, y-196);
    window.draw(text);

    text.setString(to_string(ang));
    text.setPosition(x+199, y-196);
    window.draw(text);
}


void UI::Lamps::draw(RenderWindow &window,Sick &sick,int lvl) {
    //get screen size
    float w = window.getView().getCenter().x;
    float y = window.getView().getCenter().y;
    y = y + 170;
    double center = w;

    vector<Sick::sick_info> objects;
    for (int i = 0; i < sick.all_seeck.size(); ++i)
        if (sick.all_seeck[i].open_lvl == lvl)
            objects.push_back(sick.all_seeck[i]);

    Text text;
    Font font;
    font.loadFromFile("fonts/Agit_Prop.ttf");
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(Color::Black);
    CircleShape circle(40);

    double startPosition = center - (objects.size() * 40 + (objects.size() - 1) * 5);


    for (int i = 0; i < objects.size(); ++i) {
        if (objects[i].status == status_for_seeck::need_help)
        {
            if ( 20 < objects[i].time_have < 30)
                circle.setFillColor(Color::Green);
            if (10 < objects[i].time_have <= 20)
                circle.setFillColor(Color::Yellow);
            if (objects[i].time_have < 10)
                circle.setFillColor(Color::Red);
        }
        else
            circle.setFillColor(Color::White);

        text.setString(to_string(i+1));

        circle.setPosition(startPosition + i * 90, y );

        text.setPosition(startPosition + i * 90+35, y );

        window.draw(circle);
        window.draw(text);
    }

};
