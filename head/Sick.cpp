#include "Sick.h"


void Sick::add_Seeck(Vector2f cords, string textures_path, int open_lvls, Vector2<int> size) {
    int id;

    if (all_seeck.empty())
        id = 1;
    else
        id = all_seeck[all_seeck.size() - 1].id + 1;

    Texture tex;
    tex.loadFromFile(textures_path);
    tex.setSmooth(true);

    Sprite sprite;
    sprite.setPosition(cords);
    sprite.setTexture(tex);
    sprite.setScale(size.x / sprite.getLocalBounds().width, size.y / sprite.getLocalBounds().height);

    all_seeck.push_back(sick_info{id, open_lvls, status_for_seeck::no, 0, sprite, tex});
}


void Sick::draw_Seeck(RenderWindow &window) {
    for (int i = 0; i < all_seeck.size(); i++) {
        all_seeck[i].sprite.setTexture(all_seeck[i].tex);
        window.draw(all_seeck[i].sprite);
    }
}

void Sick::update(float time, int level, int *score, int *angry) {
    for (auto &i: all_seeck) {


        if (i.open_lvl <= level) {

            if (i.status == status_for_seeck::no) {
                i.time_have = rand() % 30 + 30;
                i.status = status_for_seeck::normal;
            }

            if (i.status == status_for_seeck::normal) {
                if (i.time_have > 0) i.time_have -= time;
                else {
                    i.status = status_for_seeck::need_help;
                    i.time_have = 30;
                }
            }

            if (i.status == status_for_seeck::need_help) {
                if (i.time_have > 0) i.time_have -= time;
                else {
                    i.status = status_for_seeck::no;
                    *score -= 100;
                    *angry += 1;
                }
            }
            if (i.status == status_for_seeck::helping) {
                if (i.time_have > 0) i.time_have -= time;
                else {
                    i.status = status_for_seeck::no;
                }
            }
        }
    }
}