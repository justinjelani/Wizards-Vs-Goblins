#include "game.h"
#include <fstream>
#include <iostream>

Game::Game()
: width(1920), height(1080), window(sf::VideoMode::getDesktopMode(), "Goblin Game", sf::Style::Default),
  playerTexture(), enemyTexture(), projectileTexture(),
  player(playerTexture, projectileTexture), spawner(enemyTexture, 1),
  titleText(font, "Wizards vs Goblins", 96),
    enterText(font, "Press ENTER to Start", 40),
    backText(font, "Press backspace to go Home", 40),
    pausedText(font, "Paused", 96),
    healthText(font, "Health: 100/100", 45),
    coinsText(font, "Coins: 0", 40),
    staminaText(font, "Stamina 100/100", 40),
    levelText(font, "Level: ", 50),
    controlsText(font, "Controls:", 40),
    resetText(font, "R - reset", 40),
    spaceText(font, "Space - Shoot", 40),
    chargeText(font, "C - Recharge", 40),
    enemyText(font, "Enemies Left: ", 40),
    homeText(font, "Main Menu", 75),
    nextLevelText(font, "Next Level", 75),
    againText(font, "Play Again", 75),
    victoryText(font, "VICTORY!", 128),
    diedText(font, "You DIED!", 128),
    shopText(font, "Shop", 75),
    startLevelText(font, "Start Level", 75)
{
    window.setFramerateLimit(120);
    // Load Textures
    if(!font.openFromFile("assets/BBHBogle-Regular.ttf")){std::cerr << "failed to load font text";}
    if(!playerTexture.loadFromFile("assets/idleframes000.png")){std::cerr << "failed to load player text";} 
    if(!enemyTexture.loadFromFile("assets/idle_0000.png")){std::cerr << "failed to load enemy text";}   
    if(!projectileTexture.loadFromFile("assets/fireball1.png")){std::cerr << "failed to load proj text";} 

    loadSaves();

    player = Player(playerTexture, projectileTexture);   

    // Title Screen
    titleText.setFillColor(sf::Color::Green);
    titleText.setPosition({width/2.f - titleText.getLocalBounds().size.x/2.f, height/2.f - 250.f});
    
    // Homescreen Text
    enterText.setFillColor(sf::Color::Green);
    enterText.setPosition({width/2.f - enterText.getLocalBounds().size.x/2.f, height/2.f +150});

    // Backspace text
    backText.setFillColor(sf::Color::White);
    backText.setPosition({width/2.f - backText.getLocalBounds().size.x/2.f, height/2.f + 200});

    // Pause Screen
    pausedText.setFillColor(sf::Color::Red);
    pausedText.setPosition({width/2.f - pausedText.getLocalBounds().size.x/2.f, height/2.f - 250.f});

    // Health display
    healthText.setFillColor(sf::Color::Red);
    healthText.setPosition({10, 5});
    
    // Coin display
    coinsText.setFillColor(sf::Color::Yellow);
    coinsText.setPosition({10, 45});

    // Stamina display
    staminaText.setFillColor(sf::Color::Cyan);
    staminaText.setPosition({10, 85});

    // Enemies left
    enemyText.setFillColor(sf::Color::Green);
    enemyText.setPosition({800, 5});

    // Level Text
    levelText.setFillColor(sf::Color::Magenta);
    levelText.setPosition({1750, 5});

    // Controls
    controlsText.setFillColor(sf::Color::Green);
    controlsText.setPosition({10, 800});
    // Reset
    resetText.setFillColor(sf::Color::Green);
    resetText.setPosition({10, 850});
    // Space
    spaceText.setFillColor(sf::Color::Green);
    spaceText.setPosition({10, 900});
    // Charge
    chargeText.setFillColor(sf::Color::Green);
    chargeText.setPosition({10, 950});

    // Start Level Texts
    shopText.setFillColor(sf::Color::White);
    shopText.setPosition({width/2.f - shopText.getLocalBounds().size.x/2.f , height/2.f});
    // Next Level
    startLevelText.setFillColor(sf::Color::White);
    startLevelText.setPosition({width/2.f - startLevelText.getLocalBounds().size.x/2.f + 400, height/2.f});
    
    //End Game Texts
    // Main menu
    homeText.setFillColor(sf::Color::White);
    homeText.setPosition({width/2.f - homeText.getLocalBounds().size.x/2.f - 400, height/2.f});
    // Next Level
    nextLevelText.setFillColor(sf::Color::White);
    nextLevelText.setPosition({width/2.f - nextLevelText.getLocalBounds().size.x/2.f +400, height/2.f});
    // Play Again
    againText.setFillColor(sf::Color::White);  
    againText.setPosition({width/2.f - againText.getLocalBounds().size.x/2.f + 400, height/2.f});
    // Play Again
    victoryText.setFillColor(sf::Color::Green);  
    victoryText.setPosition({width/2.f - victoryText.getLocalBounds().size.x/2.f, height/2.f - 250.f});
    // Play Again
    diedText.setFillColor(sf::Color::Red);  
    diedText.setPosition({width/2.f - diedText.getLocalBounds().size.x/2.f, height/2.f - 250.f});

    // Set player to center of screen
    player.setPosition({static_cast<float>(width)/2.f, static_cast<float>(height)/2.f});
    
}

void Game::separateEnemies(std::vector<Enemy>& enemies, float minDistance){
    for(size_t i = 0; i < enemies.size(); i++){
        for(size_t j = 1 + i; j < enemies.size(); j++){
            
            sf::Vector2f positionA = enemies[i].getPos();
            sf::Vector2f positionB = enemies[j].getPos();

            sf::Vector2f delta = positionB-positionA;
            float distance = std::sqrt(delta.x*delta.x + delta.y*delta.y);

            if(distance > 0.f && distance < minDistance){
                sf::Vector2f direction = delta/distance;
                float push = (minDistance - distance) * 0.5f;

                enemies[i].setPosition(positionA-direction*push);
                enemies[j].setPosition(positionB+direction*push);
            }
        }
    }
}
void Game::loadSaves(){
    for(auto&save : saves) save.load();
}
void Game::loadSaveSlot(){

     if(saves[saveSelected].exists){
         player.loadFromFile(saves[saveSelected].filename);
         reset(true);
         saves[saveSelected].level = player.getLevel();
        } else {  
            player.reset(false);
            saves[saveSelected].level = player.getLevel();
            player.saveToFile(saves[saveSelected].filename);
        }
}
void Game::reset(bool keepLevel){
    player.clearProjectiles();
    player.reset(keepLevel);
    enemies.clear();
    spawner.resetEnemies(player.getLevel());
}

void Game::update(float dt){

    // Player input and movement
    player.input(dt, window);
    player.clampToWindow(width, height);

    // Enemy spawner
    spawner.update(dt, enemies, width, height);
   
    // Seperate Enemies
    separateEnemies(enemies, 50);

    // Update enemies
    for(auto& enemy : enemies){
    /* (hitboxes if needed)
        sf::FloatRect hb = enemy.getHitbox();
        sf::RectangleShape debugRect;
        debugRect.setPosition(s.position);
        debugRect.setSize(hb.size);
        debugRect.setFillColor(sf::Color::Transparent);
        debugRect.setOutlineColor(sf::Color::Red);
        debugRect.setOutlineThickness(1.f);
        window.draw(debugRect);
    */
        enemy.update(dt, player.getPos());
        enemy.updateHealthBar();

        // enemy attack
        auto intersection = player.getHitbox().findIntersection(enemy.getHitbox());
        if (intersection && enemy.canAttack()) {
            player.takeDmg(enemy.getAttack(), enemy.getPos());
            enemy.resetAttackTimer();
        }
    }

    // Update player after taking damage
    player.move(player.velocity * dt);
    player.velocity *= std::pow(0.1f, dt);
    /* (player hitbox if needed)
        sf::FloatRect hb2 = player.getHitbox();
        sf::RectangleShape debugRect2;
        debugRect2.setPosition(hb2.position);
        debugRect2.setSize(hb2.size);
        debugRect2.setFillColor(sf::Color::Transparent);
        debugRect2.setOutlineColor(sf::Color::Green);
        debugRect2.setOutlineThickness(1.f);
        window.draw(debugRect2);
    */

    // Update player projectiles and collisions
    player.updateProjectiles(dt, window, enemies, coinPopups, spawner, font);

    // update coin popup
    for(auto it = coinPopups.begin(); it != coinPopups.end();){
        it->update(dt);
        if(it->expired()){
            it = coinPopups.erase(it);
        } else {++it;}
    }

    // Update UI
    healthText.setString("Health: " + std::to_string(player.getHealth()) + "/" + std::to_string(player.getMaxHealth()));     
    coinsText.setString("Coins: " + std::to_string(player.getCoins()));
    staminaText.setString("Stamina: " + std::to_string(player.getStamina()) + "/" + std::to_string(player.getMaxStamina()));
    enemyText.setString("Enemies Left: " + std::to_string(spawner.getEnemiesLeft()));
    levelText.setString("Level: " + std::to_string(player.getLevel()));
        /*

        std::unique_ptr<Player> player = std::make_unique<Player>(playerTexture, projectileTexture);
        while(window.isOpen()){
        

            while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
        }
            processEvents();
            update(dt);
            render();
        }
}
        */
}

// Main Game funcs
void Game::run(){
    while(window.isOpen()){
            float dt = clock.restart().asSeconds();
            processEvents();

            if(state == GameState::Playing){
            update(dt);
            }

            render();
        }
}
void Game::processEvents(){
    while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                player.saveToFile(saves[saveSelected].filename);
                window.close();
        }
        if (event->is<sf::Event::Resized>()) {
             auto size = event->getIf<sf::Event::Resized>()->size;

            sf::View view(sf::FloatRect(
             sf::Vector2f({0.f, 0.f}), 
             sf::Vector2f(static_cast<float>(width), 
             static_cast<float>(height))));

    window.setView(view);
}
        // Start game from home screen
            switch(state){
                case GameState::Home:
                player.reset(true);
                if(event->is<sf::Event::KeyPressed>()){
                // Down arrow
                if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Down){
                    saveSelected = (saveSelected+1)%3;
                }
                // Up arrow
                if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Up){
                    saveSelected = (saveSelected+2)%3;
                }
                // Select a Slot
                if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Enter){
                    if(saves[saveSelected].exists){
                        // Load selected save if it exists, reset enemies for level, send to start level screen
                        loadSaveSlot();
                        reset(true);
                        state = GameState::StartLevel;
                } else {
                    state = GameState::Playing;
                }
            }
        }
                    break;
                // Start Level Screen
                case GameState::StartLevel:
                    if(event->is<sf::Event::KeyPressed>()){
                // Arrow usage
                if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Right){
                    uiSelection = (uiSelection+1)%3;
                    } else if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Left){
                        uiSelection = (uiSelection+3-1)%3;
                    }
                // Select a Option
                if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Enter){
                    // If selecting home screen
                    if(uiSelection == 0){
                            state = GameState::Home;
                        } else if(uiSelection == 1){
                            state = GameState::Shop;
                            uiSelection = 0;
                        } else {
                            state = GameState::Playing;
                            uiSelection = 0;
                        }
                    break;
                // Switch to main game
                case GameState::Playing:
                    if(event->is<sf::Event::KeyPressed>()){
                        if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape){
                            player.saveToFile(saves[saveSelected].filename);
                            state = GameState::Paused;
                            std::cout<< "Paused";
                        }
                    }
                    if(player.isDead()){
                        state = GameState::EndGame;
                        result = GameResult::died;   
                        uiSelection = 0;
                    } else if(spawner.getEnemiesLeft() == 0){
                        state = GameState::EndGame;
                        result = GameResult::won;
                        uiSelection = 0;
                    }
                    break;
                case GameState::Shop:
                    
                    break;
                // Switch to pause
                case GameState::Paused:
                    if(event->is<sf::Event::KeyPressed>()){
                        if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Enter){
                            state = GameState::Playing;
                        }
                        if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Backspace){
                            saves[saveSelected].level = player.getLevel();
                            reset(true);
                            state = GameState::Home;
                        }
                    }
                    break;
                case GameState::EndGame:
                    if(event->is<sf::Event::KeyPressed>()){
                // Arrow usage
                if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Right){
                        uiSelection = (uiSelection+1)%3;
                    } else if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Left){
                        uiSelection = (uiSelection+3-1)%3;
                        if(uiSelection<0){
                            uiSelection = 2;
                        }
                    }
                // Select a Option
                if(event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Enter){
                    // If selecting home screen
                    if(uiSelection == 0){
                        if(result == GameResult::won){
                            player.levelUp();
                        }
                        reset(true);
                        saves[saveSelected].level = player.getLevel();
                        saves[saveSelected].exists = true;
                        player.saveToFile(saves[saveSelected].filename);
                        state = GameState::Home;
                    } else if(uiSelection == 1){
                        // selecting shop
                        state = GameState::Shop;
                    } else if(uiSelection == 2){
                            // selecting next level
                            if(result == GameResult::won){
                            reset(true);
                            player.levelUp();
                            saves[saveSelected].level = player.getLevel();
                            saves[saveSelected].exists = true;
                            player.saveToFile(saves[saveSelected].filename);
                            state = GameState::Playing;
                            // selecting play again
                            //reset(true);
                            // state = GameState::Playing;
                        }
                    } else {
                        reset(true);
                        state = GameState::Playing;
                    }
                }
            }
        }
        }
        }
    }
}
void Game::render(){
    window.clear(sf::Color::Black);

    if(state == GameState::Home){
        //Draw title and instructions
        window.draw(titleText);
        window.draw(enterText);

        // Draw Saves
        for(int i = 0; i<3; i++){
            sf::Text text(font, "Save: 1 ", 56);
            text.setFillColor(i == saveSelected ? sf::Color::Yellow : sf::Color::White);
            std::string saveStatus = saves[i].exists ? (" LEVEL: " + std::to_string(saves[i].level)) : "New Game";
            text.setString("Save: " + std::to_string(i+1) + " | " + saveStatus);
            sf::FloatRect bounds = text.getLocalBounds();
            text.setPosition({width/2.f - bounds.size.x/2.f, startY + i * spacing});
            window.draw(text);
        }
    } else if(state == GameState::StartLevel){
        
        window.draw(startLevelText);
        window.draw(homeText);
        window.draw(shopText);

        sf::Text* options[3];
        options[0] = &homeText;
        options[1] = &shopText;
        options[2] = &startLevelText;

        for (int i = 0; i<3; i++){
            options[i]->setFillColor(i == uiSelection ? sf::Color::Yellow : sf::Color::White);
            window.draw(*options[i]);
        }
        
    } else if(state == GameState::Playing){
        // Draw player and UI
        player.draw(window);
        for(auto& e : enemies) e.draw(window);
        for(auto& cpopup : coinPopups) cpopup.draw(window);
        
        window.draw(healthText);
        window.draw(staminaText);
        window.draw(coinsText);
        window.draw(levelText);
        window.draw(controlsText);
        window.draw(resetText);
        window.draw(spaceText);
        window.draw(chargeText);
        window.draw(enemyText);
    }else if(state == GameState::Shop){
    } else if(state == GameState::Paused){
        // Draw Paused Screen
        window.draw(pausedText);
        window.draw(enterText);
        window.draw(backText);
    } else if(state == GameState::EndGame){
        window.draw(homeText);
        window.draw(shopText);

        sf::Text* options[3];
        options[0] = &homeText;
        options[1] = &shopText;
        if(result == GameResult::won){
            window.draw(victoryText);
            options[2] = &nextLevelText;
        } else {
            window.draw(diedText);
            options[2] = &againText;
        }
       for(int i = 0; i<3; i++){
            options[i]->setFillColor(i == uiSelection ? sf::Color::Yellow : sf::Color::White);
            window.draw(*options[i]);
        }
    }
    window.display();
}




