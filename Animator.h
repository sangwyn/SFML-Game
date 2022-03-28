#ifndef GAME__ANIMATOR_H_
#define GAME__ANIMATOR_H_

#include <vector>
#include <SFML/Graphics.hpp>

class Animator {
 private:
  sf::Vector2u image_count;
  sf::Vector2u current_image;

  float total_time;
  float switch_time;
 public:
  sf::IntRect uv_rect;
  Animator(sf::Texture& texture, sf::Vector2u image_count, float switch_time);
  ~Animator();

  void Update(int row, float delta_time);
};

#endif //GAME__ANIMATOR_H_
