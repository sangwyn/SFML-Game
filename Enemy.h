#ifndef GAME__ENEMY_H_
#define GAME__ENEMY_H_

#include <SFML/Graphics.hpp>
#include "Node.h"
#include "Bullet.h"
#include "Animator.h"

class Enemy {
 private:
  sf::Texture *texture_heart_full, *texture_heart_half, *texture_heart_empty;
  sf::Sprite sprite_heart;


  sf::Texture *texture_idle, *texture_run;
  Animator *anim_current, *anim_idle, *anim_run;

  int id;
  sf::Sprite sprite;
  int max_hp, hp;
  int dmg;
  float speed;
  sf::Texture *bullet_texture;
  Node<Bullet> *bullets = nullptr;

  sf::Vector2f velocity;

  sf::Clock shooting;
  float shooting_cooldown = 0.8f;

  sf::Clock invincibility;
  float invincible_time = 0.0f;
  bool is_dead = false;
 public:
  Enemy(int id = 1,
        sf::Vector2f position = {0.0f, 0.0f});

  void ReceiveDamage(int value);

  void SetHorVelocity(float _velocity);
  void SetVertVelocity(float _velocity);

  sf::Vector2f GetPosition();
  sf::FloatRect GetCollider();
  int GetID();
  bool IsDead();
  int GetDamage();

  void Shoot();

  bool CollideWithWalls(const std::vector<sf::Sprite> &walls);
  bool Update(float delta_time,
              const std::vector<sf::Sprite> &level,
              sf::Vector2f player_pos,
              sf::FloatRect player_collider,
              sf::FloatRect player_weapon_collider);
  void Draw(sf::RenderWindow &window);
};

#endif //GAME__ENEMY_H_
