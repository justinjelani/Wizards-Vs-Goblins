#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "enemy.h"

class EnemySpawner{
public:
    EnemySpawner(const sf::Texture& enemyTexture, int level);

    void update(float dt, std::vector<Enemy>& enemies, float width, float height);

    void resetEnemies(int newLevel);
    void removeEnemy(){enemyCount--;}
    int getEnemiesLeft(){return enemyCount;}

private:
    const sf::Texture& enemyTexture;
    const int baseEnemiesPerWave = 5;
    const int baseEnemiesPerLevel = 5;

    int level = 1;

    int enemiesLeft;
    int enemyCount;

    // Spawning
    float spawnTimer = 0.f;
    float delay; // ONLY THING CONTROLLING SPAWN SPEED

    sf::Vector2f getOffScreenSpawn(float width, float height, float offset);
};


