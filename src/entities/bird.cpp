
#include "bird.hpp"

#include "../Constants.hpp"

#include <iostream>

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

    std::vector<std::string> paths1 = {"../" + path1, path1};
    std::vector<std::string> paths2 = {"../" + path2, path2};
    std::vector<std::string> paths3 = {"../" + path3, path3};
    std::vector<std::string> paths4 = {"../" + path4, path4};

    bool loaded1 = false, loaded2 = false, loaded3 = false, loaded4 = false;

    for (const auto& path : paths1) {

        if (texture1.loadFromFile(path)) {

            loaded1 = true;

            break;
        }
    }

    for (const auto& path : paths2) {
        if (texture2.loadFromFile(path)) {
            loaded2 = true;
            break;
        }
    }
    for (const auto& path : paths3) {
        if (texture3.loadFromFile(path)) {
            loaded3 = true;
            break;
        }
    }
    for (const auto& path : paths4) {
        if (texture4.loadFromFile(path)) {
            loaded4 = true;
            break;
        }
    }

    if (!loaded1 || !loaded2 || !loaded3 || !loaded4) {

        std::vector<std::string> defaultPaths1 = {"../assets/bird/fbird1.png", "assets/bird/fbird1.png"};
        std::vector<std::string> defaultPaths2 = {"../assets/bird/fbird2.png", "assets/bird/fbird2.png"};
        std::vector<std::string> defaultPaths3 = {"../assets/bird/fbird3.png", "assets/bird/fbird3.png"};
        std::vector<std::string> defaultPaths4 = {"../assets/bird/fbird4.png", "assets/bird/fbird4.png"};

        if (!loaded1) {

            for (const auto& path : defaultPaths1) {

                if (texture1.loadFromFile(path)) {
                    loaded1 = true;
                    break;
                }
            }
        }

        if (!loaded2) {
            for (const auto& path : defaultPaths2) {
                if (texture2.loadFromFile(path)) {
                    loaded2 = true;
                    break;
                }
            }
        }
        if (!loaded3) {
            for (const auto& path : defaultPaths3) {
                if (texture3.loadFromFile(path)) {
                    loaded3 = true;
                    break;
                }
            }
        }
        if (!loaded4) {
            for (const auto& path : defaultPaths4) {
                if (texture4.loadFromFile(path)) {
                    loaded4 = true;
                    break;
                }
            }
        }
    }

    if (loaded1) {

        sprite = std::make_unique<sf::Sprite>(texture1);

        float birdY = Constants::WINDOW_HEIGHT / 2.0f - 15.0f;

        sprite->setPosition(sf::Vector2f(Constants::BIRD_X_POSITION, birdY));
    } else {

        sprite = nullptr;
    }
}

void Bird::update(float deltaTime) {

    if (started && sprite) {

        if (deltaTime > 0.1f) {

            deltaTime = 0.1f;
        }

        velocity.y += Constants::GRAVITY * deltaTime;

        if (velocity.y > 1000.0f) {

            velocity.y = 1000.0f;
        }

        sf::Vector2f moveDelta = velocity * deltaTime;

        sprite->move(moveDelta);

        if (isJumping) {

            jumpTimer += deltaTime;

            if (jumpTimer < JUMP_ANIMATION_TIME / 2.0f) {

                sprite->setTexture(texture2);

            } else if (jumpTimer < JUMP_ANIMATION_TIME) {

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
