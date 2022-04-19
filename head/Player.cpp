//
// Created by smers on 05.04.2022.
//

#include "Player.h"

Player::Player(float x, float y, Texture texture) {
    player_texture = texture;
    player.setTexture(player_texture);
    player.setPosition(x,y);
    player.setScale({0.5,0.5});
}

void Player::update(RenderWindow &window, float time_speed_up, float speed_up, vector<Sprite> *objects) {
    if (Keyboard::isKeyPressed((Keyboard::W)) && Keyboard::isKeyPressed((Keyboard::S)))
        moveX = 0;

    else if(Keyboard::isKeyPressed((Keyboard::W)))
        moveX = -0.1;


    else if(Keyboard::isKeyPressed((Keyboard::S)))
        moveX = 0.1;

    else
        moveX = 0;

    if (Keyboard::isKeyPressed((Keyboard::A)) && Keyboard::isKeyPressed((Keyboard::D)))
        moveY = 0;

    else if(Keyboard::isKeyPressed((Keyboard::A)))
        moveY = -0.1;

    else if(Keyboard::isKeyPressed((Keyboard::D)))
        moveY = 0.1;

    else
        moveY = 0;


    float speed= 0.065555;

    if (Keyboard::isKeyPressed((Keyboard::W)) && Keyboard::isKeyPressed((Keyboard::D)))
    {
        moveX = -speed;
        moveY = speed;
    }

    else if(Keyboard::isKeyPressed((Keyboard::W)) && Keyboard::isKeyPressed((Keyboard::A))){
        moveX = -speed;
        moveY = -speed;
    }

    else if(Keyboard::isKeyPressed((Keyboard::S)) && Keyboard::isKeyPressed((Keyboard::D))){
        moveX = speed;
        moveY = speed;
    }

    else if(Keyboard::isKeyPressed((Keyboard::S)) && Keyboard::isKeyPressed((Keyboard::A))){
        moveX = speed;
        moveY = -speed;
    }


    staminas(time_speed_up);

    //collision with wall
    int player_x_ = player.getPosition().x;
    int player_y_ = player.getPosition().y;

    int player_x = player.getPosition().x+moveY*time_speed_up*speed_up*stamina_speed_up;
    int player_y = player.getPosition().y+moveX*time_speed_up*speed_up*stamina_speed_up;




    for (auto obj : *objects)
    {
            if (collision_wall(player_x_, player_y, obj))
            {
                moveX = 0;
            }
            if (collision_wall(player_x, player_y_,obj))
            {
                moveY = 0;
            }

    }

    player.move(moveY*time_speed_up*speed_up*stamina_speed_up,moveX*time_speed_up*speed_up*stamina_speed_up);

    window.draw(player);
}

void Player::staminas(float time) {
    if (stamina < 0)
        stamina = 0;
    if (Keyboard::isKeyPressed((Keyboard::LShift)))
    {
        wait_time_to_recovery_stamina = 0;
        if (stamina > 0)
        {
            stamina -= 0.30;
            stamina_speed_up = 4;
        }
        else
        {
            stamina_speed_up = 1;
        }
    }
    else
    {
        if(wait_time_to_recovery_stamina <= time_to_recovery_stamina)
            wait_time_to_recovery_stamina += time/1000;
        if (stamina < 7)
        {
            if (wait_time_to_recovery_stamina >= time_to_recovery_stamina)
            {
                stamina += 0.30*time/1000;
            }
            stamina_speed_up = 1;
        }
        else
        {
            stamina_speed_up = 1;
        }
    }
}

void Player::draw_stamina(RenderWindow &window) {
    RectangleShape stamina_bar;
    stamina_bar.setSize(Vector2f(5, stamina*10));
    stamina_bar.setPosition(player.getPosition().x - 290, player.getPosition().y );
    Color color(166, 0, 144);
    stamina_bar.setFillColor(color);
    window.draw(stamina_bar);
}

bool Player::collision_wall(int player_x, int player_y, Sprite &obj) {
    int wall_x = obj.getPosition().x;
    int wall_y = obj.getPosition().y;

    if(player_x + 32 >= wall_x && player_x <= wall_x + obj.getLocalBounds().width && player_y + 62 >= wall_y && player_y <= wall_y + obj.getLocalBounds().height)
        return true;
    else
        return false;
}

Vector2<float> Player::get_position() {
    return player.getPosition();
}

Vector2<float> Player::get_center() {
    Vector2<float> position;
    position.x = player.getPosition().x+player.getTexture()->getSize().x/2;
    position.y = player.getPosition().y+player.getTexture()->getSize().y/2;
    return position;
}