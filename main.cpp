#include <SFML/Graphics.hpp>
#include <iostream>
#include "Object.h"
#include "Player.h"
#include "UI.h"


using namespace std;
using namespace sf;

int score = 0;
int level = 0;


class seeck{


private:

    enum class status_for_seeck {
        no,
        normal,
        need_help,
        helping
    };

public:

    struct seek{
        int open_lvl;
        status_for_seeck status;
        int time_have;
        Texture texture;
        Sprite sprite;
    };



    vector<seek> all_seeck;


    void add_Seeck(vector<Vector2f> cords, vector<Texture> textures, vector<int> open_lvls){
        for (int i = 0; i < cords.size(); i++) {
            Sprite sprite;
            sprite.setTexture(textures[i]);
            sprite.setPosition(cords[i]);
            all_seeck.push_back(seek{open_lvls[i], status_for_seeck::no, 0, textures[i], sprite});
        }
    }

    void draw_Seeck(RenderWindow &window){
        for (int i = 0; i < all_seeck.size(); i++) {
            window.draw(all_seeck[i].sprite);
        }
    }

    void update(float time)
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

                if (all_seeck[i].status == status_for_seeck::need_help)
                {
                    if (all_seeck[i].time_have > 0)
                        all_seeck[i].time_have -= time;
                    else
                    {
                        score -= 10;
                        all_seeck[i].status = status_for_seeck::no;
                    }
                }
            }
        }
    }

};

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



seeck::seek nearest_seek(seeck &seeck, Sprite &player)
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


int main()
{
    float time;
    RenderWindow window(VideoMode(600, 400), "Hyila ha-ha");

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

    vector<Object> objects_with_collision = {all_objects, walls};


    UI::Button bum = UI::Button(Vector2f(100,100), "sprites/button.png");




    while (window.isOpen())
    {
        Event event;

        while (window.pollEvent(event)) {
        }

        if (event.type == Event::Closed)
            window.close();

        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();


        time = clock.getElapsedTime().asMicroseconds();
        int time_speed_up = time/1000;
        clock.restart();


        window.clear(Color::White);



        player.update(window , time_speed_up ,speed_up, &objects_with_collision);
        camera(window, player, cam_pos);
        player.draw_stamina(window);
        all_objects.draw_objects(window);
        walls.draw_objects(window);
        bum.draw(window, 40+ cam_pos.x, 40+ cam_pos.y);
        window.display();
    }
}