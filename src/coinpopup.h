#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class CoinPopup{
public:
    CoinPopup(const sf::Vector2f pos, int amount, sf::Font& font);
    
    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    bool expired() const;
    
private:
    sf::Text text;
    float lifetime; // seconds
};