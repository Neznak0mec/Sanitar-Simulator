#include <SFML/Graphics.hpp>
#include <math.h>

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H


class Object {
public:
    std::vector<sf::Sprite> Objects;
    std::vector<sf::Texture> Tex;
    sf::Texture aba;

    void create_object(std::string texture_path, sf::Vector2f pos, bool auto_scale, sf::Vector2f size);

    void draw_objects(sf::RenderWindow &window, sf::Vector2<float> pos);

private:
    sf::Sprite object;
};


#endif 