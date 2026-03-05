#include "Player.h"
#include "Enemy.h"
#include "coinpopup.h"
#include "json.hpp"
#include "enemyspawner.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <algorithm> // Need for clamp
static constexpr int CURRENT_SAVE_VERSION = 1;
using json = nlohmann::json;

// Construct Player
Player::Player(const sf::Texture& texture, const sf::Texture& projTexture)
:   sprite(texture), 
    projectileTexture(projTexture), 
    level(1),
    maxHealth(100), 
    health(maxHealth),
    maxStamina(100), 
    stamina(100),
    damage(15), 
    speed(275.f),  
    healAmount(20) 
    {
    
    
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x/2.f, bounds.size.y/2.f});
    sprite.setPosition({640.f, 360.f}); // half of screen resolution
    sprite.setScale({1.f, 1.f});
};

// Selecting player slots, loading player based on slot selected

struct SaveSlot{
    std::string filename;
    int level = 1;
    bool exists = false;

    void load(){
        std::ifstream file(filename);
        if(!file.is_open()){
            exists = false;
            level = 1;
            return;
        }
        json j;
        file >>j;
        exists = true;
        level = j.value("level", 1);
    }
};

// Check if slot is empty
bool slotExists(const std::string& filename){
    std::ifstream file(filename);
    return file.good();
}
// Player input
void Player::input(float dt, sf::RenderWindow& window){


    bool isRegen = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C);
    // Movement if not regenerating stamina
    if(!isRegen){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){sprite.move({-speed * dt, 0.f});}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){sprite.move({speed * dt, 0.f});}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){sprite.move({0.f, -speed * dt});}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){sprite.move({0.f, speed * dt});}
        }
    // If holding C, regen stamina
    if(isRegen){
        regenTimer += dt;
        if(regenTimer >= 0.20f && stamina<maxStamina){
            stamina= std::min(stamina + staminaRegen, maxStamina);
            if(health<maxHealth){
                health = std::min(health + healthRegen, maxHealth);
            }
            regenTimer = 0.f;
        }
    }

    // Set health to max
    bool rPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R);
    
    // Reset character to level 1
    if(rPressed && !lastR){
        setHealth(maxHealth);
        setStamina(maxStamina);
    }
    lastR = rPressed;
    
    // Shoot per key press while not regenerating
    bool spacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
    int staminaCost = 4;
    if(!isRegen && spacePressed && !lastSpace && stamina>= staminaCost){
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        shoot(mousePos);
        stamina-=staminaCost;
    }
    lastSpace = spacePressed;
}

// Save & Load Player
void Player::saveToFile(const std::string& filename) const{
    json j;

    // Current save version
    j["saveVersion"] = CURRENT_SAVE_VERSION;

    // Save current stats
    j["level"] = level;
    j["coins"] = coins;
    j["health"] = health;
    j["maxHealth"] = maxHealth;
    j["healAmount"] = healAmount;
    j["stamina"] = stamina;
    j["maxStamina"] = maxStamina;
    j["staminaRegen"] = staminaRegen;
    j["healthRegen"] = healthRegen;
    j["damage"] = damage;
    j["speed"] = speed;

    // Save sprite
    std::ofstream file(filename);
    if(file.is_open()){
        file << j.dump(4);
    }
}
void Player::loadFromFile(const std::string& filename){
    std::ifstream file(filename);
    if(!file.is_open()) return;

    json j;
    file >> j;

    int saveVersion = j.value("saveVersion", 1);

    level = j.value("level", 1);
    coins = j.value("coins", 0);
    maxHealth = j.value("maxHealth", 100);
    health = std::clamp(j.value("health", maxHealth), 0, maxHealth);
    healAmount = j.value("healAmount", 20);
    maxStamina = j.value("maxStamina", 100);
    stamina = std::clamp(j.value("stamina", maxStamina), 0, maxStamina);
    staminaRegen = j.value("staminaRegen", 5);
    damage = j.value("damage", 15);
    speed = j.value("speed", 500.f);

    if(saveVersion > 1){
        // New stat changes go here per save (ex. maxhealth+= 20)
        // ex. coins+= 50; could be given to players for compensation on new save
    }
}
void Player::reset(bool keepLevel){
    if(!keepLevel) level = 1;
    health = maxHealth;
    stamina = maxStamina;
}
// Draw player to screen (position)
void Player::clampToWindow(unsigned int width, unsigned int height){

    sf::Vector2 pos = sprite.getPosition();
    // sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::FloatRect hitbox = getHitbox();

    float halfWidth = hitbox.size.x/2.f;
    float halfHeight = hitbox.size.y/2.f;

    // Left
    if(pos.x < halfWidth){
        pos.x = halfWidth;
    }
    // Right
    if(pos.x > width - halfWidth){
        pos.x = width- halfWidth;
    }
    // Top
    if(pos.y < halfHeight){
        pos.y = halfHeight;
    }
    // Bottom
    if(pos.y > height - halfHeight){
        pos.y = height - halfHeight;
    }

    sprite.setPosition(pos);
}
void Player::draw(sf::RenderWindow& window) const{
    window.draw(sprite);
    for(const auto& proj : projectiles){
        proj->draw(window);
    }
}
void Player::updateProjectiles(float dt, const sf::RenderWindow& window, std::vector<Enemy>& enemies, std::vector <CoinPopup>& coinPopups, EnemySpawner& spawner, sf::Font& font){
    for(auto pIt = projectiles.begin(); pIt != projectiles.end();){
        (*pIt)->update(dt);
        bool collided = false;
        for(auto eIt = enemies.begin(); eIt != enemies.end();){
            auto intersection = (*pIt)->getBounds().findIntersection(eIt->getHitbox());
            if(intersection){
                eIt->takeDmg(25);
                //Remove Projectile
                pIt = projectiles.erase(pIt);
                collided = true;
                // Enemy death = give coins + erase enemy
                if(eIt->isDead()){
                    int coinsDropped = eIt->dropCoins(level);
                    addCoins(coinsDropped);
                    coinPopups.emplace_back(eIt->getPos(), coinsDropped, font);
                    eIt = enemies.erase(eIt);
                    spawner.removeEnemy();
                } else {++eIt;}
                break;
            } else {++eIt;}
        }
        if(collided){continue;}

        // If projectile goes off screen or hits hitbox, erase
        if((*pIt)->isOffScreen(window)) pIt = projectiles.erase(pIt);
        else ++pIt;
    }

    // Remove dead enemies
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
    [](const Enemy& e){return e.isDead(); }), enemies.end());
}   
void Player::clearProjectiles(){
    projectiles.clear();
}

// Player Combat
void Player::attack(Enemy& enemy){
    // implement later
}
void Player::dash(float multiplier, float duration){
}
void Player::heal(int amount){
 /*Adds amount to the current player health
   and prevents from exceeding maxHealth
 */ 
 health = std::min(health + amount, maxHealth);   
}
void Player::takeDmg(int amount, sf::Vector2f enemyPos){

    health = std::max(0, health - amount);

    sf::Vector2f direction = sprite.getPosition() - enemyPos;
    float distance = std::sqrt(direction.x*direction.x + direction.y*direction.y);
    if(distance != 0.f){
        direction /= distance;
        float kbStrength = 600.f;
        velocity = direction * kbStrength;
    }
}
void Player::move(sf::Vector2f offset) {
    sprite.move(offset); // Pass the move command to the actual sprite
}
void Player::shoot(const sf::Vector2f& targetPos){
   projectiles.push_back(
    std::make_unique<Projectile>(projectileTexture, getPos(), targetPos, 600.f)
   );
};
sf::FloatRect Player::getBounds() const{
    return sprite.getGlobalBounds();
}
sf::FloatRect Player::getHitbox() const{
    sf::FloatRect bounds = sprite.getGlobalBounds();

    // size of the hitbox
    float hWidth = bounds.size.x * 0.30f;
    float hHeight = bounds.size.y * 0.50f;

    // Fix asymmetry with offsets
    float offsetX = -4.f;
    float offsetY = 5.f;

    sf::Vector2f center = {
        bounds.position.x + bounds.size.x / 2.f + offsetX,
        bounds.position.y + bounds.size.y / 2.f + offsetY
    };
    return sf::FloatRect({center.x - hWidth / 2.f,
          center.y - hHeight / 2.f },
        { hWidth, hHeight }
    );
}
sf::Vector2f Player::getPos() const{
    return sprite.getPosition();
}


