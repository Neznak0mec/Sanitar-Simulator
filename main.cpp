#include <SFML/Graphics.hpp>
#include <iostream>


using namespace std;
using namespace sf;

int score = 0;
int level = 0;

bool collision_wall(int player_x, int player_y, Sprite &obj);


class Object{
public:

    vector<Sprite> Objects;
    Texture aba;

    void create_object(string texture_path, Vector2f pos, int height = 0, int width = 0)
    {
        if(!aba.loadFromFile(texture_path))
        {
            printf("Error load texture");
            return;
        }
        aba.setSmooth(true);
        aba.setRepeated(false);
        object.setTexture(aba);
        if (height != 0 && width != 0)
            object.setScale({1,1});
        object.setPosition(pos.x,pos.y);

        Objects.push_back(object);
    }


    void draw_objects(RenderWindow &window)
    {
        for (int i = 0; i < Objects.size(); i++)
        {
            window.draw(Objects[i]);
        }
    }


private:
    Sprite object;

};


class Player
{
public:
    Sprite player;
    Texture player_texture;
    Player(float x, float y, Texture texture)
    {
        player_texture = texture;
        player.setTexture(player_texture);
        player.setPosition(x,y);
        player.setScale({0.5,0.5});
    }


    void update(RenderWindow &window,float time_speed_up, float speed_up, vector<Object> *objects){
        if (Keyboard::isKeyPressed((Keyboard::W)) && Keyboard::isKeyPressed((Keyboard::S)))
            moveX = 0;

        else if(Keyboard::isKeyPressed((Keyboard::W)))
            moveX = -0.1;


        else if(Keyboard::isKeyPressed((Keyboard::S)))
            moveX = 0.1;

        else
            moveX = 0;

        if (Keyboard::isKeyPressed((Keyboard::A)) && Keyboard::isKeyPressed((Keyboard::D)))
            moveY = 0;

        else if(Keyboard::isKeyPressed((Keyboard::A)))
            moveY = -0.1;

        else if(Keyboard::isKeyPressed((Keyboard::D)))
            moveY = 0.1;

        else
            moveY = 0;


        float speed= 0.065555;

        if (Keyboard::isKeyPressed((Keyboard::W)) && Keyboard::isKeyPressed((Keyboard::D)))
        {
            moveX = -speed;
            moveY = speed;
        }

        else if(Keyboard::isKeyPressed((Keyboard::W)) && Keyboard::isKeyPressed((Keyboard::A))){
            moveX = -speed;
            moveY = -speed;
        }

        else if(Keyboard::isKeyPressed((Keyboard::S)) && Keyboard::isKeyPressed((Keyboard::D))){
            moveX = speed;
            moveY = speed;
        }

        else if(Keyboard::isKeyPressed((Keyboard::S)) && Keyboard::isKeyPressed((Keyboard::A))){
            moveX = speed;
            moveY = -speed;
        }


        staminas(time_speed_up);

        //collision with wall
        int player_x_ = player.getPosition().x;
        int player_y_ = player.getPosition().y;

        int player_x = player.getPosition().x+moveY*time_speed_up*speed_up*stamina_speed_up;
        int player_y = player.getPosition().y+moveX*time_speed_up*speed_up*stamina_speed_up;



        for (int j = 0; j < objects->size(); j++){
            for (int i = 0; i < objects->at(j).Objects.size(); i++)
            {
                if (collision_wall(player_x_, player_y, objects->at(j).Objects[i]))
                {
                    moveX = 0;
                }
                if (collision_wall(player_x, player_y_, objects->at(j).Objects[i]))
                {
                    moveY = 0;
                }
        }
        }

        player.move(moveY*time_speed_up*speed_up*stamina_speed_up,moveX*time_speed_up*speed_up*stamina_speed_up);

        window.draw(player);
    }

    void staminas(float time){
        if (stamina < 0)
            stamina = 0;
        if (Keyboard::isKeyPressed((Keyboard::LShift)))
        {
            wait_time_to_recovery_stamina = 0;
            if (stamina > 0)
            {
                stamina -= 0.30;
                stamina_speed_up = 4;
            }
            else
            {
                stamina_speed_up = 1;
            }
        }
        else
        {
            if(wait_time_to_recovery_stamina <= time_to_recovery_stamina)
                wait_time_to_recovery_stamina += time/1000;
            if (stamina < 7)
            {
                if (wait_time_to_recovery_stamina >= time_to_recovery_stamina)
                {
                    stamina += 0.30*time/1000;
                }
                stamina_speed_up = 1;
            }
            else
            {
                stamina_speed_up = 1;
            }
        }
    }

    void draw_stamina(RenderWindow &window) {
        RectangleShape stamina_bar;
        stamina_bar.setSize(Vector2f(5, stamina*10));
        stamina_bar.setPosition(player.getPosition().x - 290, player.getPosition().y );
        Color color(166, 0, 144);
        stamina_bar.setFillColor(color);
        window.draw(stamina_bar);
    }

private:
    float moveY;
    float moveX;
    float stamina = 7;
    float stamina_speed_up= 1;
    float time_to_recovery_stamina = 3;
    float wait_time_to_recovery_stamina = 0;
};



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



bool collision_wall(int player_x, int player_y, Sprite &obj)
{
    int wall_x = obj.getPosition().x;
    int wall_y = obj.getPosition().y;
    if(player_x + 32 >= wall_x && player_x <= wall_x + obj.getTexture()->getSize().x && player_y + 62 >= wall_y && player_y <= wall_y + obj.getTexture()->getSize().y)
        return true;
    else
        return false;
}

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



void camera(RenderWindow &window, Player &player) {
    View view;
    view.setSize(Vector2f(600, 400));
    view.setCenter(player.player.getPosition().x, player.player.getPosition().y);
    window.setView(view);
}


int main()
{
    float time;
    RenderWindow window(VideoMode(600, 400), "Hyila ha-ha");



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
        camera(window, player);
        player.draw_stamina(window);
        all_objects.draw_objects(window);
        walls.draw_objects(window);
        window.display();
    }
}