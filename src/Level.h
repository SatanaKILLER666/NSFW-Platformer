#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Player;

class Level {
public:
    Level();
    
    bool loadFromFile(const std::string& filename);
    void update(sf::Time deltaTime, Player& player);
    void draw(sf::RenderWindow& window) const;
    
    bool isCompleted() const;
    sf::Vector2f getStartPosition() const;
    
    void addPlatform(const sf::Rect<float>& bounds);
    void setStartPosition(const sf::Vector2f& position);
    void setGoal(const sf::Rect<float>& goalBounds);
    
private:
    void checkCollisions(Player& player);
    
    struct Platform {
        sf::Rect<float> bounds;
        sf::Color color;
        bool isDeadly;
        bool isGoal;
    };
    
    std::vector<Platform> mPlatforms;
    sf::Vector2f mStartPosition;
    bool mCompleted;
};