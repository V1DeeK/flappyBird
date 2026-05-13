#pragma once

#include <SFML/Graphics.hpp>

class State
{
public:
    virtual ~State() = default;
    virtual void handleEvents(const sf::Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};
