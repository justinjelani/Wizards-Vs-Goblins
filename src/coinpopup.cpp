#include "coinpopup.h"
#include <cstdint>

CoinPopup::CoinPopup(const sf::Vector2f pos, int amount, sf::Font& font)
: text(font, sf::String("+" + std::to_string(amount)), 24), lifetime(1.0f) //1 second
    {
      sf::FloatRect bounds = text.getLocalBounds();
      text.setFillColor(sf::Color::Yellow);
      text.setPosition(pos);
      text.setOrigin({bounds.size.x/2.f, bounds.size.y/2.f});
    }

    void CoinPopup::update(float dt){
        lifetime-=dt;
        text.move({0.f, -50.f*dt}); // Moves upward

        sf::Color c = text.getFillColor();
        c.a = static_cast<uint8_t>(255* (lifetime/1.0f)); // Fades out
        text.setFillColor(c);
    }
    void CoinPopup::draw(sf::RenderWindow& window) const{
        window.draw(text);
    }
    bool CoinPopup::expired() const{
        return lifetime <= 0.f;
    }