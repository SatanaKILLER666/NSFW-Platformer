#include "Game.h"
#include <iostream>

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game() 
    : mWindow(sf::VideoMode({1024, 768}), "SFML Platformer")
    , mCurrentLevel(0)
    , mIsPaused(false)
    , mGameOver(false)
    , mHealthText(sf::Text(mFont, "Health: 100", 24))
    , mLevelText(sf::Text(mFont, "Health: 100", 24))
{
    
    mWindow.setFramerateLimit(60);
    
    loadTextures();
    loadLevels();
    
    if (!mFont.openFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }
    
    // Инициализируем тексты после загрузки шрифта
    mHealthText = sf::Text(mFont, "Health: 100", 24);
    mHealthText.setFillColor(sf::Color::White);
    mHealthText.setPosition({10.f, 10.f});
    
    mLevelText = sf::Text(mFont, "Level: 1", 24);
    mLevelText.setFillColor(sf::Color::White);
    mLevelText.setPosition({10.f, 40.f});
}

void Game::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    
    while (mWindow.isOpen()) {
        sf::Time deltaTime = clock.restart();
        timeSinceLastUpdate += deltaTime;
        
        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;
            
            processEvents();
            if (!mIsPaused && !mGameOver) {
                update(TimePerFrame);
            }
        }
        
        render();
    }
}

void Game::processEvents() {
    while (std::optional<sf::Event> event = mWindow.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            mWindow.close();
        }
        
        if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            if (keyEvent->scancode == sf::Keyboard::Scan::Escape) {
                mWindow.close();
            }
            if (keyEvent->scancode == sf::Keyboard::Scan::P) {
                mIsPaused = !mIsPaused;
            }
            if (keyEvent->scancode == sf::Keyboard::Scan::R && mGameOver) {
                mGameOver = false;
                mCurrentLevel = 0;
                if (mPlayer) {
                    mPlayer->reset();
                    mPlayer->setPosition(mLevels[mCurrentLevel].getStartPosition());
                }
            }
        }
    }
}

void Game::update(sf::Time deltaTime) {
    handleInput();
    
    if (mPlayer) {
        mPlayer->update(deltaTime);
        mLevels[mCurrentLevel].update(deltaTime, *mPlayer);
        
        if (mLevels[mCurrentLevel].isCompleted()) {
            mCurrentLevel++;
            if (mCurrentLevel >= mLevels.size()) {
                mGameOver = true;
            } else {
                mPlayer->setPosition(mLevels[mCurrentLevel].getStartPosition());
            }
        }
        
        if (mPlayer->isDead()) {
            mGameOver = true;
        }
    }
    
    if (mPlayer) {
        mHealthText.setString("Health: " + std::to_string(mPlayer->getHealth()));
    }
    mLevelText.setString("Level: " + std::to_string(mCurrentLevel + 1));
}

void Game::render() {
    mWindow.clear(sf::Color(135, 206, 235));
    
    if (mPlayer) {
        mLevels[mCurrentLevel].draw(mWindow);
        mPlayer->draw(mWindow);
    }
    
    mWindow.draw(mHealthText);
    mWindow.draw(mLevelText);
    
    if (mIsPaused) {
        sf::Text pauseText(mFont, "PAUSED", 48);
        pauseText.setPosition({400.f, 300.f});
        pauseText.setFillColor(sf::Color::White);
        mWindow.draw(pauseText);
    }
    
    if (mGameOver) {
        sf::Text gameOverText(mFont, "GAME OVER\nPress R to restart", 48);
        gameOverText.setPosition({350.f, 300.f});
        gameOverText.setFillColor(sf::Color::Red);
        mWindow.draw(gameOverText);
    }
    
    mWindow.display();
}

void Game::handleInput() {
    if (!mPlayer) return;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left)) {
        mPlayer->moveLeft();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D) || 
               sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)) {
        mPlayer->moveRight();
    } else {
        mPlayer->stopMoving();
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up)) {
        mPlayer->jump();
    }
}

void Game::loadTextures() {
    std::cout << "Textures loaded (placeholder)" << std::endl;
}

void Game::loadLevels() {
    mLevels.clear();
    
    // Level 1 - персонаж спавнится НА платформе
    Level level1;
    level1.addPlatform(sf::Rect<float>({0, 700}, {1024, 68})); // Пол
    level1.addPlatform(sf::Rect<float>({100, 600}, {200, 20})); // Стартовая платформа
    level1.addPlatform(sf::Rect<float>({400, 550}, {100, 20}));
    level1.addPlatform(sf::Rect<float>({600, 500}, {100, 20}));
    level1.setStartPosition({150, 550}); // Спавн НА платформе
    level1.setGoal(sf::Rect<float>({800, 450}, {50, 50}));
    mLevels.push_back(level1);
    
    // Level 2
    Level level2;
    level2.addPlatform(sf::Rect<float>({0, 700}, {1024, 68}));
    level2.addPlatform(sf::Rect<float>({100, 600}, {200, 20}));
    level2.addPlatform(sf::Rect<float>({350, 550}, {100, 20}));
    level2.addPlatform(sf::Rect<float>({550, 500}, {100, 20}));
    level2.addPlatform(sf::Rect<float>({750, 550}, {100, 20}));
    level2.setStartPosition({150, 550});
    level2.setGoal(sf::Rect<float>({800, 450}, {50, 50}));
    mLevels.push_back(level2);
    
    mPlayer = std::make_unique<Player>();
    mPlayer->setPosition(mLevels[mCurrentLevel].getStartPosition());
}