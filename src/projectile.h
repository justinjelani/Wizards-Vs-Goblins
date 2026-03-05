#pragma once
#include <SFML/Graphics.hpp>

class Projectile{
public:
    Projectile(const sf::Texture& texture, const sf::Vector2f& startPos, const sf::Vector2f& targetPos, float speed = 1800.f);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    bool isOffScreen(const sf::RenderWindow& window) const;



private:
sf::Sprite sprite;
sf::Vector2f velocity;
float speed;
};