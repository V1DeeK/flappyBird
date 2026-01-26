#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Bird;
class Column;

namespace Collisions {
    // Check collision between bird and columns
    // Returns true if collision detected
    bool checkBirdPipeCollision(const Bird& bird, const std::vector<Column>& columns);
    
    // Check collision between bird and screen boundaries (top/bottom)
    // Returns true if collision detected
    bool checkBirdBoundaryCollision(const Bird& bird);
    
    // Check if bird passed a column (for scoring)
    // Returns true if bird passed the column
    bool checkBirdPassedColumn(const Bird& bird, const Column& column);
}

