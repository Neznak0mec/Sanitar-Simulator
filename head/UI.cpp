
#include "UI.h"

UI::rec_Button::rec_Button(Vector2<float> size, Color color, Vector2<float> pos, string text) {
    rect.setSize(size);
    rect.setFillColor(color);
    rect.setPosition(pos.x, pos.y);
    text_button = text;
}

void UI::rec_Button::draw(RenderWindow &window) {
    window.draw(rect);
    sf::Text text;
    text.setString(text_button);
    Font font;
    font.loadFromFile("fonts/Agit_Prop.ttf");
    text.setFont(font);
    text.setPosition(rect.getPosition().x + 20, rect.getPosition().y + 18);
    window.draw(text);
}

bool UI::rec_Button::is_clicked(RenderWindow &window) {
    Vector2<int> pos = Mouse::getPosition(window);

    float but_x = rect.getPosition().x - window.getView().getCenter().x + window.getView().getSize().x / 2;
    float but_y = rect.getPosition().y - window.getView().getCenter().y + window.getView().getSize().y / 2;

    if (pos.x > but_x && pos.x < but_x + rect.getSize().x && pos.y > but_y && pos.y < but_y + rect.getSize().y) {
        if (Mouse::isButtonPressed(Mouse::Left)) {
            return true;
        }
    }
    return false;
}

void UI::rec_Button::set_pos(Vector2f pos) {
    rect.setPosition(pos.x, pos.y);
}


UI::Button::Button(Vector2<float> size, string tex_path) {
    shape.setSize(size);
    tex.loadFromFile(tex_path);
    shape.setTexture(&tex);
}

void UI::Button::draw(RenderWindow &window, float pos1, float pos2) {
    shape.setPosition(pos1, pos2);
    window.draw(shape);
}


void UI::Button::set_tex(string tex_path) {
    tex.loadFromFile(tex_path);
    shape.setTexture(&tex);
}

bool UI::Button::is_clicked(RenderWindow &window) {
    Vector2<int> pos = Mouse::getPosition(window);

    float but_x = shape.getPosition().x - window.getView().getCenter().x + window.getView().getSize().x / 2;
    float but_y = shape.getPosition().y - window.getView().getCenter().y + window.getView().getSize().y / 2;

    if (pos.x > but_x && pos.x < but_x + shape.getSize().x && pos.y > but_y && pos.y < but_y + shape.getSize().y) {
        if (Mouse::isButtonPressed(Mouse::Left)) {
            return true;
        }
    }
    return false;
}


UI::Top_Bar::Top_Bar(Vector2<float> size, Vector2<float> pos, string tex_path) {
    shape.setSize(size);
    bar_tex.loadFromFile(tex_path);
    shape.setTexture(&bar_tex);
    shape.setPosition(pos);
}

void UI::Top_Bar::draw(RenderWindow &window, int sc, int sl, int hl, int ang) {
    float x = window.getView().getCenter().x;
    float y = window.getView().getCenter().y;

    shape.setPosition(x - 550, y - 420);
    Font font;
    font.loadFromFile("fonts/Agit_Prop.ttf");

    Text text;
    text.setFont(font);
    text.setString(to_string(sc));
    text.setCharacterSize(50);
    text.setPosition(x - 250, y - 410);
    text.setFillColor(Color::Black);

    window.draw(shape);
    window.draw(text);
    text.setString(to_string(sl - 1));
    text.setPosition(x + 40, y - 410);
    window.draw(text);

    text.setString(to_string(hl));
    text.setPosition(x + 220, y - 410);
    window.draw(text);

    text.setString(to_string(ang));
    text.setPosition(x + 400, y - 410);
    window.draw(text);
}


void UI::draw_lamps(RenderWindow &window, Sick &sick, int lvl) {
    float x = window.getView().getCenter().x;
    float y = window.getView().getCenter().y;
    y = y + 380;

    double center = x;

    vector<Sick::sick_info> objects;

    for (int i = 0; i < sick.all_seeck.size(); ++i)
        if (sick.all_seeck[i].open_lvl <= lvl)
            objects.push_back(sick.all_seeck[i]);
    Text text;
    Font font;
    font.loadFromFile("fonts/Agit_Prop.ttf");
    text.setFont(font);
    text.setCharacterSize(30);
    text.setFillColor(Color::Black);

    CircleShape circle(60);
    double startPosition = center - (objects.size() * 60 + (objects.size() - 1) * 5);

    for (int i = 0; i < objects.size(); ++i) {

        if (objects[i].status == status_for_seeck::need_help) {
            if (objects[i].time_have < 10)
                circle.setFillColor(Color::Red);
            else if (objects[i].time_have < 20)
                circle.setFillColor(Color::Yellow);
            else
                circle.setFillColor(Color::Green);
        } else
            circle.setFillColor(Color::White);

        text.setString(to_string(i + 1));
        circle.setPosition(startPosition + i * 120, y);
        text.setPosition(startPosition + i * 120 + 50, y);
        window.draw(circle);
        window.draw(text);
    }

};


bool UI::menu(RenderWindow &window, bool *guide_is) {
    Sprite background;
    Texture texture;

    texture.loadFromFile("sprites/menu_back_ground.png");
    texture.setSmooth(true);
    background.setTexture(texture);
    background.setPosition(window.getView().getCenter().x - window.getView().getSize().x / 2,
                           window.getView().getCenter().y - window.getView().getSize().y / 2);
    background.setScale(window.getSize().x / background.getLocalBounds().width,
                        window.getSize().y / background.getLocalBounds().height);

    Vector2<float> pos0 = Vector2<float>(window.getView().getCenter().x - window.getView().getSize().x / 2,
                                         window.getView().getCenter().y - window.getView().getSize().y / 2);

    window.clear();
    window.draw(background);
    auto start = rec_Button(Vector2<float>(150, 75), Color::Black, Vector2<float>(pos0.x + 100, pos0.y + 400), "Start");

    start.draw(window);

    auto guide = rec_Button(Vector2<float>(150, 75), Color::Black, Vector2<float>(pos0.x + 100, pos0.y + 500), "Guide");

    guide.draw(window);

    if (start.is_clicked(window)) {
        return false;
    }

    if (guide.is_clicked(window)) {
        *guide_is = true;
        return false;
    }

    window.display();
    return true;
}

void UI::Upgrade_menu::update(RenderWindow &window, int *sc, int *sl, int *hl, int *ang, int *lvl) {


    Sprite background;
    Texture texture;
    texture.loadFromFile("sprites/Objects/clear_book.png");
    texture.setSmooth(true);

    background.setTexture(texture);
    background.setScale(4, 4);

    Button speed_upgrade(Vector2<float>(300, 150), "sprites/buttons/yskorin_ru.png");
    Button heal_upgrade(Vector2<float>(300, 150), "sprites/buttons/yzbogoin_ru.png");
    Button lvl_upgrade(Vector2<float>(300, 150), "sprites/buttons/lv_1_ru.png");
    Button ang_downgrade(Vector2<float>(300, 150), "sprites/buttons/angry_1_ru.png");

    int speed_cost;
    int heal_cost;
    int lvl_cost;
    int help_cost;

    switch (*sl) {
        case 1:
            speed_upgrade.set_tex("sprites/buttons/yskorin_ru.png");
            speed_cost = 450;
            break;

        case 2:
            speed_upgrade.set_tex("sprites/buttons/yskorin+_ru.png");
            speed_cost = 600;
            break;

        case 3:
            speed_upgrade.set_tex("sprites/buttons/yskorin++_ru.png");
            speed_cost = 900;
            break;

        case 4:
            speed_upgrade.set_tex("sprites/buttons/yskorin-_ru.png");
            speed_cost = INT32_MAX;
            break;
    }

    switch (*hl) {
        case 0:
            heal_upgrade.set_tex("sprites/buttons/yzbogoin_ru.png");
            heal_cost = 300;
            break;

        case 1:
            heal_upgrade.set_tex("sprites/buttons/yzbogoin+_ru.png");
            heal_cost = 700;
            break;

        case 2:
            heal_upgrade.set_tex("sprites/buttons/yzbogoin++_ru.png");
            heal_cost = 1500;
            break;

        case 3:
            heal_upgrade.set_tex("sprites/buttons/yzbogoin-_ru.png");
            heal_cost = INT32_MAX;
            break;

    }

    switch (*lvl) {
        case 0:
            lvl_upgrade.set_tex("sprites/buttons/lv_1_ru.png");
            lvl_cost = 1000;
            break;

        case 1:
            lvl_upgrade.set_tex("sprites/buttons/lv_2_ru.png");
            lvl_cost = 2000;
            break;

        case 2:
            lvl_upgrade.set_tex("sprites/buttons/lv_3_ru.png");
            lvl_cost = INT32_MAX;
            break;
    }


    switch (help) {
        case 0:
            ang_downgrade.set_tex("sprites/buttons/angry_1_ru.png");
            help_cost = 1000;
            break;

        case 1:
            ang_downgrade.set_tex("sprites/buttons/angry_2_ru.png");
            help_cost = 2000;
            break;

        case 2:
            ang_downgrade.set_tex("sprites/buttons/angry_3_ru.png");
            help_cost = INT32_MAX;
            break;

    }

    float start_x = window.getView().getCenter().x - window.getView().getSize().x / 2;
    float start_y = window.getView().getCenter().y - window.getView().getSize().y / 2;

    background.setPosition({start_x + 220, start_y + 100});
    window.draw(background);

    speed_upgrade.draw(window, start_x + 300, start_y + 200);
    heal_upgrade.draw(window, start_x + 800, start_y + 200);
    lvl_upgrade.draw(window, start_x + 300, start_y + 425);
    ang_downgrade.draw(window, start_x + 800, start_y + 425);

    if (Mouse::isButtonPressed(Mouse::Left)) {
        if (!is_clicked) {
            if (speed_upgrade.is_clicked(window) && *sc >= speed_cost) {
                *sc -= speed_cost;
                *sl += 1;
            }

            if (heal_upgrade.is_clicked(window) && *sc >= heal_cost) {
                *sc -= heal_cost;
                *hl += 1;
            }

            if (lvl_upgrade.is_clicked(window) && *sc >= lvl_cost) {
                *sc -= lvl_cost;
                *lvl += 1;
            }

            if (ang_downgrade.is_clicked(window) && *sc >= help_cost && *ang > 1) {
                *sc -= help_cost;
                help++;
                *ang -= 1;
            }

        }
        is_clicked = true;
    } else
        is_clicked = false;


};


void UI::game_over_sreen(RenderWindow &window, int *sc, int *sl, int *hl, int *ang, int *lvl, bool *menu_open,
                         bool *game_over, Sprite *p, Vector2<float> s_pos) {

    Sprite sprit;
    Texture tex;
    rec_Button menu = rec_Button({150, 75}, Color::Black, {0, 0}, "Menu");
    rec_Button restart = rec_Button({200, 75}, Color::Black, {0, 0}, "Restart");

    tex.loadFromFile("sprites/GAME_lOSE.png");
    tex.setSmooth(true);

    sprit.setTexture(tex);
    sprit.setPosition(window.getView().getCenter().x - window.getView().getSize().x / 2,
                      window.getView().getCenter().y - window.getView().getSize().y / 2);

    sprit.setScale(window.getSize().x / sprit.getLocalBounds().width,
                   window.getSize().y / sprit.getLocalBounds().height);

    Vector2<float> pos = {window.getView().getCenter().x - 400, window.getView().getCenter().y + 250};
    Vector2<float> pos2 = {window.getView().getCenter().x + 257, window.getView().getCenter().y + 250};


    menu.rect.setPosition(pos);
    restart.rect.setPosition(pos2);

    if (menu.is_clicked(window)) {
        *menu_open = true;
        *sc = 0;
        *sl = 1;
        *hl = 0;
        *ang = 0;
        *lvl = 0;
        *game_over = false;
        p->setPosition(s_pos);
    }

    if (restart.is_clicked(window)) {
        *sc = 0;
        *sl = 1;
        *hl = 0;
        *ang = 0;
        *lvl = 0;
        *game_over = false;
        p->setPosition(s_pos);
    }


    window.draw(sprit);
    restart.draw(window);
    menu.draw(window);

}

