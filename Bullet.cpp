#include "Bullet.h"
#include "Constants.h"

Bullet::Bullet() {

}

Bullet::Bullet(sf::Vector2f position,
               float rotation,
               sf::Vector2f velocity,
               bool is_enemy,
               const sf::Texture &texture) {
    sprite.setTexture(texture);
    sprite.setScale(kScale, kScale);
    sprite.setPosition(position);
    sprite.setRotation(rotation);
    this->velocity = velocity;
    this->is_enemy = is_enemy;

    max_lifetime = 1.0f;
    lifetime.restart();
}

void Bullet::Draw(sf::RenderWindow &window) {
    window.draw(sprite);
}

bool Bullet::CollideWithWalls(const std::vector<sf::Sprite> &walls) {
    for (int i = 0; i < walls.size(); ++i) {
        if (sprite.getGlobalBounds().intersects(walls[i].getGlobalBounds())) {
            return true;
        }
    }
    return false;
}

bool Bullet::Update(const std::vector<sf::Sprite> &walls) {
    if (lifetime.getElapsedTime().asSeconds() >= max_lifetime)
        return true;
    sprite.setPosition(sprite.getPosition() + velocity);
    if (CollideWithWalls(walls))
        return true;
    return false;
}