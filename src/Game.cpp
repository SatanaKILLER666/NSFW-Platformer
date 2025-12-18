#include "Game.h"
#include <iostream>

const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game() 
    : mWindow(sf::VideoMode({1024, 768}), "SFML Platformer")
    , mCurrentLevel(0)
    , mIsPaused(false)
    , mGameOver(false)
    , mGameWon(false)
    , mHealthText(sf::Text(mFont,L"Здоровье: 100", 24))
    , mLevelText(sf::Text(mFont,L"Здоровье: 100", 24))
    , mBackground(nullptr)
{
    
    mWindow.setFramerateLimit(60);
    
    loadTextures();
    loadLevels();
    
    if (!mFont.openFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }
    

    mHealthText = sf::Text(mFont, L"Здоровье: 100", 24);
    mHealthText.setFillColor(sf::Color::White);
    mHealthText.setPosition({10.f, 10.f});
    
    mLevelText = sf::Text(mFont, L"Уровень: 1", 24);
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
            if (keyEvent->scancode == sf::Keyboard::Scan::R) {


                mGameOver = false;
                mCurrentLevel = 0;
                
                for (auto& lvl : mLevels) {
                    lvl.reset();
                }
                if (mPlayer) {
                    mPlayer->reset();
                    if (mCurrentLevel < mLevels.size()) {
                        mPlayer->setPosition(mLevels[mCurrentLevel].getStartPosition());
                    }
                }
            }
        }
    }
}

void Game::update(sf::Time deltaTime) {
    handleInput();
    
    if (mPlayer) {
        mPlayer->update(deltaTime);
        

        if (mCurrentLevel < mLevels.size()) {
            mLevels[mCurrentLevel].update(deltaTime, *mPlayer);
            

            if (mLevels[mCurrentLevel].isCompleted()) {
    
                mCurrentLevel++;

      
                if (mCurrentLevel >= mLevels.size()) {
                    mGameOver = true;
                } else {
             

                    mPlayer->resetPhysics();
                    mPlayer->setPosition(mLevels[mCurrentLevel].getStartPosition());
                }
            }
            

            if (mPlayer->isDead()) {
                mGameOver = true;
            }
        }
    }
    

    if (mPlayer) {
        mHealthText.setString(L"Здоровье: " + std::to_wstring(mPlayer->getHealth()));
    }
    

    if (mCurrentLevel < mLevels.size()) {
        mLevelText.setString(L"Уровень: " + std::to_wstring(mCurrentLevel + 1));
    } else {
        mLevelText.setString(L"Уровень: Победа!");
    }
}

void Game::render() {
    mWindow.clear(sf::Color(135, 206, 235));

    mWindow.draw(*mBackground);
    

    if (mPlayer && mCurrentLevel < mLevels.size()) {
        mLevels[mCurrentLevel].draw(mWindow);
        mPlayer->draw(mWindow);
    }
    
    mWindow.draw(mHealthText);
    mWindow.draw(mLevelText);
    
    if (mIsPaused) {
        sf::Text pauseText(mFont, L"Игра на паузе", 48);
        pauseText.setPosition({400.f, 300.f});
        pauseText.setFillColor(sf::Color::White);
        mWindow.draw(pauseText);
    }
    
    if (mGameOver) {

        if (mCurrentLevel >= mLevels.size()) {

            sf::Text winText(mFont, L"ПОБЕДА!!!\n\nТы прошёл все уровни!\nТы - настоящий Dungeon Master!", 36);
            winText.setPosition({200.f, 200.f});
            winText.setFillColor(sf::Color::Green);
            

            sf::RectangleShape victoryBg({800.f, 300.f});
            victoryBg.setPosition({112.f, 150.f});
            victoryBg.setFillColor(sf::Color(0, 0, 0, 200));
            victoryBg.setOutlineColor(sf::Color::Yellow);
            victoryBg.setOutlineThickness(5.f);
            
            mWindow.draw(victoryBg);
            mWindow.draw(winText);
        } else {

            sf::Text gameOverText(mFont, L"Ты проиграл!:( НУБЯРА!!!\n\nНажми R, чтоб начать сначала", 48);
            gameOverText.setPosition({150.f, 300.f});
            gameOverText.setFillColor(sf::Color::Red);
            mWindow.draw(gameOverText);
        }
    }
    

    if (mCurrentLevel >= mLevels.size() && !mGameOver) {
        sf::Text finalWinText(mFont, L"УРА!!!\n\nТы - настоящий Dungeon Master!\nТы прошёл все уровни!\n\nНажми ESC, чтобы выйти", 42);
        finalWinText.setPosition({150.f, 250.f});
        finalWinText.setFillColor(sf::Color::Cyan);
        

        static float hue = 0.f;
        hue += 0.5f;
        if (hue > 360.f) hue = 0.f;
        
        sf::RectangleShape victoryBackground({1024.f, 768.f});
        victoryBackground.setFillColor(sf::Color::Black);
        mWindow.draw(victoryBackground);
        

        mWindow.draw(finalWinText);
        

        if (mPlayer) {
            sf::Text statsText(mFont, L"Финальное здоровье: " + std::to_wstring(mPlayer->getHealth()), 32);
            statsText.setPosition({400.f, 450.f});
            statsText.setFillColor(sf::Color::White);
            mWindow.draw(statsText);
        }
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

    if (!mBackgroundTexture.loadFromFile("assets/textures/background.jpg")) {
        std::cerr << "Failed to load background texture! Using default blue background." << std::endl;
        return;
    }
    

    mBackground = std::make_unique<sf::Sprite>(mBackgroundTexture);
    

    sf::Vector2f targetSize(mWindow.getSize().x, mWindow.getSize().y);
    mBackground->setScale(
        {targetSize.x / mBackgroundTexture.getSize().x,
        targetSize.y / mBackgroundTexture.getSize().y}
    );
    
    std::cout << "Background texture loaded successfully!" << std::endl;
}

void Game::loadLevels() {
    mLevels.clear();
    
    // Level 1 - с шипами
    Level level1;
    level1.addPlatform(sf::Rect<float>({0, 700}, {1024, 68}));
    level1.addPlatform(sf::Rect<float>({100, 600}, {200, 20}));
    level1.addPlatform(sf::Rect<float>({400, 550}, {100, 20}));
    level1.addPlatform(sf::Rect<float>({600, 500}, {100, 20}));
    level1.addSpike(sf::Rect<float>({300, 680}, {50, 20}));
    level1.addSpike(sf::Rect<float>({500, 680}, {50, 20}));
    level1.addSpike(sf::Rect<float>({700, 680}, {50, 20}));
    level1.setStartPosition({150, 550});
    level1.setGoal(sf::Rect<float>({800, 450}, {50, 50}));
    mLevels.push_back(level1);
    
    // 2 уровень
    Level level2;
    level2.addPlatform(sf::Rect<float>({0, 700}, {1024, 68}));
    level2.addPlatform(sf::Rect<float>({100, 600}, {200, 20}));
    level2.addPlatform(sf::Rect<float>({350, 550}, {100, 20}));
    level2.addPlatform(sf::Rect<float>({550, 500}, {100, 20}));
    level2.addPlatform(sf::Rect<float>({750, 550}, {100, 20}));
    level2.addSpike(sf::Rect<float>({320, 680}, {80, 20}));
    level2.addSpike(sf::Rect<float>({520, 680}, {80, 20}));
    level2.addSpike(sf::Rect<float>({650, 680}, {80, 20}));
    level2.addSpike(sf::Rect<float>({780, 680}, {80, 20}));
    level2.setStartPosition({150, 550});
    level2.setGoal(sf::Rect<float>({800, 450}, {50, 50}));
    mLevels.push_back(level2);
    
    // 3 уровень - АДСКИЙ ХАРДКОРИЩЕ!!! 😈
    Level level3;
    
    level3.addPlatform(sf::Rect<float>({0, 700}, {100, 68}));
    level3.addSpike(sf::Rect<float>({100, 700}, {924, 68}));
    

    level3.addPlatform(sf::Rect<float>({50, 600}, {80, 20}));
    

    level3.addPlatform(sf::Rect<float>({200, 650}, {30, 20}));
    level3.addSpike(sf::Rect<float>({150, 680}, {50, 20}));
    level3.addSpike(sf::Rect<float>({230, 680}, {50, 20}));
    

    level3.addPlatform(sf::Rect<float>({300, 600}, {25, 20}));
    level3.addSpike(sf::Rect<float>({280, 680}, {40, 20}));
    level3.addSpike(sf::Rect<float>({325, 680}, {40, 20}));
    

    level3.addPlatform(sf::Rect<float>({400, 550}, {30, 20}));
    level3.addSpike(sf::Rect<float>({380, 680}, {40, 20}));
    

    level3.addPlatform(sf::Rect<float>({500, 620}, {25, 20}));
    level3.addSpike(sf::Rect<float>({480, 680}, {40, 20}));
    level3.addSpike(sf::Rect<float>({525, 680}, {40, 20}));
    

    level3.addPlatform(sf::Rect<float>({600, 580}, {20, 20}));
    level3.addSpike(sf::Rect<float>({580, 680}, {40, 20}));
    level3.addSpike(sf::Rect<float>({620, 680}, {40, 20}));
    

    level3.addPlatform(sf::Rect<float>({700, 540}, {25, 20}));
    level3.addSpike(sf::Rect<float>({680, 680}, {40, 20}));
    

    level3.addPlatform(sf::Rect<float>({800, 500}, {20, 20}));
    level3.addSpike(sf::Rect<float>({780, 680}, {40, 20}));
    level3.addSpike(sf::Rect<float>({820, 680}, {40, 20}));
    

    level3.addSpike(sf::Rect<float>({0, 400}, {20, 200}));
    level3.addSpike(sf::Rect<float>({1004, 400}, {20, 200}));
    

    level3.addSpike(sf::Rect<float>({250, 650}, {100, 20}));
    level3.addSpike(sf::Rect<float>({450, 600}, {100, 20}));
    level3.addSpike(sf::Rect<float>({650, 650}, {100, 20}));
    

    level3.addSpike(sf::Rect<float>({350, 520}, {50, 20}));
    level3.addSpike(sf::Rect<float>({550, 480}, {50, 20}));
    level3.addSpike(sf::Rect<float>({750, 440}, {50, 20}));
    

    level3.setStartPosition({80, 550});
    level3.setGoal(sf::Rect<float>({900, 460}, {30, 30}));
    
    mLevels.push_back(level3);
    
    mPlayer = std::make_unique<Player>();
    mPlayer->setPosition(mLevels[mCurrentLevel].getStartPosition());
}