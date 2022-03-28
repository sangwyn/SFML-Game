#include "Weapon.h"
#include "Constants.h"
#include <cmath>

Weapon::Weapon(const sf::Texture &texture) {
    sprite.setTexture(texture);
    sprite.setScale(kScale, kScale);
    sprite.setOrigin(0,
                     sprite.getTextureRect().height / 2.0f);
}

void Weapon::SetPosition(sf::Vector2f pos) {
    sprite.setPosition(pos);
}

sf::FloatRect Weapon::GetCollider() {
    return sprite.getGlobalBounds();
}

void Weapon::Update(sf::Vector2f mouse_pos) {
    float angle = atan2(mouse_pos.y - sprite.getPosition().y,
                        mouse_pos.x - sprite.getPosition().x) * 180
        / 3.141;
    if (angle < 0.0f)
        angle += 360.0f;
    if (angle > 90.0f && angle < 270.0f)
        sprite.setScale(kScale, -kScale);
    else
        sprite.setScale(kScale, kScale);
    sprite.setRotation(angle);
}

void Weapon::Draw(sf::RenderWindow &window) {
//    auto debug = sprite.getGlobalBounds();
//    sf::RectangleShape rect({debug.width, debug.height});
//    rect.setPosition({debug.left, debug.top});
//    rect.setFillColor(sf::Color::Red);
//    window.draw(rect);

    window.draw(sprite);
}