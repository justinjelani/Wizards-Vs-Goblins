#include "enemyspawner.h"
#include <random>
#include <cstdlib>
#include <algorithm>
#include <SFML/Graphics.hpp>

EnemySpawner::EnemySpawner(const sf::Texture& enemyTexture, int level)
: enemyTexture(enemyTexture), level(level), enemiesLeft(baseEnemiesPerLevel + level*baseEnemiesPerLevel), enemyCount(baseEnemiesPerLevel + level*baseEnemiesPerLevel), spawnTimer(0.f)
{}

sf::Vector2f EnemySpawner::getOffScreenSpawn(float width, float height, float offset){
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> sideDist(0, 3);
    std::uniform_real_distribution<float> xDist(0.f, width);
    std::uniform_real_distribution<float> yDist(0.f, height);

    int side = sideDist(gen);

    switch(side){
        case 0: return{-offset, yDist(gen)};
        case 1: return{width + offset, yDist(gen)};
        case 2: return{xDist(gen), -offset};
        default: return{ xDist(gen), height + offset};
    }
}

void EnemySpawner::update(float dt, std::vector<Enemy>& enemies, float width, float height){
    if (enemiesLeft<=0){return;}

        // Higher level = faster spawns (min delay, delay between spawns, level * how much to decrease per level)
            delay = std::max(0.3f, 1.5f - level * 0.1f);

        spawnTimer+= dt;
        while(spawnTimer >= delay && enemiesLeft > 0){
            
            // decrease timer
            spawnTimer -= delay;

            //spawn enemies
            sf::Vector2f spawnPos = getOffScreenSpawn(width, height, 50);
            int hp = 125 + level*2;
            int damage = 10 + level*2;
            enemies.emplace_back(enemyTexture, spawnPos, hp, hp, damage);
            enemiesLeft--;
        }
}
void EnemySpawner::resetEnemies(int newLevel){
        level = newLevel;
        enemyCount = baseEnemiesPerLevel + level*baseEnemiesPerLevel;
        enemiesLeft = enemyCount;
        spawnTimer = 0.f;
}
