#pragma once
#include <SFML/Graphics.hpp>

class Enemy{
public:
    Enemy(const sf::Texture& texture, const sf::Vector2f& startPos, int hp, int maxHealth, int attack);

    void update(float dt, const sf::Vector2f& startPos);
    void draw(sf::RenderWindow& window) const;

    // Enemy combat
    void takeDmg(int amount);
    bool isDead() const;
    bool canAttack();
    void resetAttackTimer();
    void updateHealthBar();
    int dropCoins(int level) const;

    // Get methods
    int getHealth() const;
    int getMaxHealth() const;
    int getAttack() const;
    float getAttackTimer() const;
    float getAttackCooldown() const;
    sf::FloatRect getHitbox() const;

    // Enemy position/boundaries
    sf::FloatRect getBounds() const;
    sf::Vector2f getPos() const;
    void setPosition(const sf::Vector2f& pos);

private:
    sf::Sprite sprite;
    sf::RectangleShape frontBar;
    sf::RectangleShape backBar;
    
    int health;
    int maxHealth;
    int meleeAttack;
    float attackCooldown; // seconds between each attack
    float attackTimer; // tracks time from last attack
    float speed;
};