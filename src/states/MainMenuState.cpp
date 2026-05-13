#include "MainMenuState.hpp"
#include "GameState.hpp"
#include "../Game.hpp"
#include "../Constants.hpp"
#include <vector>
#include <iostream>

MainMenuState::MainMenuState(Game& game)
    : game(game)
    , playButton(
        sf::Vector2f(250.0f, 60.0f),
        sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f - 125.0f, Constants::WINDOW_HEIGHT / 2.0f + 200.0f),
        "Play"
      )
    , exitButton(
        sf::Vector2f(250.0f, 60.0f),
        sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f - 125.0f, Constants::WINDOW_HEIGHT / 2.0f + 280.0f),
        "Exit"
      )
{
    std::vector<std::string> bgPaths = {
        "../assets/textures/SkySunGame.png",
        "assets/textures/SkySunGame.png"
    };
    for (const auto& path : bgPaths) {
        if (backgroundTexture.loadFromFile(path)) {
            backgroundSprite = std::make_unique<sf::Sprite>(backgroundTexture);
            float scaleX = static_cast<float>(Constants::WINDOW_WIDTH) / backgroundTexture.getSize().x;
            float scaleY = static_cast<float>(Constants::WINDOW_HEIGHT) / backgroundTexture.getSize().y;
            backgroundSprite->setScale(sf::Vector2f(scaleX, scaleY));
            break;
        }
    }
}

void MainMenuState::handleEvents(const sf::Event& event) {
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
}

void MainMenuState::render(sf::RenderWindow& window) {
    if (backgroundSprite) {
        window.draw(*backgroundSprite);
    } else {
        window.clear(sf::Color::Black);
    }

    playButton.render(window);
    exitButton.render(window);
}