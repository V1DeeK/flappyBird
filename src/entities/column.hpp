#pragma once
#include <SFML/Graphics.hpp>

class Column {
public:
    Column(float x, float gapY); // x — позиция по горизонтали, gapY — центр дыры

    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getTopBounds() const;
    sf::FloatRect getBottomBounds() const;

    float getX() const;
    float getGapY() const { return gapY; } // Возвращаем центр дыры для размещения монеток
    bool isPassed() const { return passed; }
    void markPassed() { passed = true; }

private:
    sf::RectangleShape topPipe;
    sf::RectangleShape bottomPipe;
    float gapY; // Сохраняем центр дыры для размещения монеток
    mutable bool passed = false;  // mutable чтобы можно было изменять в const методах
};