#include "Player.h"
#include <iostream>

Player::Player() 
    : mVelocity(0.f, 0.f)
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
    float delta = deltaTime.asSeconds();
    

    static sf::Time damageTimer = sf::Time::Zero;
    if (mShape.getFillColor() == sf::Color::Red) {
        damageTimer += deltaTime;
        if (damageTimer > sf::seconds(0.3f)) {
            mShape.setFillColor(sf::Color::Blue);
            damageTimer = sf::Time::Zero;
        }
    }
    

    if (!mIsOnGround) {
        mVelocity.y += GRAVITY * delta;
    }
    

    mVelocity.x = 0.f;
    if (mIsMovingLeft) {
        mVelocity.x = -MOVE_SPEED;
    } else if (mIsMovingRight) {
        mVelocity.x = MOVE_SPEED;
    }
    

    mShape.move(mVelocity * delta);
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
    

    mVelocity.y = -150.f;
    mIsOnGround = false;
    
    std::cout << "Player took " << damage << " damage! Health: " << mHealth << std::endl;
}

void Player::heal(int amount) {
    mHealth += amount;
    if (mHealth > mMaxHealth) mHealth = mMaxHealth;
    mShape.setFillColor(sf::Color::Blue);
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
    mIsOnGround = false;
    mIsMovingLeft = false;
    mIsMovingRight = false;
}

void Player::resetPhysics() {
    mVelocity = {0.f, 0.f};
    mIsOnGround = false;
    mIsMovingLeft = false;
    mIsMovingRight = false;
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

void Player::setOnGround(bool onGround) {
    mIsOnGround = onGround;
    if (mIsOnGround) {
        mVelocity.y = 0.f;
    }
}

bool Player::isOnGround() const {
    return mIsOnGround;
}

void Player::applyCorrection(const sf::Vector2f& correction) {
    mShape.move(correction);
}

static constexpr float GRAVITY = 600.f;
static constexpr float MOVE_SPEED = 180.f;
static constexpr float JUMP_FORCE = -380.f;