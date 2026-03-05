#pragma once
#include "projectile.h"
#include <memory>
#include <algorithm>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

class Enemy;
class EnemySpawner;
class CoinPopup;

class Player{
public:

    // Constructor for player
    Player(const sf::Texture& texture, const sf::Texture& projTexture);

    sf::Vector2f getPos() const;    
    // Input/movement
    void input(float dt, sf::RenderWindow& window);
    void move(sf::Vector2f offset);
    // Drawing player
    void clampToWindow(unsigned int width, unsigned int height);
    void draw(sf::RenderWindow& window) const;
    void updateProjectiles(float dt, const sf::RenderWindow& window, std::vector<Enemy>& enemies, std::vector <CoinPopup>& coinPopups, EnemySpawner& spawner, sf::Font& font);
    void clearProjectiles();

    // Player combat
    void attack(Enemy& enemy);
    void shoot(const sf::Vector2f& targetpos);
    void dash(float multiplier, float duration);
    void heal(int amount);
    void takeDmg(int amount, sf::Vector2f enemyPos);
    bool isDead(){return getHealth() <= 0;}

    // get methods
    int getCoins() const {return coins;}
    void addCoins(int amount){coins+= amount;}
    int getHealth() const{ return health;}
    int getMaxHealth() const{return maxHealth;}
    int getStamina() const{return stamina;}
    int getMaxStamina() const{return maxStamina;}
    int getDamage() const{return damage;}
    int getLevel() const{return level;}
    float getSpeed() const{return speed;}
    sf::FloatRect getHitbox() const;
    sf::FloatRect getBounds() const;

    // set methods
    void setHealth(int newHealth){health = std::clamp(newHealth, 0, maxHealth);}
    void setCoins(int newCoins){coins = newCoins;}
    void setStamina(int newStamina){stamina = newStamina;}
    void levelUp(){level++;}
    // Positioning
    void setPosition(const sf::Vector2f& pos) {sprite.setPosition(pos);}
    sf::Vector2f velocity{0.f, 0.f};

    // Saving & Loading player
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void reset(bool keepLevel);

private:
    sf::Sprite sprite;
    sf::Texture projectileTexture;
    std::vector<std::unique_ptr<Projectile>> projectiles;

    bool lastSpace = false;
    bool lastR = false;

    // Player Stats
    int health;
    int maxHealth;
    int stamina;
    int maxStamina;

    // Abilities
    int healAmount;
    int damage; 
    float speed; 
    int healthRegen = 2;
    int staminaRegen = 15;
    float regenTimer = 0.f;
    
    // Coins & Level
    int level;
    int coins = 0;
};