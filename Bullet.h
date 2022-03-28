#ifndef GAME__BULLET_H_
#define GAME__BULLET_H_

#include <SFML/Graphics.hpp>

class Bullet {
 private:
  sf::Sprite sprite;
  sf::Vector2f velocity;
  sf::Clock lifetime;
  float max_lifetime;
  bool is_enemy;
 public:
  Bullet();
  Bullet(sf::Vector2f position,
         float rotation,
         sf::Vector2f velocity,
         bool is_enemy,
         const sf::Texture &texture);
  void Draw(sf::RenderWindow &window);
  bool CollideWithWalls(const std::vector<sf::Sprite> &walls);
  bool Update(const std::vector<sf::Sprite> &walls);
};

#endif //GAME__BULLET_H_
