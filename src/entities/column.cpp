#include "column.hpp"
#include "../Constants.hpp"
#include "../ResourceManager.hpp"
#include <vector>

Column::Column(float x, float gapY)
    : gapY(gapY)
{
    auto& rm = ResourceManager::getInstance();
    
    static bool texturesLoaded = false;
    if (!texturesLoaded) {
        std::vector<std::string> pathsDown = {"../assets/textures/PipeDown.png", "assets/textures/PipeDown.png"};
        std::vector<std::string> pathsUp = {"../assets/textures/PipeUp.png", "assets/textures/PipeUp.png"};
        
        bool loadedDown = rm.loadTextureFromPaths("pipe_down", pathsDown);
        bool loadedUp = rm.loadTextureFromPaths("pipe_up", pathsUp);
        
        if (loadedDown && loadedUp) {
            texturesLoaded = true;
        }
    }
    
    float gapHeight = Constants::COLUMN_GAP;
    float pipeWidth = Constants::PIPE_WIDTH;
    
    float gapTop = gapY - gapHeight / 2.0f;
    float topHeight = gapTop;
    if (topHeight < Constants::MIN_PIPE_HEIGHT) {
        topHeight = Constants::MIN_PIPE_HEIGHT;
        gapTop = Constants::MIN_PIPE_HEIGHT;
        gapY = gapTop + gapHeight / 2.0f;
    }
    topPipe.setSize(sf::Vector2f(pipeWidth, topHeight));
    topPipe.setPosition(sf::Vector2f(x, 0.0f));
    if (texturesLoaded && rm.hasTexture("pipe_down")) {
        topPipe.setTexture(&rm.getTexture("pipe_down"));
    } else {
        topPipe.setFillColor(sf::Color::Green);
    }
    
    float bottomY = gapY + gapHeight / 2.0f;
    float bottomHeight = Constants::WINDOW_HEIGHT - bottomY;
    if (bottomHeight < Constants::MIN_PIPE_HEIGHT) {
        bottomHeight = Constants::MIN_PIPE_HEIGHT;
        bottomY = Constants::WINDOW_HEIGHT - Constants::MIN_PIPE_HEIGHT;
        gapY = bottomY - gapHeight / 2.0f;
    }
    bottomPipe.setSize(sf::Vector2f(pipeWidth, bottomHeight));
    bottomPipe.setPosition(sf::Vector2f(x, bottomY));
    if (texturesLoaded && rm.hasTexture("pipe_up")) {
        bottomPipe.setTexture(&rm.getTexture("pipe_up"));
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
