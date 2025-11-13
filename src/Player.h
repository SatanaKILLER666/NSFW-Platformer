#pragma once

#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    
    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window) const;
    
    void moveLeft();
    void moveRight();
    void stopMoving();
    void jump();
    
    void takeDamage(int damage);
    void heal(int amount);
    bool isDead() const;
    int getHealth() const;
    void reset();
    
    sf::Rect<float> getGlobalBounds() const;
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f position);
    
private:
    void applyPhysics(sf::Time deltaTime);
    
    sf::RectangleShape mShape;
    sf::Vector2f mVelocity;
    sf::Vector2f mAcceleration;
    
    int mHealth;
    int mMaxHealth;
    
    bool mIsOnGround;
    bool mIsMovingLeft;
    bool mIsMovingRight;
    
    static constexpr float GRAVITY = 980.f;
    static constexpr float MOVE_ACCELERATION = 800.f;
    static constexpr float JUMP_FORCE = -450.f;
    static constexpr float FRICTION = 0.85f;
    static constexpr float MAX_SPEED = 300.f;
};