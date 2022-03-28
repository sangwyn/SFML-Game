#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "Player.h"
#include "Animator.h"
#include "MainWindowFunctions.h"
#include "Enemy.h"
#include "Constants.h"
#include "Node.h"

int main() {
    sf::RenderWindow
        window(sf::VideoMode(1600, 900),
               "Game", sf::Style::Close);
    sf::Image icon;
    icon.loadFromFile("Assets/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    sf::View view(sf::Vector2f(0.0f, 0.0f),
                  sf::Vector2f(window.getSize().x, window.getSize().y));

    sf::Font font;
    font.loadFromFile("Assets/munro.ttf");
    sf::Clock gameover_timer;
    int respawn_time = 3;

    Player player(6);

    std::vector<sf::Sprite> level;

    std::vector<sf::Texture> floor_tiles(8);
    for (int i = 0; i < floor_tiles.size(); ++i) {
        floor_tiles[i].loadFromFile(
            "Assets/tile_" + std::to_string(i) + ".png");
    }
    std::vector<std::string>
        tile_names = {"wall_l", "wall_r", "wall_up", "wall",
                      "door_ul", "door_ur", "door_dl", "door_dr", "door_up_l",
                      "door_up_r",
                      "wall_up_corner_ur", "wall_up_corner_ul",
                      "wall_up_corner_dr", "wall_up_corner_dl",
                      "wall_corner_ul", "wall_corner_ur", "wall_up_door_l",
                      "wall_up_door_r"};
    std::map<std::string, sf::Texture> tiles;
    for (auto name: tile_names) {
        tiles[name].loadFromFile("Assets/" + name + ".png");
    }
    std::vector<sf::Sprite> walls;
    CreateLevel(level, walls,
                floor_tiles, tiles);

    Node<Enemy> *enemies = new Node<Enemy>;
//    enemies->data = Enemy(3, 1, {500.0f, 500.0f});

//    int frames = 0;
//    sf::Clock framerate;

    float delta_time;
    sf::Clock delta;

    sf::Vector2f spawn_position = {1120.0f, 580.0f};

    sf::Clock imp_spawner;
    float imp_cooldown = 1.0f;

    sf::Clock demon_spawner;
    float demon_cooldown = 3.0f;

    sf::Clock devil_spawner;
    float devil_cooldown = 10.0f;

    while (window.isOpen()) {
//        if (framerate.getElapsedTime().asSeconds() >= 1.0f) {
//            std::cout << frames << '\n';
//            frames = 0;
//            framerate.restart();
//        }
        delta_time = delta.getElapsedTime().asSeconds();
        delta.restart();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::D) {
                    player.SetHorVelocity(kSpeed);
                } else if (event.key.code == sf::Keyboard::A) {
                    player.SetHorVelocity(-kSpeed);
                } else if (event.key.code == sf::Keyboard::W) {
                    player.SetVertVelocity(-kSpeed);
                } else if (event.key.code == sf::Keyboard::S) {
                    player.SetVertVelocity(kSpeed);
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::D
                    || event.key.code == sf::Keyboard::A) {
                    player.SetHorVelocity(0);
                } else if (event.key.code == sf::Keyboard::W
                    || event.key.code == sf::Keyboard::S) {
                    player.SetVertVelocity(0);
                }
            }
        }

        window.clear(kBackgroundColor);
        if (player.IsDead()) {
            GameOverScreen(window, player.GetScore(), player.GetHighScore(), font, respawn_time, gameover_timer);
            if (respawn_time < (int)gameover_timer.getElapsedTime().asSeconds()) {
                Restart(player, enemies);
            }
            window.display();
            continue;
        }
        DrawLevel(window, level);

        if (imp_spawner.getElapsedTime().asSeconds() > imp_cooldown) {
            Node<Enemy> *imp = new Node<Enemy>;
            imp->data = Enemy(1, spawn_position);
            if (enemies == nullptr)
                enemies = imp;
            else
                enemies->InsAfter(imp);
            imp_spawner.restart();
        }
        if (demon_spawner.getElapsedTime().asSeconds() > demon_cooldown) {
            Node<Enemy> *demon = new Node<Enemy>;
            demon->data = Enemy(2, spawn_position);
            if (enemies == nullptr)
                enemies = demon;
            else
                enemies->InsAfter(demon);
            demon_spawner.restart();
        }
        if (devil_spawner.getElapsedTime().asSeconds() > devil_cooldown) {
            Node<Enemy> *devil = new Node<Enemy>;
            devil->data = Enemy(3, spawn_position);
            if (enemies == nullptr)
                enemies = devil;
            else
                enemies->InsAfter(devil);
            devil_spawner.restart();
        }
        Node<Enemy> *enemy_i = enemies;
        while (enemy_i != nullptr) {
            if (enemy_i->data.Update(delta_time,
                                 walls,
                                 player.GetPosition(),
                                 player.GetCollider(),
                                 player.GetWeaponCollider())) {
                player.ReceiveDamage(enemy_i->data.GetDamage());
                if (player.IsDead()) {
                    gameover_timer.restart();
                    break;
                }
            }
            enemy_i->data.Draw(window);
            enemy_i = enemy_i->next;
        }
        if (player.IsDead())
            continue;
        player.Update(delta_time,
                      walls,
                      enemies,
                      window.mapPixelToCoords(sf::Mouse::getPosition(window)));

        view.setCenter(player.GetPosition());
        window.setView(view);

        player.Draw(window, font);
        window.display();
//        ++frames;
    }
    if (player.GetScore() >= player.GetHighScore()) {
        SaveHighscore(player.GetScore());
    }

    return 0;
}
