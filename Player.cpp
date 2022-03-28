#include "Player.h"
#include "Constants.h"
#include "Enemy.h"
#include "MathFunctions.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>

Player::Player(int max_hp) {
    std::ifstream fin("highscore.txt");
    std::string s;
    fin >> s;
    highscore = StrToInt(s);
    fin.close();
    this->max_hp = hp = max_hp;

    texture_heart_full = new sf::Texture;
    texture_heart_full->loadFromFile("Assets/heart_full.png");

    texture_heart_half = new sf::Texture;
    texture_heart_half->loadFromFile("Assets/heart_half.png");

    texture_heart_empty = new sf::Texture;
    texture_heart_empty->loadFromFile("Assets/heart_empty.png");
    sprite_heart.setTexture(*texture_heart_empty);
    sprite_heart.setScale(kScale, kScale);

    texture_idle = new sf::Texture;
    texture_idle->loadFromFile("Assets/knight_idle.png");
    anim_idle = new Animator(*texture_idle, {4, 1}, 0.1f);

    texture_run = new sf::Texture;
    texture_run->loadFromFile("Assets/knight_run.png");
    anim_run = new Animator(*texture_run, {4, 1}, 0.1f);

    sprite.setTexture(*texture_idle);
    anim_current = anim_idle;

    sprite.setScale(kScale, kScale);
    sprite.setOrigin(sprite.getGlobalBounds().width / 2.0f,
                     sprite.getGlobalBounds().height / 2.0f);
    sprite.setPosition(
        sprite.getGlobalBounds().width * 4.0f
            + sprite.getGlobalBounds().width / 2.0f
            + 1.0f,
        sprite.getGlobalBounds().height * 9.0f
            + sprite.getGlobalBounds().height / 2.0f
            + 1.0f);

    texture_weapon = new sf::Texture;
    texture_weapon->loadFromFile("Assets/sword_hor.png");
    weapon = new Weapon(*texture_weapon);
    weapon->SetPosition(sprite.getPosition());
}

void Player::Heal(int value) {
    hp += value;
    if (hp > max_hp) {
        hp = max_hp;
    }
}

void Player::ReceiveDamage(int value) {
    if (invincibility.getElapsedTime().asSeconds() < invincible_time)
        return;
    hp -= value;
    if (hp <= 0) {
        is_dead = true;
    }
    invincibility.restart();
    invincible_time = 0.5f;
}

bool Player::IsDead() {
    return is_dead;
}

int Player::GetScore() {
    return score;
}

bool Player::CollideWithWalls(const std::vector<sf::Sprite> &walls) {
    for (int i = 0; i < walls.size(); ++i) {
        if (sprite.getGlobalBounds().intersects(walls[i].getGlobalBounds())) {
            return true;
        }
    }
    return false;
}

void Player::Update(float delta_time,
                    const std::vector<sf::Sprite> &walls,
                    Node<Enemy> *&enemies,
                    sf::Vector2f mouse_pos) {
    sprite.setPosition(sprite.getPosition() + velocity * delta_time);
    if (CollideWithWalls(walls)) {
        sprite.setPosition(sprite.getPosition() - velocity * delta_time);
    }
    if (velocity.x != 0 || velocity.y != 0) {
        anim_current = anim_run;
        sprite.setTexture(*texture_run);
        if (velocity.x < 0)
            sprite.setScale(-kScale, kScale);
        else
            sprite.setScale(kScale, kScale);
    } else {
        anim_current = anim_idle;
        sprite.setTexture(*texture_idle);
    }
    anim_current->Update(0, delta_time);
    sprite.setTextureRect(anim_current->uv_rect);
    sprite.setOrigin(sprite.getTextureRect().width / 2.0f,
                     sprite.getTextureRect().height / 2.0f);

    weapon->Update(mouse_pos);
    weapon->SetPosition(sprite.getPosition());

    Node<Enemy> *enemy_i = enemies, *prev = nullptr;
    while (enemy_i != nullptr) {
        Node<Enemy> *tmp = enemy_i->next;
      //  if (weapon->GetCollider().intersects(enemy_i->data.GetCollider())) {
//            enemy_i->data.ReceiveDamage(1);
            if (enemy_i->data.IsDead()) {
                if (enemy_i->data.GetID() == 1)
                    ++score;
                else if (enemy_i->data.GetID() == 2)
                    score += 3;
                else if (enemy_i->data.GetID() == 3)
                    score += 10;
                delete enemy_i;
                enemy_i = tmp;
                if (prev == nullptr) {
                    enemies = tmp;
                } else {
                    prev->next = tmp;
                }
                continue;
            }
   //     }
        prev = enemy_i;
        enemy_i = tmp;
    }
}

void Player::Draw(sf::RenderWindow &window, sf::Font font) {
//    auto debug = sprite.getGlobalBounds();
//    sf::RectangleShape rect({debug.width, debug.height});
//    rect.setPosition({debug.left, debug.top});
//    rect.setFillColor(sf::Color::Green);
//    window.draw(rect);

    window.draw(sprite);
    weapon->Draw(window);

    sf::Vector2f view_center = window.getView().getCenter();
    sf::Vector2f view_size = window.getView().getSize();
    sf::Vector2f hud_pos = {view_center.x - view_size.x / 2.0f + 10.0f,
                            view_center.y - view_size.y / 2.0f + 10.0f};
    int hp_counter = hp;
    float heart_width = sprite_heart.getGlobalBounds().width;
    for (int i = 0; i < max_hp / 2; ++i) {
        if (hp_counter >= 2) {
            sprite_heart.setTexture(*texture_heart_full);
            hp_counter -= 2;
        } else if (hp_counter >= 1) {
            sprite_heart.setTexture(*texture_heart_half);
            --hp_counter;
        } else {
            sprite_heart.setTexture(*texture_heart_empty);
        }
        sprite_heart.setPosition(hud_pos);
        window.draw(sprite_heart);
        hud_pos.x += heart_width + 5.0f;
    }

    hud_pos = {view_center.x - view_size.x / 2.0f + 10.0f,
               view_center.y - view_size.y / 2.0f + 15.0f + sprite_heart.getGlobalBounds().height};
    sf::Text text;
    text.setFont(font);
    text.setPosition(hud_pos);
    if (score >= highscore) {
        highscore = score;
        text.setFillColor(sf::Color(255, 119, 0));
    }
    else {
        text.setFillColor(sf::Color::White);
    }
    text.setString("Score: " + std::to_string(score) + " (" + std::to_string(highscore) + ")");
    text.setCharacterSize(kTextSize);
    window.draw(text);
}

void Player::SetHorVelocity(float _velocity) {
    velocity.x = _velocity;
}

void Player::SetVertVelocity(float _velocity) {
    velocity.y = _velocity;
}

sf::Vector2f Player::GetPosition() {
    return sprite.getPosition();
}

sf::FloatRect Player::GetCollider() {
    return sprite.getGlobalBounds();
}

int Player::GetHighScore() {
    return highscore;
}

sf::FloatRect Player::GetWeaponCollider() {
    return weapon->GetCollider();
}