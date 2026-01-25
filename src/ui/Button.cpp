#include "Button.hpp"
#include "../Constants.hpp"
#include <iostream>
#include <vector>

Button::Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& textStr)
    : rect(size)
{
    rect.setPosition(position);
    // Делаем фон кнопки полупрозрачным для лучшей видимости текста
    rect.setFillColor(sf::Color(50, 50, 50, 200));  // Темно-серый с прозрачностью
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(2.0f);

    // Загружаем шрифт
    bool fontLoaded = false;
    
    if (font.openFromFile("../assets/fonts/ScoreFont.ttf")) {
        fontLoaded = true;
    }

    if (!fontLoaded) {
        return;
    }

    // Создаем текст с загруженным шрифтом
    text = std::make_unique<sf::Text>(font);
    text->setString(textStr);
    text->setCharacterSize(Constants::FONT_SIZE);
    text->setFillColor(sf::Color::White);

    // Центрирование текста
    // В SFML 3.0 FloatRect имеет поля position и size
    sf::FloatRect textBounds = text->getLocalBounds();
    
    // Правильное центрирование с учетом position (может быть отрицательным из-за глифов)
    sf::Vector2f origin(
        textBounds.size.x / 2.0f + textBounds.position.x, 
        textBounds.size.y / 2.0f + textBounds.position.y
    );
    text->setOrigin(origin);
    
    // Позиционируем текст в центре кнопки
    text->setPosition(sf::Vector2f(position.x + size.x / 2.0f, position.y + size.y / 2.0f));
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.is<sf::Event::MouseButtonPressed>()) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            pressed = true;
        }
    }
    if (event.is<sf::Event::MouseButtonReleased>()) {
        pressed = false;
    }
}

void Button::render(sf::RenderWindow& window) {
    window.draw(rect);
    if (text) {
        window.draw(*text);
    }
}

bool Button::isPressed() const {
    return pressed;
}
