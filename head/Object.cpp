//
// Created by smers on 02.04.2022.
//
#include "Object.h"

float distance(sf::Sprite &sprite1, sf::Vector2<float> pos)
{
    float x1 = sprite1.getPosition().x;
    float y1 = sprite1.getPosition().y;
    float x2 = pos.x;
    float y2 = pos.y;
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void Object::draw_objects(sf::RenderWindow &window, sf::Vector2<float> pos) {
    {
        for (int i = 0; i < Objects.size(); i++)
        {
            if (distance(Objects[i], pos) < (sqrt(pow(window.getSize().x, 2) + pow(window.getSize().y, 2)) / 2)+100)
                window.draw(Objects[i]);

        }
    }

}

void Object::create_object(std::string texture_path, sf::Vector2f pos, int height, int width) {
    if(!aba.loadFromFile(texture_path))
    {
        printf("Error load texture");
        return;
    }
    aba.setSmooth(true);
    aba.setRepeated(false);
    object.setTexture(aba);
    if (height != 0 && width != 0)
        object.setScale({1,1});
    object.setPosition(pos.x,pos.y);

    Objects.push_back(object);
}