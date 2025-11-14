#include "Physics.h"
#include <algorithm>

namespace Physics {

    CollisionResult checkCollision(const sf::Rect<float>& player, const sf::Rect<float>& platform) {
        CollisionResult result{false, {0,0}, false, false, false, false};
        

        if (!player.findIntersection(platform).has_value()) {
            return result;
        }
        
        result.collision = true;
        

        float overlapLeft = (player.position.x + player.size.x) - platform.position.x;
        float overlapRight = (platform.position.x + platform.size.x) - player.position.x;
        float overlapTop = (player.position.y + player.size.y) - platform.position.y;
        float overlapBottom = (platform.position.y + platform.size.y) - player.position.y;
        

        float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});
        

        if (minOverlap == overlapTop) {

            result.correction.y = -overlapTop;
            result.fromBottom = true;
        } 
        else if (minOverlap == overlapBottom) {

            result.correction.y = overlapBottom;
            result.fromTop = true;
        }
        else if (minOverlap == overlapLeft) {

            result.correction.x = -overlapLeft;
            result.fromRight = true;
        }
        else if (minOverlap == overlapRight) {

            result.correction.x = overlapRight;
            result.fromLeft = true;
        }
        
        return result;
    }
    
    bool isOnGround(const sf::Rect<float>& player, const sf::Rect<float>& platform) {

        float playerBottom = player.position.y + player.size.y;
        float platformTop = platform.position.y;
        

        return std::abs(playerBottom - platformTop) < 5.0f;
    }
}