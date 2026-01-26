#include "MainMenuState.hpp"
#include "GameState.hpp"
#include "../Game.hpp"
#include "../Constants.hpp"
#include "../ResourceManager.hpp"
#include <vector>
#include <iostream>

MainMenuState::MainMenuState(Game& game)
    : game(game)
    , playButton(
        sf::Vector2f(Constants::BUTTON_WIDTH, Constants::BUTTON_HEIGHT),
        sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f - Constants::BUTTON_X_OFFSET, Constants::WINDOW_HEIGHT / 2.0f + Constants::MENU_PLAY_BUTTON_Y),
        "Play"
      )
    , exitButton(
        sf::Vector2f(Constants::BUTTON_WIDTH, Constants::BUTTON_HEIGHT),
        sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f - Constants::BUTTON_X_OFFSET, Constants::WINDOW_HEIGHT / 2.0f + Constants::MENU_EXIT_BUTTON_Y),
        "Exit"
      )
{
    // Load background using ResourceManager
    auto& rm = ResourceManager::getInstance();
    std::vector<std::string> bgPaths = {
        "../assets/textures/SkySunGame.png",
        "assets/textures/SkySunGame.png"
    };
    if (rm.loadTextureFromPaths("menu_background", bgPaths)) {
        backgroundTexture = rm.getTexture("menu_background");
        backgroundSprite = std::make_unique<sf::Sprite>(backgroundTexture);
        float scaleX = static_cast<float>(Constants::WINDOW_WIDTH) / backgroundTexture.getSize().x;
        float scaleY = static_cast<float>(Constants::WINDOW_HEIGHT) / backgroundTexture.getSize().y;
        backgroundSprite->setScale(sf::Vector2f(scaleX, scaleY));
    }
}

void MainMenuState::handleEvents(const sf::Event& event) {
    // Обрабатываем события кнопок главного меню
    playButton.handleEvent(event, game.window);
    exitButton.handleEvent(event, game.window);

    if (playButton.isPressed()) {
        game.changeState(std::make_unique<GameState>(game));
    }

    if (exitButton.isPressed()) {
        game.window.close();
    }
}

void MainMenuState::update(float deltaTime) {
    // Ничего не обновляется в меню
}

void MainMenuState::render(sf::RenderWindow& window) {
    // Рисуем фон
    if (backgroundSprite) {
        window.draw(*backgroundSprite);
    } else {
        window.clear(sf::Color::Black);
    }

    playButton.render(window);
    exitButton.render(window);
}