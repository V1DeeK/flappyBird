#include "bird.hpp"
#include "../Constants.hpp"
#include "../ResourceManager.hpp"
#include <vector>

Bird::Bird() {
    loadTextures("assets/bird/fbird1.png", "assets/bird/fbird2.png", 
                 "assets/bird/fbird3.png", "assets/bird/fbird4.png");
}

Bird::Bird(const std::string& texturePath1, const std::string& texturePath2, 
           const std::string& texturePath3, const std::string& texturePath4) {
    loadTextures(texturePath1, texturePath2, texturePath3, texturePath4);
}

void Bird::loadTextures(const std::string& path1, const std::string& path2, 
                        const std::string& path3, const std::string& path4) {
    auto& rm = ResourceManager::getInstance();
    
    std::vector<std::string> paths1 = {"../" + path1, path1};
    std::vector<std::string> paths2 = {"../" + path2, path2};
    std::vector<std::string> paths3 = {"../" + path3, path3};
    std::vector<std::string> paths4 = {"../" + path4, path4};
    
    bool loaded1 = rm.loadTextureFromPaths("bird_texture1", paths1);
    bool loaded2 = rm.loadTextureFromPaths("bird_texture2", paths2);
    bool loaded3 = rm.loadTextureFromPaths("bird_texture3", paths3);
    bool loaded4 = rm.loadTextureFromPaths("bird_texture4", paths4);
    
    if (!loaded1 || !loaded2 || !loaded3 || !loaded4) {
        std::vector<std::string> defaultPaths1 = {"../assets/bird/fbird1.png", "assets/bird/fbird1.png"};
        std::vector<std::string> defaultPaths2 = {"../assets/bird/fbird2.png", "assets/bird/fbird2.png"};
        std::vector<std::string> defaultPaths3 = {"../assets/bird/fbird3.png", "assets/bird/fbird3.png"};
        std::vector<std::string> defaultPaths4 = {"../assets/bird/fbird4.png", "assets/bird/fbird4.png"};
        
        if (!loaded1) {
            loaded1 = rm.loadTextureFromPaths("bird_texture1", defaultPaths1);
        }
        if (!loaded2) {
            loaded2 = rm.loadTextureFromPaths("bird_texture2", defaultPaths2);
        }
        if (!loaded3) {
            loaded3 = rm.loadTextureFromPaths("bird_texture3", defaultPaths3);
        }
        if (!loaded4) {
            loaded4 = rm.loadTextureFromPaths("bird_texture4", defaultPaths4);
        }
    }
    
    if (loaded1) {
        texture1 = rm.getTexture("bird_texture1");
    }
    if (loaded2) {
        texture2 = rm.getTexture("bird_texture2");
    }
    if (loaded3) {
        texture3 = rm.getTexture("bird_texture3");
    }
    if (loaded4) {
        texture4 = rm.getTexture("bird_texture4");
    }
    
    if (loaded1) {
        sprite = std::make_unique<sf::Sprite>(texture1);
        float birdY = Constants::WINDOW_HEIGHT / 2.0f - Constants::BIRD_Y_OFFSET;
        sprite->setPosition(sf::Vector2f(Constants::BIRD_X_POSITION, birdY));
    } else {
        sprite = nullptr;
    }
}

void Bird::update(float deltaTime) {
    if (started && sprite) {
        if (deltaTime > Constants::MAX_DELTA_TIME) {
            deltaTime = Constants::MAX_DELTA_TIME;
        }
        
        velocity.y += Constants::GRAVITY * deltaTime;
        
        if (velocity.y > Constants::MAX_FALL_SPEED) {
            velocity.y = Constants::MAX_FALL_SPEED;
        }
        
        sf::Vector2f moveDelta = velocity * deltaTime;
        sprite->move(moveDelta);
        
        if (isJumping) {
            jumpTimer += deltaTime;
            
            if (jumpTimer < Constants::JUMP_ANIMATION_TIME / 2.0f) {
                sprite->setTexture(texture2);
            } else if (jumpTimer < Constants::JUMP_ANIMATION_TIME) {
                sprite->setTexture(texture3);
            } else {
                isJumping = false;
                jumpTimer = 0.0f;
            }
        } else {
            if (velocity.y > 0) {
                sprite->setTexture(texture4);
            } else {
                sprite->setTexture(texture1);
            }
        }
    }
}

void Bird::jump() {
    started = true;
    velocity.y = Constants::BIRD_JUMP_IMPULSE;
    isJumping = true;
    jumpTimer = 0.0f;
    
    if (sprite) {
        sprite->setTexture(texture2);
    }
}

void Bird::draw(sf::RenderWindow& window) const {
    if (sprite) {
        window.draw(*sprite);
    }
}

sf::FloatRect Bird::getBounds() const {
    if (sprite) {
        sf::FloatRect bounds = sprite->getGlobalBounds();
        return bounds;
    }
    return sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f));
}
