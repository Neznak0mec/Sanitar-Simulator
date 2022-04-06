//
// Created by smers on 05.04.2022.
//

#ifndef DURKACPP_PLAYER_H
#define DURKACPP_PLAYER_H
#include <SFML/Graphics.hpp>
#include "Object.h"
using namespace sf;
using namespace std;


class Player {
public:
    Sprite player;
    Texture player_texture;
    Player(float x, float y, Texture texture);
    void update(RenderWindow &window,float time_speed_up, float speed_up, vector<Object> *objects);
    void staminas(float time);
    void draw_stamina(RenderWindow &window);
    bool collision_wall(int player_x, int player_y, Sprite &obj);
    Vector2<float> get_position();
private:
    float moveY;
    float moveX;
    float stamina = 7;
    float stamina_speed_up= 1;
    float time_to_recovery_stamina = 3;
    float wait_time_to_recovery_stamina = 0;
};



#endif //DURKACPP_PLAYER_H
