//
// Created by smers on 02.04.2022.
//
#include "Object.h"

void Object::draw_objects(sf::RenderWindow &window) {
    {
        for (int i = 0; i < Objects.size(); i++)
        {
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
