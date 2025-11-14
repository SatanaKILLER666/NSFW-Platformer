#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Player.h"
#include "Level.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void handleInput();
    void loadTextures();
    void loadLevels();
    

    sf::RenderWindow mWindow;
    std::unique_ptr<Player> mPlayer;
    std::vector<Level> mLevels;
    size_t mCurrentLevel;
    
    sf::Font mFont;
    sf::Text mHealthText;
    sf::Text mLevelText;

    sf::Texture mBackgroundTexture;

    std::unique_ptr<sf::Sprite> mBackground; 
    
    bool mIsPaused;
    bool mGameOver;
    bool mGameWon;
    
    static const sf::Time TimePerFrame;
};