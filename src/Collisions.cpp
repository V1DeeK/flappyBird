#include "Collisions.hpp"
#include "entities/bird.hpp"
#include "entities/column.hpp"
#include "Constants.hpp"

namespace Collisions {
    bool checkBirdPipeCollision(const Bird& bird, const std::vector<Column>& columns) {
        sf::FloatRect birdBounds = bird.getBounds();
        
        // Check if bird bounds are valid
        if (birdBounds.size.x <= 0 || birdBounds.size.y <= 0) {
            return false;
        }
        
        // Create a smaller rectangle for more forgiving collision
        float collisionScale = Constants::COLLISION_SCALE;
        float widthReduction = birdBounds.size.x * (1.0f - collisionScale) / 2.0f;
        float heightReduction = birdBounds.size.y * (1.0f - collisionScale) / 2.0f;
        
        sf::FloatRect collisionBounds(
            sf::Vector2f(birdBounds.position.x + widthReduction, birdBounds.position.y + heightReduction),
            sf::Vector2f(birdBounds.size.x * collisionScale, birdBounds.size.y * collisionScale)
        );
        
        for (const auto& column : columns) {
            float pipeX = column.getX();
            float pipeRight = pipeX + Constants::PIPE_WIDTH;
            
            // Оптимизация: проверяем только трубы, которые находятся на экране или близко к птице
            // Пропускаем трубы, которые еще не появились (справа от экрана)
            if (pipeRight < 0) {
                continue; // Труба еще не появилась на экране
            }
            
            // Пропускаем трубы, которые уже прошли (далеко слева от птицы)
            // Проверяем только трубы в зоне видимости птицы
            if (pipeX > Constants::WINDOW_WIDTH + Constants::PIPE_COLLISION_OFFSET) {
                continue; // Труба уже далеко позади
            }
            
            sf::FloatRect topBounds = column.getTopBounds();
            sf::FloatRect bottomBounds = column.getBottomBounds();
            
            // Check if pipe bounds are valid
            if (topBounds.size.x <= 0 || topBounds.size.y <= 0 ||
                bottomBounds.size.x <= 0 || bottomBounds.size.y <= 0) {
                continue;
            }
            
            // Check if pipe reached bird on X axis (труба в зоне птицы)
            bool pipeReachedBird = (
                topBounds.position.x < collisionBounds.position.x + collisionBounds.size.x &&
                topBounds.position.x + topBounds.size.x > collisionBounds.position.x &&
                topBounds.position.x + topBounds.size.x >= Constants::BIRD_X_POSITION - Constants::PIPE_COLLISION_OFFSET
            );
            
            if (pipeReachedBird) {
                // Check collision with top pipe (AABB)
                bool topCollision = (
                    collisionBounds.position.x < topBounds.position.x + topBounds.size.x &&
                    collisionBounds.position.x + collisionBounds.size.x > topBounds.position.x &&
                    collisionBounds.position.y < topBounds.position.y + topBounds.size.y &&
                    collisionBounds.position.y + collisionBounds.size.y > topBounds.position.y
                );
                
                // Check collision with bottom pipe (AABB)
                bool bottomCollision = (
                    collisionBounds.position.x < bottomBounds.position.x + bottomBounds.size.x &&
                    collisionBounds.position.x + collisionBounds.size.x > bottomBounds.position.x &&
                    collisionBounds.position.y < bottomBounds.position.y + bottomBounds.size.y &&
                    collisionBounds.position.y + collisionBounds.size.y > bottomBounds.position.y
                );
                
                if (topCollision || bottomCollision) {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    bool checkBirdBoundaryCollision(const Bird& bird) {
        if (!bird.hasStarted()) {
            return false;
        }
        
        sf::FloatRect birdBounds = bird.getBounds();
        
        // Check if bird bounds are valid
        if (birdBounds.size.x <= 0 || birdBounds.size.y <= 0) {
            return false;
        }
        
        // Check top and bottom boundaries with margin
        bool hitTop = birdBounds.position.y < -Constants::BOUNDARY_COLLISION_MARGIN;
        bool hitBottom = birdBounds.position.y + birdBounds.size.y > Constants::WINDOW_HEIGHT + Constants::BOUNDARY_COLLISION_MARGIN;
        
        return hitTop || hitBottom;
    }
    
    bool checkBirdPassedColumn(const Bird& bird, const Column& column) {
        if (column.isPassed()) {
            return false; // Already passed
        }
        
        sf::FloatRect birdBounds = bird.getBounds();
        
        // Check if bird bounds are valid
        if (birdBounds.size.x <= 0 || birdBounds.size.y <= 0) {
            return false;
        }
        
        // If bird's X is greater than column's X + width, bird passed
        return birdBounds.position.x > column.getX() + Constants::PIPE_WIDTH;
    }
}

