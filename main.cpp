#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "head/Object.h"
#include "head/Player.h"
#include "head/UI.h"
#include "head/Sick.h"


using namespace std;
using namespace sf;

int score = 0;
int level = 0;
int speed_lv = 0;
int heal_lv = 0;
int angry = 0;


const int H = 12;
const int W = 40;


String ASCIIMAP[H] = {
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
        "B      B                         B     B",
        "B      B                         B     B",
        "BBBBB  BBB   AAAA                B     B",
        "B                                B     B",
        "B         OOOO                BBBB     B",
        "B                                B     B",
        "BBB                  O           B     B",
        "B              BB                BB    B",
        "B              BB                      B",
        "B    B         BB         BB           B",
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",

};

float distance(Sprite &sprite1, Sprite &sprite2)
{
    float x1 = sprite1.getPosition().x;
    float y1 = sprite1.getPosition().y;
    float x2 = sprite2.getPosition().x;
    float y2 = sprite2.getPosition().y;
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

float distance_p_s(Sprite seeck, Player sprite)
{
    float x1 = seeck.getPosition().x+seeck.getLocalBounds().width/2;
    float y1 = seeck.getPosition().y+seeck.getLocalBounds().height/2;
    float x2 = sprite.get_center().x;
    float y2 = sprite.get_center().y;
    return sqrt(pow(x1 - x2,2) +pow( y1 - y2,2));
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

void create_map(Object* ava)
{
    for (int i=0; i<H; i++) {
        for (int j = 0; j < W; j++) {
            if (ASCIIMAP[i][j] == 'B') {
                ava->create_object("sprites/brickTextureWhite.png", Vector2f(j*32, i*32),1,1);
            }
        }
    }
}

void camera(RenderWindow &window, Player &player,Vector2f cam_pos)  {
    View view;
    view.setSize(Vector2f(600, 400));
    cam_pos.x = player.player.getPosition().x;
    cam_pos.y = player.player.getPosition().y;
    view.setCenter(cam_pos);
    window.setView(view);
}

int may_help(Sick seeck,Player &player)
{
    auto ssa = nearest_sick(seeck, player.player);
    if (ssa.status == status_for_seeck::need_help)
    {
        std::cout << distance_p_s(ssa.sprite,player) << endl;
        if (distance_p_s(ssa.sprite,player) < 100)
        {
            for (int i = 0; i < seeck.all_seeck.size();i++)
            {
                cout << i << endl;
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
    RenderWindow window(VideoMode(1280, 800), "ha-ha");

    Vector2f cam_pos;

    Texture playerTexture;
    playerTexture.loadFromFile("sprites/down.png");
    playerTexture.setSmooth(true);
    auto player = Player(100,150, playerTexture);

    window.setFramerateLimit(60);
    Clock clock;
    float speed_up=1;

    Object all_objects;
    all_objects.create_object("sprites/brickTextureWhite.png", Vector2f(128,74), 10, 10);

    Object walls;
    create_map(&walls);

    vector<Vector2f> cors = {{200,200},{300,200}};
    vector<string> tex = {"sprites/down.png", "sprites/down.png"};
    vector<int> open_lvl = {0, 0};

    Sick seeck;
    seeck.add_Seeck(cors, tex, open_lvl);


    vector<Sprite> objects_with_collision;
    for (const auto & Object : all_objects.Objects)
    {
        objects_with_collision.push_back(Object);
    }
    for (const auto & Object : walls.Objects)
    {
        objects_with_collision.push_back(Object);
    }
    for (const auto & Object : seeck.all_seeck)
    {
        objects_with_collision.push_back(Object.sprite);
    }


    UI::Button bum = UI::Button(Vector2f(20,20), "sprites/healing.png");

    UI::Button button_up = UI::Button(Vector2f(20,20), "sprites/exit.png");


    float time_in_game = 0;
    float time_speed_up;

    int healing_status = 1;

    string top_bar_path = "sprites/bar.png";
    UI::Top_Bar top_bar = UI::Top_Bar(Vector2f(550,50), Vector2f(0,0), top_bar_path);


    while (window.isOpen())
    {
        Event event;

        while (window.pollEvent(event)) {}

        if (event.type == Event::Closed)
            window.close();

        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();


        time = clock.getElapsedTime().asMicroseconds();
        clock.restart();

        window.clear(Color::Red);

        time_in_game += time/1000000;


        if (time_in_game > 1)
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

        camera(window, player, cam_pos);

        all_objects.draw_objects(window, player.player.getPosition());

        walls.draw_objects(window , player.player.getPosition());

        seeck.draw_Seeck(window);

        player.draw_stamina(window);

        top_bar.draw(window, score, speed_lv, heal_lv, angry);

        UI::Lamps::draw(window,seeck, level);


        if (distance_p_s(nearest_sick(seeck,player.player).sprite,player) < 100)
        {
            if (nearest_sick(seeck,player.player).status == status_for_seeck::helping){
                healing_status = 0;
                bum.set_tex("sprites/healing.png");
                bum.draw(window, 40+player.get_position().x, 40+player.get_position().y);

            }

            else if (nearest_sick(seeck,player.player).status == status_for_seeck::need_help)
            {
                healing_status = 1;
                bum.set_tex("sprites/E.png");
                bum.draw(window, 40+player.get_position().x, 40+player.get_position().y);
            }
            else{
                healing_status =  1;
            }

        }


        if (Keyboard::isKeyPressed(Keyboard::E))
        {
            int ss = may_help(seeck,player);
            if (ss!= -1)
            {
                seeck.all_seeck[ss].status = status_for_seeck::helping;
                seeck.all_seeck[ss].time_have = 30;
            }
        }

        window.display();
    }
}
