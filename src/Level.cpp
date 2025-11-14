#include "Level.h"
#include "Player.h"
#include "Physics.h"
#include <fstream>
#include <sstream>
#include <iostream>

Level::Level() 
    : mCompleted(false)
    , mStartPosition(100.f, 500.f) {
}

bool Level::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to load level: " << filename << std::endl;
        return false;
    }
    
    mPlatforms.clear();
    std::string line;
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        
        if (type == "platform") {
            float x, y, width, height;
            iss >> x >> y >> width >> height;
            
            Platform platform;
            platform.bounds = sf::Rect<float>({x, y}, {width, height});
            platform.color = sf::Color::Green;
            platform.isDeadly = false;
            platform.isGoal = false;
            mPlatforms.push_back(platform);
        }
        else if (type == "start") {
            float x, y;
            iss >> x >> y;
            mStartPosition = {x, y};
        }
        else if (type == "spike") {
            float x, y, width, height;
            iss >> x >> y >> width >> height;
            
            Platform spike;
            spike.bounds = sf::Rect<float>({x, y}, {width, height});
            spike.color = sf::Color::Red;
            spike.isDeadly = true;
            spike.isGoal = false;
            mPlatforms.push_back(spike);
        }
        else if (type == "goal") {
            float x, y, width, height;
            iss >> x >> y >> width >> height;
            
            Platform goal;
            goal.bounds = sf::Rect<float>({x, y}, {width, height});
            goal.color = sf::Color::Yellow;
            goal.isDeadly = false;
            goal.isGoal = true;
            mPlatforms.push_back(goal);
        }
    }
    
    file.close();
    return true;
}

void Level::update(sf::Time deltaTime, Player& player) {
    checkCollisions(player);
}

void Level::draw(sf::RenderWindow& window) const {
    for (const auto& platform : mPlatforms) {
        sf::RectangleShape shape;
        shape.setPosition(platform.bounds.position);
        shape.setSize(platform.bounds.size);
        shape.setFillColor(platform.color);
        window.draw(shape);
    }
}

void Level::checkCollisions(Player& player) {
    sf::Rect<float> playerBounds = player.getGlobalBounds();
    bool onGround = false;
    
    for (const auto& platform : mPlatforms) {
        Physics::CollisionResult collision = Physics::checkCollision(playerBounds, platform.bounds);
        
        if (collision.collision) {
            if (platform.isDeadly) {
                player.takeDamage(25);
            } else if (platform.isGoal) {
                mCompleted = true;
            } else {

                player.applyCorrection(collision.correction);
                

                if (collision.fromBottom) {
                    onGround = true;
                }
            }
        }
    }
    
    player.setOnGround(onGround);
}

bool Level::isCompleted() const {
    return mCompleted;
}

sf::Vector2f Level::getStartPosition() const {
    return mStartPosition;
}

void Level::addPlatform(const sf::Rect<float>& bounds) {
    Platform platform;
    platform.bounds = bounds;
    platform.color = sf::Color::Green;
    platform.isDeadly = false;
    platform.isGoal = false;
    mPlatforms.push_back(platform);
}


void Level::addSpike(const sf::Rect<float>& bounds) {
    Platform spike;
    spike.bounds = bounds;
    spike.color = sf::Color::Red;
    spike.isDeadly = true;
    spike.isGoal = false;
    mPlatforms.push_back(spike);
}

void Level::setStartPosition(const sf::Vector2f& position) {
    mStartPosition = position;
}

void Level::setGoal(const sf::Rect<float>& goalBounds) {
    Platform goal;
    goal.bounds = goalBounds;
    goal.color = sf::Color::Yellow;
    goal.isDeadly = false;
    goal.isGoal = true;
    mPlatforms.push_back(goal);
}