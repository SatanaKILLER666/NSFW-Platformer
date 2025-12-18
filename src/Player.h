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


    
    void resetPhysics();
    
    sf::Rect<float> getGlobalBounds() const;
    sf::Vector2f getPosition() const;
    void setPosition(sf::Vector2f position);
    
    void setOnGround(bool onGround);
    bool isOnGround() const;
    void applyCorrection(const sf::Vector2f& correction);
    
private:
    sf::RectangleShape mShape;
    sf::Vector2f mVelocity;
    
    int mHealth;
    int mMaxHealth;
    
    bool mIsOnGround;
    bool mIsMovingLeft;
    bool mIsMovingRight;
    
    static constexpr float GRAVITY = 500.f;
    static constexpr float MOVE_SPEED = 200.f;
    static constexpr float JUMP_FORCE = -350.f;
};