#ifndef GAME__MAINWINDOWFUNCTIONS_H_
#define GAME__MAINWINDOWFUNCTIONS_H_

#include <vector>
#include "Player.h"
#include "Enemy.h"

void CreateLevel(std::vector<sf::Sprite> &level,
                 std::vector<sf::Sprite> &walls,
                 const std::vector<sf::Texture> &floor_tiles,
                 std::map<std::string, sf::Texture> &tiles);

void DrawLevel(sf::RenderWindow &window,
               const std::vector<sf::Sprite> &level);

void SaveHighscore(int highscore);

void Restart(Player &player, Node<Enemy> *&enemies);

void GameOverScreen(sf::RenderWindow &window,
                    int score,
                    int highscore,
                    sf::Font font,
                    int respawn_time,
                    sf::Clock timer);

void MainMenu(sf::RenderWindow &window,
              sf::Font font);

#endif //GAME__MAINWINDOWFUNCTIONS_H_
