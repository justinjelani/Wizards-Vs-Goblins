#include "projectile.h"
#include <cmath>

Projectile::Projectile(const sf::Texture& texture, const sf::Vector2f& startPos, const sf::Vector2f& targetPos, float speed)
:sprite(texture)
{  
    // Center the origin for movement
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x/2.f, bounds.size.y/2.f});
    sprite.setPosition(startPos);

    // Calculate direction
    sf::Vector2f direction = targetPos - startPos;
    float length = std::sqrt(direction.x*direction.x + direction.y*direction.y);
    if(length != 0.f) direction/=length;
    velocity = direction * speed;
}

void Projectile::update(float dt){
    sprite.move(velocity*dt);
}
void Projectile::draw(sf::RenderWindow& window) const{
    window.draw(sprite);
}
sf::FloatRect Projectile::getBounds() const{
    return sprite.getGlobalBounds();
}
bool Projectile::isOffScreen(const sf::RenderWindow& window) const{
    sf::Vector2f pos = sprite.getPosition();
    return pos.x < 0 || pos.x > window.getSize().x || pos.y < 0 || pos.y > window.getSize().y;
}