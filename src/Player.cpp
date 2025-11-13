#include "Player.h"
#include <iostream>

Player::Player() 
    : mVelocity(0.f, 0.f)
    , mAcceleration(0.f, 0.f)
    , mHealth(100)
    , mMaxHealth(100)
    , mIsOnGround(false)
    , mIsMovingLeft(false)
    , mIsMovingRight(false) {
    
    mShape.setSize({32.f, 64.f});
    mShape.setFillColor(sf::Color::Blue);
    mShape.setOrigin({16.f, 32.f});
}

void Player::update(sf::Time deltaTime) {
    mAcceleration.x = 0.f;
    
    if (mIsMovingLeft) {
        mAcceleration.x = -MOVE_ACCELERATION;
    } else if (mIsMovingRight) {
        mAcceleration.x = MOVE_ACCELERATION;
    }
    
    mAcceleration.y = GRAVITY;
    applyPhysics(deltaTime);
}

void Player::applyPhysics(sf::Time deltaTime) {
    float delta = deltaTime.asSeconds();
    
    mVelocity += mAcceleration * delta;
    
    if (mIsOnGround && mAcceleration.x == 0.f) {
        mVelocity.x *= FRICTION;
    }
    
    if (std::abs(mVelocity.x) > MAX_SPEED) {
        mVelocity.x = (mVelocity.x > 0) ? MAX_SPEED : -MAX_SPEED;
    }
    
    mShape.move(mVelocity * delta);
    mIsOnGround = false;
}

void Player::draw(sf::RenderWindow& window) const {
    window.draw(mShape);
}

void Player::moveLeft() {
    mIsMovingLeft = true;
    mIsMovingRight = false;
}

void Player::moveRight() {
    mIsMovingRight = true;
    mIsMovingLeft = false;
}

void Player::stopMoving() {
    mIsMovingLeft = false;
    mIsMovingRight = false;
}

void Player::jump() {
    if (mIsOnGround) {
        mVelocity.y = JUMP_FORCE;
        mIsOnGround = false;
    }
}

void Player::takeDamage(int damage) {
    mHealth -= damage;
    if (mHealth < 0) mHealth = 0;
    mShape.setFillColor(sf::Color::Red);
}

void Player::heal(int amount) {
    mHealth += amount;
    if (mHealth > mMaxHealth) mHealth = mMaxHealth;
}

bool Player::isDead() const {
    return mHealth <= 0;
}

int Player::getHealth() const {
    return mHealth;
}

void Player::reset() {
    mHealth = mMaxHealth;
    mVelocity = {0.f, 0.f};
    mShape.setFillColor(sf::Color::Blue);
}

sf::Rect<float> Player::getGlobalBounds() const {
    return mShape.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const {
    return mShape.getPosition();
}

void Player::setPosition(sf::Vector2f position) {
    mShape.setPosition(position);
}