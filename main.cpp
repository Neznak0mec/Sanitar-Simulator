#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include "head/Object.h"
#include "head/Player.h"
#include "head/UI.h"
#include "head/Sick.h"
#include "head/json.hpp"

using namespace std;
using namespace sf;

int score = 0;
int level = 0;
int speed_lv = 0;
int heal_lv = 0;
int angry = 0;

Vector2<float> center(Sprite sprite)
{
    return {sprite.getPosition().x+sprite.getLocalBounds().width*sprite.getScale().x,sprite.getPosition().y+sprite.getLocalBounds().height*sprite.getScale().x};
}


float distance(Sprite sprite1, Sprite sprite2)
{
   Vector2<float> S1 = center(sprite1);
   Vector2<float> S2 = center(sprite2);
   return sqrt(pow(S1.x-S2.x,2) + pow(S1.y - S2.y, 2));
}

Sick::sick_info nearest_sick(Sick &seeck, Sprite &player)
{
    float min_distance = distance(player, seeck.all_seeck[0].sprite);
    int index = 0;
    for (int i = 1; i < seeck.all_seeck.size(); i++)
    {
        float distance_to = distance(player, seeck.all_seeck[i].sprite);
        if (distance_to < min_distance)
        {
            min_distance = distance_to;
            index = i;
        }
    }
    return seeck.all_seeck[index];
}

void create_map(vector<string> map,Object* walls,Object* obj,Sick* sick,vector<Sprite>* colis, Player* player, nlohmann::json fil)
{
    for (int i=0; i<map.size(); i++) {
        for (int j = 0; j < map[i].length(); j++)
        {
            string elem(1, map[i][j]);


            if (fil[elem]["type"].is_null() || elem == " ")
            {
                continue;
            }


            if (fil[elem]["type"].get<string>()=="wall")
            {
                walls->create_object(fil[elem]["tex_path"].get<string>(), Vector2f(j*64, i*64), fil[elem]["auto_scale"].get<bool>());

                colis->push_back(walls->Objects[walls->Objects.size()-1]);
            }


            if (fil[elem]["type"].get<string>()=="obj")
            {
                obj->create_object(fil[elem]["tex_path"].get<string>(), Vector2f(j*64, i*64), fil[elem]["auto_scale"].get<bool>());

                if (fil[elem]["collision"].get<bool>())
                    colis->push_back(obj->Objects[obj->Objects.size()-1]);
            }


            if (fil[elem]["type"].get<string>()=="sick")
            {
                Vector2<int> size = {fil[elem]["width"].get<int>(),fil[elem]["height"].get<int>()};

                sick->add_Seeck(Vector2f(j*64, i*64),fil[elem]["tex_path"].get<string>(),fil[elem]["lvl"].get<int>(),size);
                colis->push_back(sick->all_seeck[sick->all_seeck.size()-1].sprite);
            }


            if (fil[elem]["type"].get<string>()=="player")
            {
                player->player.setPosition(Vector2f(j*64, i*64));
            }
        }
    }
}

void camera(RenderWindow &window, Player &player)
{
    View view;
    view.setSize(Vector2f(1400, 840));
    view.setCenter(player.player.getPosition());
    window.setView(view);
}

int may_help(Sick seeck,Player &player)
{
    auto ssa = nearest_sick(seeck, player.player);
    if (ssa.status == status_for_seeck::need_help)
    {
        if (distance(ssa.sprite,player.player) < 100)
        {
            for (int i = 0; i < seeck.all_seeck.size();i++)
            {
                if (seeck.all_seeck[i].id == ssa.id)
                {
                    score += 7;
                    return i;
                }
            }
        }
    }
    return -1;
}

int main()
{
    float time;
    RenderWindow window(VideoMode(1400, 840), "ha-ha");

    fstream file("map.json");
    nlohmann::json jf = nlohmann::json::parse(file);
    vector<string> map = jf["map"].get<std::vector<string>>();
    file.close();

    Texture playerTexture;
    playerTexture.loadFromFile("sprites/down.png");
    playerTexture.setSmooth(true);
    Player player = Player(100,150, playerTexture);

    window.setFramerateLimit(60);
    Clock clock;
    float speed_up=1;



    Object all_objects;
    Sick seeck;
    Object walls;
    vector<Sprite> objects_with_collision;


    create_map(map,&walls,&all_objects,&seeck,&objects_with_collision,&player, jf);

    UI::Button heal = UI::Button(Vector2f(20,20), "sprites/healing.png");

    UI::Button exitB = UI::Button(Vector2f(50,50), "sprites/exit.png");


    float time_in_game = 0;
    float time_speed_up;

    int healing_status = 1;

    string top_bar_path = "sprites/bar.png";
    UI::Top_Bar top_bar = UI::Top_Bar(Vector2f(1100,100), Vector2f(0,0), top_bar_path);

    bool pause = false;
    Sprite pause_sprite;
    Texture pause_texture;
    pause_texture.loadFromFile("sprites/pause.png");
    pause_sprite.setTexture(pause_texture);
    pause_sprite.setPosition(Vector2f(0,0));
    pause_sprite.setScale(Vector2f(0.5,0.5));

    string last_button = "";

    bool menu_open = true;

    while (window.isOpen())
    {
        Event event;

        while (window.pollEvent(event)) {}

        if (event.type == Event::Closed)
            window.close();

        if (exitB.is_clicked(window))
        {
            window.close();
        }


        if (menu_open) {
            menu_open = UI::menu(window, Mouse::getPosition(window));
            continue;
        }


        time = clock.getElapsedTime().asMicroseconds();
        clock.restart();

        window.clear(Color::Green);

        time_in_game += time/1000000;

        if (pause)
            time = 0;



        if (time_in_game > 1 and !pause)
        {
            int last_score = score;
            score = seeck.update(time_in_game, level, score);
            if (score != last_score)
            {
                angry++;
            }
            time_in_game = 0;
        }

        if (nearest_sick(seeck, player.player).status == status_for_seeck::helping)
            time_speed_up = 0;
        else
            time_speed_up = time/1000;



        player.update(window , time_speed_up ,speed_up*healing_status, &objects_with_collision);

        camera(window, player);

        all_objects.draw_objects(window, player.player.getPosition());

        walls.draw_objects(window , player.player.getPosition());

        seeck.draw_Seeck(window);

        player.draw_stamina(window);

        top_bar.draw(window, score, speed_lv, heal_lv, angry);

        UI::Lamps::draw(window,seeck, level);

        exitB.draw(window,player.player.getPosition().x+640,player.player.getPosition().y-410);


        if (distance(player.player,nearest_sick(seeck,player.player).sprite) < 100)
        {
            if (nearest_sick(seeck,player.player).status == status_for_seeck::helping){
                healing_status = 0;
                heal.set_tex("sprites/healing.png");
                heal.draw(window, 40+player.get_position().x, 40+player.get_position().y);

            }

            else if (nearest_sick(seeck,player.player).status == status_for_seeck::need_help)
            {
                healing_status = 1;
                heal.set_tex("sprites/E.png");
                heal.draw(window, 40+player.get_position().x, 40+player.get_position().y);
            }
            else{
                healing_status =  1;
            }

        }

        if (Keyboard::isKeyPressed(Keyboard::E))
        {
            int mh = may_help(seeck,player);
            if (mh != -1)
            {
                seeck.all_seeck[mh].status = status_for_seeck::helping;
                seeck.all_seeck[mh].time_have = 30;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape) and last_button != "escape")
        {
            if (pause)
            {
                pause = false;
            }
            else
            {
                pause = true;
            }
            last_button = "escape";
        }
        if (!Keyboard::isKeyPressed(Keyboard::Escape))
            last_button = "";

        if (pause)
        {
            pause_sprite.setPosition(window.getView().getCenter()-Vector2f(250,100));
            window.draw(pause_sprite);
        }


        window.display();
    }
}
