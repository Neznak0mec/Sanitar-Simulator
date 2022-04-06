//
// Created by smers on 05.04.2022.
//

#ifndef DURKACPP_UI_H
#define DURKACPP_UI_H
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class UI {
public:
    class Button{
        public:
        sf::RectangleShape shape;

        Button(Vector2<float> size, string tex_path);
        void draw(RenderWindow &window,float pos1, float pos2);

    private:
        Vector2<int> pos;
        Vector2<int> size;
        Texture tex;
    };
};

#endif //DURKACPP_UI_H
