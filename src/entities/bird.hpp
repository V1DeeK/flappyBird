
#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

class Bird {

public:

    Bird();

    Bird(const std::string& texturePath1, const std::string& texturePath2,
         const std::string& texturePath3, const std::string& texturePath4);

    void update(float deltaTime);

    void jump();

    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;

    bool hasStarted() const { return started; }

private:

    void loadTextures(const std::string& path1, const std::string& path2,
                     const std::string& path3, const std::string& path4);

    std::unique_ptr<sf::Sprite> sprite;

    sf::Texture texture1;

    sf::Texture texture2;

    sf::Texture texture3;

    sf::Texture texture4;

    sf::Vector2f velocity;

    bool started = false;

    bool isJumping = false;

    float jumpTimer = 0.0f;

    static constexpr float JUMP_ANIMATION_TIME = 0.15f;
};
