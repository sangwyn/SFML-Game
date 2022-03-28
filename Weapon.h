#ifndef GAME_CMAKE_BUILD_DEBUG_ASSETS_WEAPON_H_
#define GAME_CMAKE_BUILD_DEBUG_ASSETS_WEAPON_H_

#include <SFML/Graphics.hpp>

class Weapon {
 private:
  sf::Sprite sprite;
 public:
  Weapon(const sf::Texture &texture);
  void SetPosition(sf::Vector2f pos);
  sf::FloatRect GetCollider();
  void Update(sf::Vector2f mouse_pos);
  void Draw(sf::RenderWindow &window);
};

#endif //GAME_CMAKE_BUILD_DEBUG_ASSETS_WEAPON_H_