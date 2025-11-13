#pragma once

#include <SFML/Graphics.hpp>

namespace Physics {
    struct CollisionResult {
        bool collision;
        sf::Vector2f correction;
        bool fromBottom;
        bool fromTop;
        bool fromLeft;
        bool fromRight;
    };
    
    CollisionResult checkCollision(const sf::Rect<float>& player, const sf::Rect<float>& platform);
    bool isOnGround(const sf::Rect<float>& player, const sf::Rect<float>& platform);
}