#include <SFML/Graphics.hpp>
#include <fstream>
#include "head/Object.h"
#include "head/Player.h"
#include "head/UI.h"
#include "head/Sick.h"
#include "head/json.hpp"


using namespace std;
using namespace sf;

int score = 0;
int level = 0;
int speed_lv = 1;
int heal_lv = 0;
int angry = 0;

Vector2<float> p_start_position = {0, 0};

// get center of sprite
Vector2<float> center(Sprite sprite) {
    return {sprite.getPosition().x + sprite.getLocalBounds().width * sprite.getScale().x,
            sprite.getPosition().y + sprite.getLocalBounds().height * sprite.getScale().y};
}

// get distance between two sprites
float distance(Sprite sprite1, Sprite sprite2) {
    Vector2<float> S1 = center(sprite1);
    Vector2<float> S2 = center(sprite2);
    return sqrt(pow(S1.x - S2.x, 2) + pow(S1.y - S2.y, 2));
}

// get info about the nearest sick
Sick::sick_info nearest_sick(Sick &seeck, Sprite &player) {
    float min_distance = distance(player, seeck.all_seeck[0].sprite);
    int index = 0;
    for (int i = 1; i < seeck.all_seeck.size(); i++) {
        float distance_to = distance(player, seeck.all_seeck[i].sprite);
        if (distance_to < min_distance) {
            min_distance = distance_to;
            index = i;
        }
    }
    return seeck.all_seeck[index];
}


void create_map(vector<string> map, Object *walls, Object *obj, Sick *sick, vector<Sprite> *colis, Player *player,
                Object *floor, Object *book, nlohmann::json fil, RenderWindow &window) {
    srand(time(NULL));
    std::vector<std::string> paths = {"sprites/floor/floor1.png", "sprites/floor/floor2.png",
                                      "sprites/floor/floor3.png", "sprites/floor/floor4.png"};

    int sizi = 0;
    for (string str: map)
        sizi += str.length();


    int counter = 0;
    float proc = sizi / 100;
    int progress = 0;


    Vector2<float> start_pos = {window.getView().getCenter().x - window.getView().getSize().x / 2,
                                window.getView().getCenter().y - window.getView().getSize().y / 2};

    RectangleShape progress_bar;
    progress_bar.setSize(Vector2f(progress, 20));
    progress_bar.setFillColor(Color::White);
    progress_bar.setPosition(start_pos.x + 1000, start_pos.y + 700);

    Sprite load;
    Texture load_texture;
    load_texture.loadFromFile("sprites/loading.png");
    load.setTexture(load_texture);
    load.setPosition(Vector2f(start_pos.x + 965, start_pos.y + 610));
    load.setScale(Vector2f(1.6, 1));


    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].length(); j++) {
            window.clear();
            string elem(1, map[i][j]);

            if (fil[elem]["type"].is_null() || elem == " ") {
                floor->create_object(paths[rand() % 4], Vector2f(j * 64, i * 64), true, {0, 0});
            }


            else if (fil[elem]["type"].get<string>() == "wall") {
                Vector2f size = {0, 0};
                if (!fil[elem]["size"].empty())
                    size = {fil[elem]["size"].get<vector<float>>()[0], fil[elem]["size"].get<vector<float>>()[1]};

                bool scale = false;
                if (!fil[elem]["auto_scale"].empty()) scale = fil[elem]["auto_scale"].get<bool>();

                walls->create_object(fil[elem]["tex_path"].get<string>(), Vector2f(j * 64, i * 64), scale, size);
                colis->push_back(walls->Objects[walls->Objects.size() - 1]);
            }


            else if (fil[elem]["type"].get<string>() == "obj") {
                Vector2f size = {0, 0};
                if (!fil[elem]["size"].empty())
                    size = {fil[elem]["size"].get<vector<float>>()[0], fil[elem]["size"].get<vector<float>>()[1]};

                bool scale = false;
                if (!fil[elem]["auto_scale"].empty()) scale = fil[elem]["auto_scale"].get<bool>();


                obj->create_object(fil[elem]["tex_path"].get<string>(), Vector2f(j * 64, i * 64), scale, size);

                if (fil[elem]["collision"].get<bool>() || fil[elem]["collision"].empty())
                    colis->push_back(obj->Objects[obj->Objects.size() - 1]);

                floor->create_object(paths[rand() % 4], Vector2f(j * 64, i * 64), true, {0, 0});
            }

            else if (fil[elem]["type"].get<string>() == "sick") {

                Vector2i size = {0, 0};
                if (!fil[elem]["size"].empty())
                    size = {fil[elem]["size"].get<vector<int>>()[0], fil[elem]["size"].get<vector<int>>()[1]};

                bool scale = false;
                if (!fil[elem]["auto_scale"].empty()) scale = fil[elem]["auto_scale"].get<bool>();

                sick->add_Seeck(Vector2f(j * 64, i * 64), fil[elem]["tex_path"].get<string>(),
                                fil[elem]["lvl"].get<int>(), size);

                colis->push_back(sick->all_seeck[sick->all_seeck.size() - 1].sprite);
                floor->create_object(paths[rand() % 4], Vector2f(j * 64, i * 64), true, {0, 0});
            }

            else if (fil[elem]["type"].get<string>() == "player") {
                player->inv_player.setPosition(Vector2f(j * 64, i * 64));
                floor->create_object(paths[rand() % 4], Vector2f(j * 64, i * 64), true, {0, 0});
                p_start_position = Vector2f(j * 64, i * 64);
            }

            else if (fil[elem]["type"].get<string>() == "book") {

                Vector2f size = {0, 0};
                if (!fil[elem]["size"].empty())
                    size = {fil[elem]["size"].get<vector<float>>()[0], fil[elem]["size"].get<vector<float>>()[1]};

                bool scale = false;
                if (!fil[elem]["auto_scale"].empty()) scale = fil[elem]["auto_scale"].get<bool>();

                book->create_object(fil[elem]["tex_path"].get<string>(), Vector2f(j * 64, i * 64), scale, size);
                floor->create_object(paths[rand() % 4], Vector2f(j * 64, i * 64), true, {0, 0});
                colis->push_back(walls->Objects[book->Objects.size() - 1]);
            }


            Event event;

            while (window.pollEvent(event)) {}

            if (event.type == Event::Closed) window.close();

            counter++;
            progress = counter / proc;
            if (progress > 100) progress = 100;
            progress_bar.setSize(Vector2f(progress * 2.847, 20));

            window.draw(load);
            window.draw(progress_bar);
            window.display();
        }
    }
}


void camera(RenderWindow &window, Player &player) {
    View view;
    view.setSize(Vector2f(1400, 840));
    view.setCenter(center(player.inv_player));
    window.setView(view);
}


int may_help(Sick seeck, Player &player) {
    auto ssa = nearest_sick(seeck, player.inv_player);
    if (ssa.status == status_for_seeck::need_help && distance(ssa.sprite, player.inv_player) < 140) {
        score += 90 + (90 * level / 5);
        return ssa.id - 1;
    }

    return -1;
}

int main() {

    float time;
    RenderWindow window(VideoMode(1400, 840), "");
    window.setFramerateLimit(60);


    auto icon = Image();
    icon.loadFromFile("icon.png");


    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    fstream file("map.json");


    nlohmann::json mapjs = nlohmann::json::parse(file);
    vector<string> map = mapjs["map"].get<std::vector<string>>();
    file.close();

    fstream file2("guide.json");
    nlohmann::json guidejs = nlohmann::json::parse(file2);
    vector<string> guide = guidejs["map"].get<std::vector<string>>();
    file.close();


    Texture playerTexture;
    playerTexture.loadFromFile("sprites/player/down.png");
    Player player = Player(100, 150, playerTexture);


    Clock clock;

    Object floor;
    Object all_objects;
    Sick seeck;
    Object walls;
    vector<Sprite> objects_with_collision;
    Object book;

    UI::Button heal = UI::Button(Vector2f(40, 40), "sprites/healing.png");
    UI::Button exitB = UI::Button(Vector2f(50, 50), "sprites/exit.png");
    UI::rec_Button to_menu = UI::rec_Button({150, 75}, Color::Black, {0, 0}, "Menu");

    float time_in_game = 0;
    float time_speed_up;

    int healing_status = 1;
    string top_bar_path = "sprites/bar.png";
    UI::Top_Bar top_bar = UI::Top_Bar(Vector2f(1100, 100), Vector2f(0, 0), top_bar_path);


    bool pause = false;
    Sprite pause_sprite;
    Texture pause_texture;
    pause_texture.loadFromFile("sprites/pause.png");
    pause_sprite.setTexture(pause_texture);
    pause_sprite.setPosition(Vector2f(0, 0));
    pause_sprite.setScale(Vector2f(0.5, 0.5));


    string last_button = "";


    bool menu_open = true;
    bool map_created = false;
    bool upgrade_menu = false;
    bool game_over = false;
    bool guide_is = false;
    float speed_up = 1;

    UI::Upgrade_menu oba;


    while (window.isOpen()) {
        Event event;

        while (window.pollEvent(event)) {}


        if (event.type == Event::Closed)
            window.close();

        if (exitB.is_clicked(window)) {
            window.close();
        }


        if (menu_open) {
            if (map_created) {
                walls = {};
                all_objects = {};
                seeck = {};
                objects_with_collision = {};
                floor = {};
                book = {};
                map_created = false;
                score = 0;
                level = 0;
                speed_lv = 1;
                heal_lv = 0;
                angry = 0;
                pause = false;
                guide_is = false;
            }
            menu_open = UI::menu(window, &guide_is);
            continue;
        }

        if (!map_created) {

            if (guide_is) {
                create_map(guide, &walls, &all_objects, &seeck, &objects_with_collision, &player, &floor, &book,
                           guidejs, window);
            } else {
                create_map(map, &walls, &all_objects, &seeck, &objects_with_collision, &player, &floor, &book, mapjs,
                           window);
                seeck.all_seeck[1].time_have = 100;
            }
            seeck.all_seeck[0].status = status_for_seeck::need_help;
            seeck.all_seeck[0].time_have = 40;
            map_created = true;
        }


        time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        window.clear(Color(32, 32, 32));
        time_in_game += time / 1000000;
        if (pause || game_over) time = 0;

        if (time_in_game > 1 && !pause && !game_over) {
            seeck.update(time_in_game, level, &score, &angry);
            time_in_game = 0;
        }

        if (nearest_sick(seeck, player.inv_player).status == status_for_seeck::helping) time_speed_up = 0;
        else
            time_speed_up = time / 1000;

        camera(window, player);
        floor.draw_objects(window, window.getView().getCenter());

        switch (speed_lv) {
            case 0:
                speed_up = 1;
                break;
            case 1:
                speed_up = 1.2;
                break;

            case 2:
                speed_up = 1.5;
                break;

            case 3:
                speed_up = 2;
                break;
        }

        player.update(window, time_speed_up, speed_up * healing_status, &objects_with_collision);
        walls.draw_objects(window, window.getView().getCenter());
        all_objects.draw_objects(window, window.getView().getCenter());
        seeck.draw_Seeck(window);
        book.draw_objects(window, window.getView().getCenter());
        exitB.draw(window, window.getView().getCenter().x + 640, window.getView().getCenter().y - 410);
        top_bar.draw(window, score, speed_lv, heal_lv, angry);
        UI::draw_lamps(window, seeck, level);
        if (guide_is) {
            if (seeck.all_seeck[0].status == status_for_seeck::need_help)
                seeck.all_seeck[0].time_have = 25;
        }


        if (distance(player.inv_player, nearest_sick(seeck, player.inv_player).sprite) < 140) {
            if (nearest_sick(seeck, player.inv_player).status == status_for_seeck::helping) {
                healing_status = 0;
                heal.set_tex("sprites/healing.png");
                heal.draw(window, 20 + window.getView().getCenter().x, 20 + window.getView().getCenter().y);

            } else if (nearest_sick(seeck, player.inv_player).status == status_for_seeck::need_help) {
                healing_status = 1;
                heal.set_tex("sprites/E.png");
                heal.draw(window, 20 + window.getView().getCenter().x, 20 + window.getView().getCenter().y);
            } else {
                healing_status = 1;

            }
        }

        if (distance(book.Objects[0], player.inv_player) < 140) {
            heal.set_tex("sprites/E.png");
            heal.draw(window, 20 + window.getView().getCenter().x, 20 + window.getView().getCenter().y);
        }


        if (Keyboard::isKeyPressed(Keyboard::E) && last_button != "E") {
            int mh = may_help(seeck, player);
            if (mh != -1) {
                seeck.all_seeck[mh].status = status_for_seeck::helping;
                switch (heal_lv) {
                    case 0:
                        seeck.all_seeck[mh].time_have = 25;
                        break;
                    case 1:
                        seeck.all_seeck[mh].time_have = 20;
                        break;
                    case 2:
                        seeck.all_seeck[mh].time_have = 15;
                        break;
                    case 3:
                        seeck.all_seeck[mh].time_have = 10;
                        break;
                }
            }

            if (distance(book.Objects[0], player.inv_player) < 140) {
                if (upgrade_menu) {
                    pause = false;
                    upgrade_menu = false;
                } else {
                    pause = true;
                    upgrade_menu = true;
                }

            }
            last_button = "E";
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape) && last_button != "escape") {
            if (pause) {
                pause = false;
            } else {
                pause = true;
            }
            if (upgrade_menu) {
                upgrade_menu = false;
            }
            last_button = "escape";
        }

        if (!Keyboard::isKeyPressed(Keyboard::Escape) && !Keyboard::isKeyPressed(Keyboard::E)) last_button = "";

        if (pause && !game_over && !upgrade_menu) {
            pause_sprite.setPosition(window.getView().getCenter() - Vector2f(250, 100));
            to_menu.set_pos(window.getView().getCenter() + Vector2f(-100, 200));
            to_menu.draw(window);
            if (to_menu.is_clicked(window)) {
                menu_open = true;
            }
            window.draw(pause_sprite);
        }

        if (game_over)
            UI::game_over_sreen(window, &score, &speed_lv, &heal_lv, &angry, &level, &menu_open, &game_over,
                                &player.inv_player, p_start_position);

        if (upgrade_menu) {
            oba.update(window, &score, &speed_lv, &heal_lv, &angry, &level);
            pause = true;
        }

        if (angry >= 3 || score < -100) {
            game_over = true;
        }

        window.setActive();
        window.display();
    }
}
