#include "Collisions.hpp"
#include "entities/bird.hpp"
#include "entities/column.hpp"
#include "Constants.hpp"

namespace Collisions {
    bool checkBirdPipeCollision(const Bird& bird, const std::vector<Column>& columns) {
        sf::FloatRect birdBounds = bird.getBounds();
        
        if (birdBounds.size.x <= 0 || birdBounds.size.y <= 0) {
            return false;
        }
        
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
            
            if (pipeRight < 0) {
                continue;
            }
            
            if (pipeX > Constants::WINDOW_WIDTH + Constants::PIPE_COLLISION_OFFSET) {
                continue;
            }
            
            sf::FloatRect topBounds = column.getTopBounds();
            sf::FloatRect bottomBounds = column.getBottomBounds();
            
            if (topBounds.size.x <= 0 || topBounds.size.y <= 0 ||
                bottomBounds.size.x <= 0 || bottomBounds.size.y <= 0) {
                continue;
            }
            
            bool pipeReachedBird = (
                topBounds.position.x < collisionBounds.position.x + collisionBounds.size.x &&
                topBounds.position.x + topBounds.size.x > collisionBounds.position.x &&
                topBounds.position.x + topBounds.size.x >= Constants::BIRD_X_POSITION - Constants::PIPE_COLLISION_OFFSET
            );
            
            if (pipeReachedBird) {
                bool topCollision = (
                    collisionBounds.position.x < topBounds.position.x + topBounds.size.x &&
                    collisionBounds.position.x + collisionBounds.size.x > topBounds.position.x &&
                    collisionBounds.position.y < topBounds.position.y + topBounds.size.y &&
                    collisionBounds.position.y + collisionBounds.size.y > topBounds.position.y
                );
                
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
        
        if (birdBounds.size.x <= 0 || birdBounds.size.y <= 0) {
            return false;
        }
        
        bool hitTop = birdBounds.position.y < -Constants::BOUNDARY_COLLISION_MARGIN;
        bool hitBottom = birdBounds.position.y + birdBounds.size.y > Constants::WINDOW_HEIGHT + Constants::BOUNDARY_COLLISION_MARGIN;
        
        return hitTop || hitBottom;
    }
    
    bool checkBirdPassedColumn(const Bird& bird, const Column& column) {
        if (column.isPassed()) {
            return false;
        }
        
        sf::FloatRect birdBounds = bird.getBounds();
        
        if (birdBounds.size.x <= 0 || birdBounds.size.y <= 0) {
            return false;
        }
        
        return birdBounds.position.x > column.getX() + Constants::PIPE_WIDTH;
    }
}
