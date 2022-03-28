#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>
#include <string>
#include <fstream>
#include "MainWindowFunctions.h"
#include "Constants.h"
#include "Player.h"
#include "Node.h"
#include "Enemy.h"

void CreateLevel(std::vector<sf::Sprite> &level,
                 std::vector<sf::Sprite> &walls,
                 const std::vector<sf::Texture> &floor_tiles,
                 std::map<std::string, sf::Texture> &tiles) {
    std::ifstream fin("level.txt");

    sf::Sprite tile;
    tile.setTexture(floor_tiles[0]);
    tile.setScale(kScale, kScale);
    float size_x = tile.getGlobalBounds().width,
        size_y = tile.getGlobalBounds().height;
    std::mt19937 gen
        (std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dis(0, floor_tiles.size() - 1);

    int i = 0;
    while (!fin.eof()) {
        std::string s;
        fin >> s;
        if (s.empty())
            break;
        for (int j = 0; j < s.size(); ++j) {
            if (s[j] == '-')
                continue;
            sf::Vector2f pos(size_x * j, size_y * i);
            tile.setPosition(pos);
            if (s[j] == '0') {
                tile.setTexture(floor_tiles[dis(gen)]);
            } else if (s[j] == '1') {
                if (j == 0)
                    tile.setTexture(tiles["wall_l"]);
                else
                    tile.setTexture(tiles["wall_r"]);
                walls.push_back(tile);
                sf::Sprite extra_tile;
                extra_tile.setTexture(floor_tiles[dis(gen)]);
                extra_tile.setScale(kScale, kScale);
                extra_tile.setPosition(pos);
                level.push_back(extra_tile);
            } else if (s[j] == '2') {
                tile.setTexture(tiles["wall"]);
                walls.push_back(tile);
            } else if (s[j] == 'z') {
                if (i == 1) {
                    if (j == 0) {
                        tile.setTexture(tiles["wall_up_corner_ul"]);
                    } else {
                        tile.setTexture(tiles["wall_up_corner_ur"]);
                    }
                } else {
                    if (j == 0) {
                        tile.setTexture(tiles["wall_up_corner_dl"]);
                        sf::Sprite extra_tile;
                        extra_tile.setTexture(floor_tiles[dis(gen)]);
                        extra_tile.setScale(kScale, kScale);
                        extra_tile.setPosition(pos);
                        level.push_back(extra_tile);
                    } else {
                        tile.setTexture(tiles["wall_up_corner_dr"]);
                        sf::Sprite extra_tile;
                        extra_tile.setTexture(floor_tiles[dis(gen)]);
                        extra_tile.setScale(kScale, kScale);
                        extra_tile.setPosition(pos);
                        level.push_back(extra_tile);
                    }
                }
                walls.push_back(tile);
            } else if (s[j] == 'c') {
                if (j == 0) {
                    tile.setTexture(tiles["wall_corner_ul"]);
                } else {
                    tile.setTexture(tiles["wall_corner_ur"]);
                }
                walls.push_back(tile);
            } else if (s[j] == 'u') {
                tile.setTexture(tiles["wall_up"]);
                walls.push_back(tile);
                if (i >= 2) {
                    sf::Sprite extra_tile;
                    extra_tile.setTexture(floor_tiles[dis(gen)]);
                    extra_tile.setScale(kScale, kScale);
                    extra_tile.setPosition(pos);
                    level.push_back(extra_tile);
                }
            } else if (s[j] == 'b') {
                if (j != s.size() - 1 && s[j + 1] == 's') {
                    tile.setTexture(tiles["wall_up_door_l"]);
                } else {
                    tile.setTexture(tiles["wall_up_door_r"]);
                }
                walls.push_back(tile);
            } else if (s[j] == 'h') {
                if (j != s.size() - 1 && s[j + 1] == s[j]) {
                    tile.setTexture(tiles["door_up_l"]);
                } else {
                    tile.setTexture(tiles["door_up_r"]);
                }
                walls.push_back(tile);
            } else if (s[j] == 's') {
                if (j != s.size() - 1 && s[j + 1] == s[j]) {
                    tile.setTexture(tiles["door_ul"]);
                } else {
                    tile.setTexture(tiles["door_ur"]);
                }
                walls.push_back(tile);
            } else if (s[j] == 'd') {
                if (j != s.size() - 1 && s[j + 1] == s[j]) {
                    tile.setTexture(tiles["door_dl"]);
                } else {
                    tile.setTexture(tiles["door_dr"]);
                }
                walls.push_back(tile);
            }
            level.push_back(tile);
        }
        ++i;
    }

    fin.close();
}

void DrawLevel(sf::RenderWindow &window,
               const std::vector<sf::Sprite> &level) {
    for (const auto &i: level)
        window.draw(i);
}

void SaveHighscore(int highscore) {
    std::ofstream fout("highscore.txt");
    fout << highscore;
    fout.close();
}

void Restart(Player &player, Node<Enemy> *&enemies) {
    if (player.GetScore() >= player.GetHighScore()) {
        SaveHighscore(player.GetScore());
    }
    player = Player(6);
    Node<Enemy> *enemy_i = enemies;
    while (enemy_i != nullptr) {
        Node<Enemy> *tmp = enemy_i->next;
        delete enemy_i;
        enemy_i = tmp;
    }
    enemies = nullptr;
}

void GameOverScreen(sf::RenderWindow &window,
                    int score,
                    int highscore,
                    sf::Font font,
                    int respawn_time,
                    sf::Clock timer) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(kTextSize);
    text.setString("YOU DIED\nYour score was: " + std::to_string(score) + " (" + std::to_string(highscore) + ")" +"\nRespawning in " + std::to_string(respawn_time
                                                                   - (int) timer.getElapsedTime().asSeconds())
                       + " seconds");
    text.setFillColor(sf::Color::White);
    text.setOrigin(text.getGlobalBounds().width / 2.0f,
                   text.getGlobalBounds().height / 2.0f);
    text.setPosition(window.getView().getCenter());
    window.draw(text);
}