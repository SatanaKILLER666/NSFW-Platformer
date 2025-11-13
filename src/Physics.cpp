#include "Physics.h"
#include <algorithm>

namespace Physics {
    CollisionResult checkCollision(const sf::Rect<float>& player, const sf::Rect<float>& platform) {
        CollisionResult result{false, {0,0}, false, false, false, false};
        
        // Проверяем пересечение
        if (!player.findIntersection(platform).has_value()) {
            return result;
        }
        
        result.collision = true;
        
        // Вычисляем перекрытия по всем сторонам
        float overlapLeft = (player.position.x + player.size.x) - platform.position.x;
        float overlapRight = (platform.position.x + platform.size.x) - player.position.x;
        float overlapTop = (player.position.y + player.size.y) - platform.position.y;
        float overlapBottom = (platform.position.y + platform.size.y) - player.position.y;
        
        // Находим минимальное перекрытие
        float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});
        
        // Определяем направление столкновения и корректируем позицию
        if (minOverlap == overlapTop) {
            // Столкновение сверху (стоим на платформе)
            result.correction.y = -overlapTop;
            result.fromBottom = true;
        } 
        else if (minOverlap == overlapBottom) {
            // Столкновение снизу (ударились головой)
            result.correction.y = overlapBottom;
            result.fromTop = true;
        }
        else if (minOverlap == overlapLeft) {
            // Столкновение слева
            result.correction.x = -overlapLeft;
            result.fromRight = true;
        }
        else if (minOverlap == overlapRight) {
            // Столкновение справа
            result.correction.x = overlapRight;
            result.fromLeft = true;
        }
        
        return result;
    }
    
    bool isOnGround(const sf::Rect<float>& player, const sf::Rect<float>& platform) {
        // Проверяем, стоит ли игрок на платформе
        float playerBottom = player.position.y + player.size.y;
        float platformTop = platform.position.y;
        
        // Если низ игрока близко к верху платформы и игрок движется вниз
        return std::abs(playerBottom - platformTop) < 5.0f;
    }
}