//
// Created by noname on 15.04.22.
//

#include "Sick.h"


void Sick::add_Seeck(Vector2f cords, string textures_path, int open_lvls){
        Sprite sprite;
        Texture tex;
        tex.loadFromFile(textures_path);
        tex.setSmooth(true);
        sprite.setTexture(tex);
        sprite.setPosition(cords);
        all_seeck.push_back(sick_info{rand()%70,open_lvls, status_for_seeck::no, 0, tex, sprite});

}



void Sick::draw_Seeck(RenderWindow &window){
    for (int i = 0; i < all_seeck.size(); i++) {
        window.draw(all_seeck[i].sprite);
    }
}

int Sick::update(float time, int level,int score)
{
    for (int i = 0; i < all_seeck.size(); i++)
    {
        if (all_seeck[i].open_lvl <= level)
        {

            if (all_seeck[i].status == status_for_seeck::no)
            {
                all_seeck[i].time_have = rand() % 30 + 30;
                all_seeck[i].status = status_for_seeck::normal;
            }

            if (all_seeck[i].status == status_for_seeck::normal)
            {
                if (all_seeck[i].time_have > 0)
                    all_seeck[i].time_have -= time;
                else
                {
                    all_seeck[i].status = status_for_seeck::need_help;
                    all_seeck[i].time_have = 30;
                }
            }

            if (all_seeck[i].status == status_for_seeck::need_help) {
                if (all_seeck[i].time_have > 0)
                    all_seeck[i].time_have -= time;
                else {

                    all_seeck[i].status = status_for_seeck::no;
                    return score-10;
                }
            }
            if (all_seeck[i].status == status_for_seeck::helping) {
                if (all_seeck[i].time_have > 0)
                    all_seeck[i].time_have -= time;
                else {
                    all_seeck[i].status = status_for_seeck::no;
                }
            }
        }
    }
    return score;
}