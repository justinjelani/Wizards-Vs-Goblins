#include "enemy.h"
#include "player.h"
#include "coinpopup.h"
#include <cmath>
#include <random>

Enemy::Enemy(const sf::Texture& texture, const sf::Vector2f& startPos, int hp, int maxHealth, int attack)
: sprite(texture), speed(200.f), health(hp), maxHealth(hp), meleeAttack(attack), attackCooldown(1.f), attackTimer(0.f){
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x/2.f, bounds.size.y/2.f});
    sprite.setPosition(startPos);
    sprite.setScale({1, 1});

    // Health Bars
    frontBar.setSize({50.f, 5.f});
    frontBar.setFillColor(sf::Color::Green);
    backBar.setSize({50.f, 5.f});
    backBar.setFillColor(sf::Color::Red);    
}

// Enemy Position
void Enemy::update(float dt, const sf::Vector2f& playerPos){
    // Move toward the player
    sf::Vector2f direction = playerPos - sprite.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y* direction.y);
    if(distance != 0.f){
        direction /= distance;
        sprite.move(direction * speed * dt);
    }

    // Attack timer
    attackTimer += dt;
    if(attackTimer > attackCooldown){
        attackTimer = attackCooldown; // Caps the timer with cooldown
    }
}
void Enemy::draw(sf::RenderWindow& window) const{
    window.draw(sprite);
    window.draw(backBar);
    window.draw(frontBar);
}

// Enemy combat
void Enemy::takeDmg(int amount){
    // Prevents negative health (below 0)
    health = std::max(health - amount, 0);
}
bool Enemy::isDead() const{
    return health <= 0;
}
bool Enemy::canAttack(){
    return attackTimer >= attackCooldown;
}
void Enemy::resetAttackTimer(){
    attackTimer = 0.f;
}
void Enemy::updateHealthBar(){
    // Create original bar position
    sf::Vector2f barPos = sprite.getPosition();
    barPos.y -= 30.f;
    barPos.x -= 50.f /2.f;

    // Set color of health bars, update position
    backBar.setPosition(barPos);
    frontBar.setPosition(barPos);

    // Tracks enemy health
    float enemyHealth = static_cast<float>(health)/maxHealth;
    if(enemyHealth < 0.f) enemyHealth = 0.f;
    if(enemyHealth > 1.f) enemyHealth = 1.f;
    frontBar.setSize(sf::Vector2f(50.f * enemyHealth, 5.f));
}

// Get Methods
float Enemy::getAttackTimer() const{
    return attackTimer;
}
float Enemy::getAttackCooldown() const{
    return attackCooldown;
}
int Enemy::getHealth() const{
    return health;
}
int Enemy::getAttack() const{
    return meleeAttack;
}
int Enemy::getMaxHealth() const{
    return maxHealth;
}
sf::FloatRect Enemy::getBounds() const{
    return sprite.getGlobalBounds();
}
sf::Vector2f Enemy::getPos() const{
    return sprite.getPosition();
}
sf::FloatRect Enemy::getHitbox() const{
    sf::FloatRect bounds = sprite.getGlobalBounds();

    // size of the hitbox
    float hWidth = bounds.size.x * 0.27f;
    float hHeight = bounds.size.y * 0.43f;

    // Fix asymmetry with offsets
    float offsetX = 2.f;
    float offsetY = 22.f;

    sf::Vector2f center = {
        bounds.position.x + bounds.size.x / 2.f + offsetX,
        bounds.position.y + bounds.size.y / 2.f + offsetY
    };
    return sf::FloatRect({center.x - hWidth / 2.f,
          center.y - hHeight / 2.f },
        { hWidth, hHeight }
    );
}
void Enemy::setPosition(const sf::Vector2f& pos){
    sprite.setPosition(pos);
}

// Gameplay
int Enemy::dropCoins(int level) const{
    static std::random_device rand;
    static std::mt19937 gen(rand());

    int minCoins = 40;
    int maxCoins = 50;

    std::uniform_int_distribution<> dis(minCoins, maxCoins);
    return dis(gen) + level * 5;
}



