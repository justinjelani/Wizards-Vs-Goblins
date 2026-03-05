#pragma once
#include <SFML/Graphics.hpp>
#include "json.hpp"
#include "player.h"
#include "enemy.h"
#include "enemyspawner.h"
#include "coinpopup.h"
#include <fstream>
#include <string>
#include <vector>

using json = nlohmann::json;


class Game{
public:

    enum class GameState{Home,Playing,EndGame,Paused, StartLevel, Shop};
    enum class GameResult{won, died};

    Game();
    void run();

private:

    // Core
    void separateEnemies(std::vector<Enemy>& enemies, float minDistance);
    void processEvents();
    void update(float dt);
    void render();
    void reset(bool keepLevel);
    void loadSaves();
    void loadSaveSlot();
    // void spawnEnemies(float dt);
    
    struct Save{
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
    // Window
    sf::RenderWindow window;
    unsigned int width = 1280.f;
    unsigned int height = 720.f;

    // Save saves
    Save saves[3] = {{"save1.json"},{"save2.json"}, {"save3.json"}};

    // Save Selection
    int saveSelected = 0; // Selecting through menus
    int uiSelection = 0; // End screen selection
    float startY = height/2.f - 60;
    float spacing = 60.f;

    // Game state
    GameState state = GameState::Home;
    GameResult result;
    // Timing
    sf::Clock clock;
    // Assets
    sf::Texture playerTexture;
    sf::Texture enemyTexture;
    sf::Texture projectileTexture;
    sf::Font font;

    // Objects
    Player player;
    std::vector<Enemy> enemies;
    EnemySpawner spawner;
    std::vector<CoinPopup> coinPopups;

    // UI Text
    sf::Text titleText;
    sf::Text enterText;
    sf::Text backText;
    sf::Text pausedText;
    sf::Text healthText;
    sf::Text coinsText;
    sf::Text staminaText;
    sf::Text levelText;
    sf::Text controlsText;
    sf::Text resetText;
    sf::Text spaceText;
    sf::Text chargeText;
    sf::Text enemyText;
    sf::Text homeText;
    sf::Text nextLevelText;
    sf::Text againText;
    sf::Text victoryText;
    sf::Text diedText;
    sf::Text shopText;
    sf::Text startLevelText;
};