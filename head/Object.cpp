//
// Created by smers on 02.04.2022.
//
#include "Object.h"

using namespace sf;

Vector2<float> centerS(Sprite sprite)
{
    return {sprite.getPosition().x+sprite.getLocalBounds().width*sprite.getScale().x,sprite.getPosition().y+sprite.getLocalBounds().height*sprite.getScale().x};
}

float distancec(Sprite sprite1, Sprite sprite2)
{
    Vector2<float> S1 = centerS(sprite1);
    Vector2<float> S2 = centerS(sprite2);
    return sqrt(pow(S1.x-S2.x,2) + pow(S1.y - S2.y, 2));
}

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
        for (int i = 0; i < Objects.size(); i++) {
            if (distance(Objects[i], pos) < (sqrt(pow(window.getSize().x, 2) + pow(window.getSize().y, 2)) / 2)+100)
            {
                Objects[i].setTexture(Tex[i]);
                window.draw(Objects[i]);
            }
        }
    }

}

void Object::create_object(std::string texture_path, sf::Vector2f pos, bool auto_scale, sf::Vector2i size) {
    if(!aba.loadFromFile(texture_path))
    {
        printf("Error load texture");
        return;
    }
    aba.setSmooth(true);
    aba.setRepeated(false);
    object.setTexture(aba);
    Tex.push_back(aba);
    int width = object.getTexture()->getSize().x;
    int height = object.getTexture()->getSize().y;
    if (auto_scale) {
        if (width == height) {
            object.setScale(64.0f / width, 64.0f / height);
        }
        else if (width <= 2 * height && width >= height) {
            object.setScale(64.0f / width, 128.0f / height);
        }
        else if (height <= 2 * width && height >= width) {
            object.setScale(128.0f / width, 64.0f / height);
        } else {
            object.setScale(64.0f / width, 64.0f / height);
        }
    }

    if (!auto_scale && size.x != 0 && size.y != 0) {
        object.setScale(size.x / width, size.y / height);
    }
    object.setPosition(pos.x,pos.y);

    Objects.push_back(object);
}

