//
// Created by smers on 05.04.2022.
//

#include "UI.h"

UI::rec_Button::rec_Button(Vector2<float> size, Color color, Vector2<float> pos, string text) {
    rect.setSize(size);
    rect.setFillColor(color);
    rect.setPosition(pos.x, pos.y);
    text_button = text;
}

void UI::rec_Button::draw(RenderWindow &window) {
    window.draw(rect);
    sf::Text text;
    text.setString(text_button);
    Font font;
    font.loadFromFile("fonts/Agit_Prop.ttf");
    text.setFont(font);
    text.setPosition(rect.getPosition().x+20, rect.getPosition().y+18);
    window.draw(text);
}

bool UI::rec_Button::is_clicked(float pos1, float pos2) {
    if (rect.getGlobalBounds().contains(pos1, pos2))
    {
        printf("на");
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            return true;
        }
    }
    return false;
}


UI::Button::Button(Vector2<float> size, string tex_path) {
    shape.setSize(size);
    tex.loadFromFile(tex_path);
    shape.setTexture(&tex);
    rect = UI::rec_Button(size,Color::Black,shape.getPosition(),"");
}

void UI::Button::draw(RenderWindow &window,float pos1, float pos2) {
    shape.setPosition(pos1, pos2);
    window.draw(shape);
}


void UI::Button::set_tex(string tex_path) {
    tex.loadFromFile(tex_path);
    shape.setTexture(&tex);
}

bool UI::Button::is_clicked(Vector2<int> pos)
{
    return rect.is_clicked(pos.x,pos.y);
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

bool UI::menu(RenderWindow &window,Vector2<int> mouse_pos){
    Sprite background;
    Texture texture;
    texture.loadFromFile("sprites/menu_back_ground.png");
    texture.setSmooth(true);
    background.setTexture(texture);
    background.setScale({1.8,1.5});
    background.setPosition({0,0});

    window.clear();

    window.draw(background);

    auto start = rec_Button(Vector2<float>(150,75),Color::Black,Vector2<float>(100,400),"Start");
    start.draw(window);

    auto FAQ = rec_Button(Vector2<float>(150,75),Color::Black,Vector2<float>(100,500),"FAQ");
    FAQ.draw(window);

    if (start.is_clicked(mouse_pos.x,mouse_pos.y))
    {
        return false;
    }

    window.display();

    return true;
}

