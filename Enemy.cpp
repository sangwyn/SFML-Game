#include "Enemy.h"
#include "Constants.h"
#include "MathFunctions.h"
#include <algorithm>
#include <iostream>
#include <cmath>

Enemy::Enemy(int id,
             sf::Vector2f position) {
    this->id = id;
    texture_heart_full = new sf::Texture;
    texture_heart_full->loadFromFile("Assets/heart_full.png");

    texture_heart_half = new sf::Texture;
    texture_heart_half->loadFromFile("Assets/heart_half.png");

    texture_heart_empty = new sf::Texture;
    texture_heart_empty->loadFromFile("Assets/heart_empty.png");
    sprite_heart.setTexture(*texture_heart_empty);
    sprite_heart.setScale(kScale * 0.3f, kScale * 0.3f);

    texture_run = new sf::Texture;
    texture_idle = new sf::Texture;
    if (id == 1) {
        dmg = 1;
        max_hp = hp = 1;
        speed = kSpeed * 0.6f;
        texture_idle->loadFromFile("Assets/imp_idle.png");
        texture_run->loadFromFile("Assets/imp_run.png");
    } else if (id == 2) {
        dmg = 1;
        max_hp = hp = 3;
        speed = kSpeed * 0.4f;
        texture_idle->loadFromFile("Assets/demon_idle.png");
        texture_run->loadFromFile("Assets/demon_run.png");
    } else if (id == 3) {
        dmg = 3;
        max_hp = hp = 10;
        speed = kSpeed * 0.4f;
        texture_idle->loadFromFile("Assets/devil_idle.png");
        texture_run->loadFromFile("Assets/devil_run.png");
    }
    anim_idle = new Animator(*texture_idle, {4, 1}, 0.1f);
    anim_run = new Animator(*texture_run, {4, 1}, 0.1f);
    position.y += sprite.getGlobalBounds().height;

    anim_current = anim_idle;
    sprite.setTexture(*texture_idle);
    sprite.setScale(kScale, kScale);
    sprite.setOrigin(sprite.getTextureRect().width / 2.0f,
                     sprite.getTextureRect().height / 2.0f);
    sprite.setPosition(position);
    shooting.restart();
}

void Enemy::ReceiveDamage(int value) {
    if (invincibility.getElapsedTime().asSeconds() < invincible_time)
        return;
    hp -= value;
    if (hp <= 0) {
        is_dead = true;
    }
    invincibility.restart();
    invincible_time = 0.5f;
}

bool Enemy::CollideWithWalls(const std::vector<sf::Sprite> &walls) {
    for (int i = 0; i < walls.size(); ++i) {
        if (sprite.getGlobalBounds().intersects(walls[i].getGlobalBounds())) {
            return true;
        }
    }
    return false;
}

bool Enemy::Update(float delta_time,
                   const std::vector<sf::Sprite> &walls,
                   sf::Vector2f player_pos,
                   sf::FloatRect player_collider,
                   sf::FloatRect player_weapon_collider) {
    bool damaged_player = false;
    //if (enemy_update.getElapsedTime().asSeconds() > update_time) {
    velocity = player_pos - GetPosition();
    if (velocity.x != 0)
        velocity.x /= fabs(velocity.x);
    if (velocity.y != 0)
        velocity.y /= fabs(velocity.y);
    velocity *= speed;
    bool pushed = false;
    sprite.setPosition(sprite.getPosition() + velocity * delta_time);
    if (GetCollider().intersects(player_collider)) {
        damaged_player = true;
        sf::Vector2f pushing_vector =
            {(velocity.x * GetCollider().width) / (float) fabs(velocity.x),
             (velocity.y * GetCollider().height) / (float) fabs(velocity.y)};
        sprite.setPosition(
            sprite.getPosition() - 2.0f * pushing_vector * delta_time);
        velocity.x *= -1;
        velocity.y *= -1;
        pushed = true;
        sprite.setPosition(sprite.getPosition() + velocity * delta_time);
    } else if (GetCollider().intersects(player_weapon_collider)) {
        ReceiveDamage(1);
        sf::Vector2f pushing_vector =
            {(velocity.x * GetCollider().width) / (float) fabs(velocity.x),
             (velocity.y * GetCollider().height) / (float) fabs(velocity.y)};
        sprite.setPosition(
            sprite.getPosition() - 2.0f * pushing_vector * delta_time);
        velocity.x *= -1;
        velocity.y *= -1;
        pushed = true;
        sprite.setPosition(sprite.getPosition() + velocity * delta_time);
    }

    if (CollideWithWalls(walls)) {
        sprite.setPosition(sprite.getPosition() - velocity * delta_time);
    }
    if (velocity.x != 0 || velocity.y != 0) {
        anim_current = anim_run;
        sprite.setTexture(*texture_run);
        if (velocity.x < 0 && !pushed)
            sprite.setScale(-kScale, kScale);
        else if (!pushed)
            sprite.setScale(kScale, kScale);
    } else {
        anim_current = anim_idle;
        sprite.setTexture(*texture_idle);
    }
    anim_current->Update(0, delta_time);
    sprite.setTextureRect(anim_current->uv_rect);
    sprite.setOrigin(sprite.getTextureRect().width / 2.0f,
                     sprite.getTextureRect().height / 2.0f);

    if (bullets != nullptr) {
        Node<Bullet> *p = bullets, *prev = nullptr;
        while (p != nullptr) {
            if (p->data.Update(walls)) {
                Node<Bullet> *tmp = p->next;
                delete p;
                p = tmp;
                if (prev == nullptr) {
                    bullets = tmp;
                } else {
                    prev->next = tmp;
                }
                continue;
            }
            prev = p;
            p = p->next;
        }
    }
//    if (shooting.getElapsedTime().asSeconds() >= shooting_cooldown) {
//        Shoot();
//        shooting.restart();
//    }

    return damaged_player;
}

void Enemy::Draw(sf::RenderWindow &window) {
//    auto debug = sprite.getGlobalBounds();
//    sf::RectangleShape rect({debug.width, debug.height});
//    rect.setPosition({debug.left, debug.top});
//    rect.setFillColor(sf::Color::Green);
//    window.draw(rect);
    window.draw(sprite);
    sf::Vector2f health_pos = {GetPosition().x - GetCollider().width / 2,
                              GetPosition().y - GetCollider().height / 2 - 20.0f};
    int hp_counter = hp;
    float heart_width = sprite_heart.getGlobalBounds().width;
    for (int i = 0; i <= max_hp / 2; ++i) {
        if (hp_counter >= 2) {
            sprite_heart.setTexture(*texture_heart_full);
            hp_counter -= 2;
        } else if (hp_counter >= 1) {
            sprite_heart.setTexture(*texture_heart_half);
            --hp_counter;
        } else {
            sprite_heart.setTexture(*texture_heart_empty);
        }
        sprite_heart.setPosition(health_pos);
        window.draw(sprite_heart);
        health_pos.x += heart_width + 5.0f;
    }
//    Node<Bullet> *p = bullets;
//    while (p != nullptr) {
//        p->data.Draw(window);
//        p = p->next;
//    }
}

void Enemy::SetHorVelocity(float _velocity) {
    velocity.x = _velocity;
}

void Enemy::SetVertVelocity(float _velocity) {
    velocity.y = _velocity;
}

sf::Vector2f Enemy::GetPosition() {
    return sprite.getPosition();
}

sf::FloatRect Enemy::GetCollider() {
    return sprite.getGlobalBounds();
}

void Enemy::Shoot() {
    Bullet bullet(GetPosition(),
                  sprite.getRotation(),
                  velocity * 2.0f,
                  true,
                  *bullet_texture);
    Node<Bullet> *node = new Node<Bullet>(bullet);
    if (bullets == nullptr)
        bullets = node;
    else
        bullets->InsAfter(node);
}

int Enemy::GetID() {
    return id;
}

bool Enemy::IsDead() {
    return is_dead;
}

int Enemy::GetDamage() {
    return dmg;
}