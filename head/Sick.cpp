//
// Created by noname on 15.04.22.
//

#include "Sick.h"


void Sick::add_Seeck(Vector2f cords, string textures_path, int open_lvls,Vector2<int> size){
        int id;
        if (all_seeck.empty())
            id = 1;
        else
            id = all_seeck[all_seeck.size()-1].id+1;
        Texture tex;
        tex.loadFromFile(textures_path);
        tex.setSmooth(true);
        Sprite sprite;
        sprite.setPosition(cords);
        sprite.setTexture(tex);
        sprite.setScale(size.x/sprite.getLocalBounds().width,size.y/sprite.getLocalBounds().height);
        all_seeck.push_back(sick_info{id ,open_lvls, status_for_seeck::no, 0 ,sprite, tex});
}



void Sick::draw_Seeck(RenderWindow &window){
    for (int i = 0; i < all_seeck.size(); i++) {
        all_seeck[i].sprite.setTexture(all_seeck[i].tex);
        window.draw(all_seeck[i].sprite);
    }
}

void Sick::update(float time, int level,int* score)
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
                    *score-=100;
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
}