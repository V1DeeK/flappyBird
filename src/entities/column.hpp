#pragma once
#include <SFML/Graphics.hpp>

class Column {
public:
    Column(float x, float gapY);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getTopBounds() const;
    sf::FloatRect getBottomBounds() const;

    float getX() const;
    float getGapY() const { return gapY; }
    bool isPassed() const { return passed; }
    void markPassed() { passed = true; }

private:
    sf::RectangleShape topPipe;
    sf::RectangleShape bottomPipe;
    float gapY;
    mutable bool passed = false;
};
