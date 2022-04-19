//
// Created by noname on 15.04.22.
//
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;
#ifndef DURKACPP_SICK_H
#define DURKACPP_SICK_H


enum class status_for_seeck {
    no,
    normal,
    need_help,
    helping
};

class Sick{
public:

    struct sick_info{
        int id;
        int open_lvl;
        status_for_seeck status;
        int time_have;
        Texture texture;
        Sprite sprite;
    };

    vector<sick_info> all_seeck;

    void add_Seeck(vector<Vector2f> cords, vector<string> textures_path, vector<int> open_lvls);

    void draw_Seeck(RenderWindow &window);

    int update(float time, int level, int score);

};


#endif //DURKACPP_SICK_H