#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Button.hpp"

class ModalDialog {
public:
    ModalDialog();

    void show();
    void hide();
    bool isVisible() const { return visible; }

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

    bool isYesPressed() const;
    bool isNoPressed() const;
    void reset();

    void setMessage(const std::string& message);

private:
    bool visible = false;

    sf::RectangleShape background;
    sf::RectangleShape dialogBox;

    sf::Font font;
    std::unique_ptr<sf::Text> messageText;
    bool fontLoaded = false;

    Button yesButton;
    Button noButton;
};
