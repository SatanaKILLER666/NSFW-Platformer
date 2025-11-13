#include "Physics.h"

namespace Physics {
    bool checkCollision(const sf::Rect<float>& a, const sf::Rect<float>& b) {
        return a.findIntersection(b).has_value();
    }
}