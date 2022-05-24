//
// Created by smers on 05.04.2022.
//

#ifndef DURKACPP_UI_H
#define DURKACPP_UI_H
#include <SFML/Graphics.hpp>
#include "Sick.h"
#include <math.h>


using namespace std;
using namespace sf;

namespace UI{

    class rec_Button{
        public:
        sf::RectangleShape rect;
        string text_button;

        rec_Button(Vector2<float> size, Color color, Vector2<float> pos, string text);

        void draw(RenderWindow &window);

        bool is_clicked(RenderWindow &window);

        void set_pos(Vector2f pos);

    };

    class Button{
    public:
        sf::RectangleShape shape;

        Button(Vector2<float> size, string tex_path);
        void draw(RenderWindow &window,float pos1, float pos2);

        void set_tex(string tex_path);

        bool is_clicked(RenderWindow &window);

    private:
        Vector2<int> pos;
        Vector2<int> size;
        Texture tex;
    };

    class Top_Bar{
        public:
        sf::RectangleShape shape;

        Top_Bar(Vector2<float> size,Vector2<float> pos, string tex_path);
        void draw(RenderWindow &window,int sc ,int sl, int hl, int ang);

    private:
        Texture bar_tex;
    };

    class Lamps{
        public:
        sf::RectangleShape shape;

        static void draw(RenderWindow &window, Sick &sick,int lvl);
    };


    bool menu(RenderWindow &window, bool* guide_is);

    class Upgrade_menu
            {
        bool is_clicked = false;
        int help = 0;

    public:
        void update(RenderWindow &window,int* sc ,int* sl, int* hl, int* ang, int* lvl);
    };



    void game_over_sreen(RenderWindow &window,int* sc ,int* sl, int* hl, int* ang, int* lvl,bool* menu_open, bool* game_over,Sprite* p, Vector2<float> s_pos);

};

#endif //DURKACPP_UI_H
