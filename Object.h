//
// Created by smers on 02.04.2022.
//
#include <SFML/Graphics.hpp>
#ifndef DURKACPP_OBJECT_H
#define DURKACPP_OBJECT_H



class Object {

public:
    std::vector<sf::Sprite> Objects;
    sf::Texture aba;
    void create_object(std::string texture_path, sf::Vector2f pos, int height = 0, int width = 0);
    void draw_objects(sf::RenderWindow &window);

private:
    sf::Sprite object;
};


#endif //DURKACPP_OBJECT_H
