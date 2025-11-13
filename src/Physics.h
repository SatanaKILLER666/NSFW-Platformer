#pragma once

#include <SFML/Graphics.hpp>

namespace Physics {
    bool checkCollision(const sf::Rect<float>& a, const sf::Rect<float>& b);
}