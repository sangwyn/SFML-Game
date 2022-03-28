#ifndef GAME__PLAYER_H_
#define GAME__PLAYER_H_

#include <vector>
#include <SFML/Graphics.hpp>
#include "Animator.h"
#include "Weapon.h"
#include "Node.h"
#include "Enemy.h"

class Player {
 private:
  sf::Texture *texture_idle, *texture_run, *texture_weapon, *texture_heart_full, *texture_heart_half, *texture_heart_empty;
  sf::Sprite sprite, sprite_heart;
  int max_hp, hp;

  Animator *anim_current, *anim_idle, *anim_run;
  Weapon *weapon;

  sf::Vector2f velocity;

  sf::Clock invincibility;
  float invincible_time = 0.0f;

  bool is_dead = false;
  int score = 0, highscore;
 public:
  Player(int max_hp);

  void Heal(int value);

  void ReceiveDamage(int value);

  void SetHorVelocity(float _velocity);
  void SetVertVelocity(float _velocity);

  sf::Vector2f GetPosition();
  sf::FloatRect GetCollider();
  sf::FloatRect GetWeaponCollider();
  bool IsDead();
  int GetScore();
  int GetHighScore();

  bool CollideWithWalls(const std::vector<sf::Sprite> &walls);
  void Update(float delta_time, const std::vector<sf::Sprite> &walls, Node<Enemy> *&enemies, sf::Vector2f mouse_pos);
  void Draw(sf::RenderWindow &window, sf::Font font);
};

#endif
