#include "Object.h"

using namespace sf;



float distance(sf::Sprite &sprite1, sf::Vector2<float> pos) {
    float x1 = sprite1.getPosition().x;
    float y1 = sprite1.getPosition().y;
    float x2 = pos.x;
    float y2 = pos.y;
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void Object::draw_objects(sf::RenderWindow &window, sf::Vector2<float> pos) {
    {
        for (int i = 0; i < Objects.size(); i++) {
            Objects[i].setTexture(Tex[i]);
            window.draw(Objects[i]);
        }
    }

}

void Object::create_object(std::string texture_path, sf::Vector2f pos, bool auto_scale, sf::Vector2f size) {
    if (!aba.loadFromFile(texture_path)) {
        return;
    }

    aba.setSmooth(true);
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
        }
        else {
            object.setScale(64.0f / width, 64.0f / height);
        }
    }

    if (!auto_scale && size.x != 0 && size.y != 0) {
        object.setScale(size.x / width, size.y / height);
    }

    object.setPosition(pos.x, pos.y);
    Objects.push_back(object);
}

