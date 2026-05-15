#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>

class Button {
public:
    Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void render(sf::RenderWindow& window);
    bool isPressed() const;

private:
    sf::RectangleShape rect;
    sf::Font font;
    std::unique_ptr<sf::Text> text;  // Используем указатель, так как Text требует Font при создании
    bool pressed = false;
};
