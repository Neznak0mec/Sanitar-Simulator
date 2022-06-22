#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <SFML/Graphics.hpp>
#include "Object.h"

using namespace sf;
using namespace std;


class Player {
public:
    Sprite player;
    Sprite inv_player;
    Texture inv_player_tex;
    Texture player_texture;

    Player(float x, float y, Texture texture);

    void update(RenderWindow &window, float time_speed_up, float speed_up, vector<Sprite> *objects);

    void staminas(float time);

    static bool collision_wall(int player_x, int player_y, Sprite *obj, Sprite *hit);

    Vector2<float> get_center();

private:
    float moveY;
    float moveX;
    int frame = 0;
    int last_status = -1;
    int counter = 0;
    string tex_path;
    float stamina = 10;
    float stamina_speed_up = 1;
    float time_to_recovery_stamina = 3;
    float wait_time_to_recovery_stamina = 0;
};


#endif