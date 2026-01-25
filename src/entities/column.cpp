#include "column.hpp"
#include "../Constants.hpp"
#include <iostream>
#include <vector>

Column::Column(float x, float gapY)
    : gapY(gapY)
{
    // Загружаем текстуры труб
    static sf::Texture pipeDownTexture;
    static sf::Texture pipeUpTexture;
    static bool texturesLoaded = false;
    
    if (!texturesLoaded) {
        std::vector<std::string> pathsDown = {"../assets/textures/PipeDown.png", "assets/textures/PipeDown.png"};
        std::vector<std::string> pathsUp = {"../assets/textures/PipeUp.png", "assets/textures/PipeUp.png"};
        
        bool loadedDown = false, loadedUp = false;
        
        for (const auto& path : pathsDown) {
            if (pipeDownTexture.loadFromFile(path)) {
                loadedDown = true;
                break;
            }
        }
        for (const auto& path : pathsUp) {
            if (pipeUpTexture.loadFromFile(path)) {
                loadedUp = true;
                break;
            }
        }
        
        if (loadedDown && loadedUp) {
            texturesLoaded = true;
        }
    }
    
    float gapHeight = Constants::COLUMN_GAP;
    float pipeWidth = 80.0f;
    
    // Верхняя труба (PipeDown)
    // gapY - это центр дыры между трубами
    // Верхняя труба идет от верха экрана (Y=0) до начала дыры
    float gapTop = gapY - gapHeight / 2.0f; // Верх дыры
    float topHeight = gapTop; // Высота верхней трубы = расстояние от верха до верха дыры
    // Ограничиваем минимальную высоту верхней трубы (чтобы не было слишком маленьких дыр)
    if (topHeight < 100.0f) {
        topHeight = 100.0f;
        gapTop = 100.0f;
        // Пересчитываем gapY, чтобы дыра была правильного размера
        gapY = gapTop + gapHeight / 2.0f;
    }
    topPipe.setSize(sf::Vector2f(pipeWidth, topHeight));
    topPipe.setPosition(sf::Vector2f(x, 0.0f));
    if (texturesLoaded) {
        topPipe.setTexture(&pipeDownTexture);
    } else {
        topPipe.setFillColor(sf::Color::Green);
    }
    
    // Нижняя труба (PipeUp)
    float bottomY = gapY + gapHeight / 2.0f;
    float bottomHeight = Constants::WINDOW_HEIGHT - bottomY;
    // Ограничиваем минимальную высоту нижней трубы
    if (bottomHeight < 100.0f) {
        bottomHeight = 100.0f;
        bottomY = Constants::WINDOW_HEIGHT - 100.0f;
        // Пересчитываем gapY, чтобы дыра была правильного размера
        gapY = bottomY - gapHeight / 2.0f;
    }
    bottomPipe.setSize(sf::Vector2f(pipeWidth, bottomHeight));
    bottomPipe.setPosition(sf::Vector2f(x, bottomY));
    if (texturesLoaded) {
        bottomPipe.setTexture(&pipeUpTexture);
    } else {
        bottomPipe.setFillColor(sf::Color::Green);
    }
}

void Column::update(float dt) {
    float moveX = -Constants::COLUMN_SPEED * dt;
    topPipe.move(sf::Vector2f(moveX, 0.0f));
    bottomPipe.move(sf::Vector2f(moveX, 0.0f));
}

void Column::draw(sf::RenderWindow& window) const {
    window.draw(topPipe);
    window.draw(bottomPipe);
}

sf::FloatRect Column::getTopBounds() const {
    return topPipe.getGlobalBounds();
}

sf::FloatRect Column::getBottomBounds() const {
    return bottomPipe.getGlobalBounds();
}

float Column::getX() const {
    return topPipe.getPosition().x;
}
