
#include "Player.h"

Player::Player(float x, float y, Texture texture) {
    player_texture = texture;
    player.setTexture(player_texture);
    player.setScale({0.5, 0.5});

    inv_player_tex.loadFromFile("sprites/hit_box.png");
    inv_player.setTexture(inv_player_tex);
    inv_player.setScale({55 / inv_player.getLocalBounds().width, 120 / inv_player.getLocalBounds().height});
    inv_player.setPosition(x, y);
}

void Player::update(RenderWindow &window, float time_speed_up, float speed_up, vector<Sprite> *objects) {

    if ((Keyboard::isKeyPressed((Keyboard::W)) || Keyboard::isKeyPressed((Keyboard::Up))) &&
        (Keyboard::isKeyPressed((Keyboard::S)) || Keyboard::isKeyPressed((Keyboard::Down))))
        moveX = 0;

    else if (Keyboard::isKeyPressed((Keyboard::W)) || Keyboard::isKeyPressed((Keyboard::Up))) {
        last_status = 1;
        moveX = -0.1;

    } else if (Keyboard::isKeyPressed((Keyboard::S)) || Keyboard::isKeyPressed((Keyboard::Down))) {
        last_status = -1;
        moveX = 0.1;

    } else
        moveX = 0;

    if ((Keyboard::isKeyPressed((Keyboard::A)) || Keyboard::isKeyPressed(Keyboard::Left)) &&
        (Keyboard::isKeyPressed((Keyboard::D)) || Keyboard::isKeyPressed(Keyboard::Right)))
        moveY = 0;

    else if (Keyboard::isKeyPressed((Keyboard::A)) || Keyboard::isKeyPressed(Keyboard::Left)) {
        last_status = -2;
        moveY = -0.1;

    } else if (Keyboard::isKeyPressed((Keyboard::D)) || Keyboard::isKeyPressed(Keyboard::Right)) {
        last_status = 2;
        moveY = 0.1;

    } else
        moveY = 0;


    float speed = 0.065555;
    if ((Keyboard::isKeyPressed((Keyboard::W)) || Keyboard::isKeyPressed(Keyboard::Up)) &&
        (Keyboard::isKeyPressed((Keyboard::D)) || Keyboard::isKeyPressed(Keyboard::Right))) {
        moveX = -speed;
        moveY = speed;

    } else if ((Keyboard::isKeyPressed((Keyboard::W)) || Keyboard::isKeyPressed(Keyboard::Up)) &&
               (Keyboard::isKeyPressed((Keyboard::A)) || Keyboard::isKeyPressed(Keyboard::Left))) {
        moveX = -speed;
        moveY = -speed;

    } else if ((Keyboard::isKeyPressed((Keyboard::S)) || Keyboard::isKeyPressed(Keyboard::Down)) &&
               (Keyboard::isKeyPressed((Keyboard::D)) || Keyboard::isKeyPressed(Keyboard::Right))) {
        moveX = speed;
        moveY = speed;

    } else if ((Keyboard::isKeyPressed((Keyboard::S)) || Keyboard::isKeyPressed(Keyboard::Down)) &&
               (Keyboard::isKeyPressed((Keyboard::A)) || Keyboard::isKeyPressed(Keyboard::Left))) {
        moveX = speed;
        moveY = -speed;
    }


    staminas(time_speed_up);

    int player_x_ = inv_player.getPosition().x;
    int player_y_ = inv_player.getPosition().y;

    int player_x = inv_player.getPosition().x + moveY * time_speed_up * speed_up * stamina_speed_up;
    int player_y = inv_player.getPosition().y + moveX * time_speed_up * speed_up * stamina_speed_up;

    for (auto obj: *objects) {
        if (collision_wall(player_x_, player_y, &obj, &inv_player)) {
            moveX = 0;
        }
        if (collision_wall(player_x, player_y_, &obj, &inv_player)) {
            moveY = 0;
        }
    }

    inv_player.move(moveY * time_speed_up * speed_up * stamina_speed_up,
                    moveX * time_speed_up * speed_up * stamina_speed_up);

    Vector2<float> cent_pos;
    cent_pos.x = window.getView().getCenter().x - player.getLocalBounds().width * player.getScale().x;
    cent_pos.y = window.getView().getCenter().y - player.getLocalBounds().height * player.getScale().y;

    player.setPosition(cent_pos);
    tex_path = "sprites/player/";
    if ((moveX != 0 || moveY != 0) && time_speed_up != 0) {
        switch (last_status) {
            case -1:
                tex_path += "down_";
                break;
            case 1:
                tex_path += "up_";
                break;
            case 2:
                tex_path += "right_";
                break;
            case -2:
                tex_path += "left_";
                break;
        }
        tex_path += to_string(frame) + ".png";
    }

    else {
        switch (last_status) {
            case -1:
                tex_path += "down.png";
                break;
            case 1:
                tex_path += "up.png";
                break;
            case 2:
                tex_path += "right.png";
                break;
            case -2:
                tex_path += "left.png";
                break;
        }
    }

    if (counter == 15) {
        if (frame == 1)
            frame = 2;
        else
            frame = 1;
        counter = 0;
    }
    counter++;

    player_texture.loadFromFile(tex_path);
    player.setTexture(player_texture, true);

    player.setScale({64 / player.getLocalBounds().width, 128 / player.getLocalBounds().height});

    window.draw(player);
}

void Player::staminas(float time) {
    if (stamina < 0) stamina = 0;
    if (Keyboard::isKeyPressed((Keyboard::LShift)) || Keyboard::isKeyPressed(Keyboard::RShift)) {

        wait_time_to_recovery_stamina = 0;

        if (stamina > 0) {
            stamina -= 0.30;
            stamina_speed_up = 4;
        } else {
            stamina_speed_up = 1;
        }

    }

    else {
        if (wait_time_to_recovery_stamina <= time_to_recovery_stamina) wait_time_to_recovery_stamina += time / 1000;
        if (stamina < 10) {
            if (wait_time_to_recovery_stamina >= time_to_recovery_stamina) {
                stamina += 0.30 * time / 1000;
            }
            stamina_speed_up = 1;
        }

        else {
            stamina_speed_up = 1;
        }
    }
}

Vector2<float> Player::get_center() {
    Vector2<float> position;
    position.x = player.getPosition().x + player.getTexture()->getSize().x / 2;
    position.y = player.getPosition().y + player.getTexture()->getSize().y / 2;
    return position;
}

bool Player::collision_wall(int player_x, int player_y, Sprite *obj, Sprite *hit) {

    float wall_x = obj->getPosition().x;
    float wall_y = obj->getPosition().y;

    float scale_x = obj->getScale().x;
    float scale_y = obj->getScale().y;

    return (player_x + hit->getLocalBounds().width * hit->getScale().x >= wall_x &&
            player_x <= wall_x + obj->getLocalBounds().width * scale_x)
           && (player_y + hit->getLocalBounds().height * hit->getScale().y >= wall_y &&
               player_y <= wall_y + obj->getLocalBounds().height * scale_y);
}