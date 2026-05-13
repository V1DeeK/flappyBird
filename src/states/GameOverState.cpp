#include "GameOverState.hpp"
#include "../Game.hpp"
#include "../Constants.hpp"
#include "../ui/Button.hpp"
#include "GameState.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

GameOverState::GameOverState(Game& game, int score, int highScore)
    : game(game)
    , score(score)
    , highScore(highScore)
    , restartButton(
        sf::Vector2f(250.0f, 60.0f),
        sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f - 125.0f, Constants::WINDOW_HEIGHT / 2.0f + 50.0f),
        "Restart"
      )
    , exitButton(
        sf::Vector2f(250.0f, 60.0f),
        sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f - 125.0f, Constants::WINDOW_HEIGHT / 2.0f + 130.0f),
        "Exit"
      )
{

    if (score > highScore) {
        std::ofstream file(Constants::HIGHSCORE_FILE);
        if (file.is_open()) {
            file << score;
            this->highScore = score;
        }
    }

    std::vector<std::string> bgPaths = {
        "../assets/textures/FonGameOver.png",
        "assets/textures/FonGameOver.png"
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

    if (font.openFromFile("../assets/fonts/ScoreFont.ttf")) {
        fontLoaded = true;
    }

    if (fontLoaded) {

        gameOverText = std::make_unique<sf::Text>(font);
        gameOverText->setString("Game Over");
        gameOverText->setCharacterSize(48);
        gameOverText->setFillColor(sf::Color::Red);
        gameOverText->setStyle(sf::Text::Bold);

        sf::FloatRect bounds = gameOverText->getLocalBounds();
        sf::Vector2f origin(bounds.size.x / 2.0f + bounds.position.x,
                           bounds.size.y / 2.0f + bounds.position.y);
        gameOverText->setOrigin(origin);
        gameOverText->setPosition(sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f, 150.0f));

        scoreText = std::make_unique<sf::Text>(font);
        scoreText->setString("Score: " + std::to_string(score));
        scoreText->setCharacterSize(36);
        scoreText->setFillColor(sf::Color::Yellow);

        bounds = scoreText->getLocalBounds();
        origin = sf::Vector2f(bounds.size.x / 2.0f + bounds.position.x,
                             bounds.size.y / 2.0f + bounds.position.y);
        scoreText->setOrigin(origin);
        scoreText->setPosition(sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f, 250.0f));

        highScoreText = std::make_unique<sf::Text>(font);
        highScoreText->setString("High Score: " + std::to_string(this->highScore));
        highScoreText->setCharacterSize(36);
        highScoreText->setFillColor(sf::Color::Red);
        highScoreText->setStyle(sf::Text::Bold);

        bounds = highScoreText->getLocalBounds();

        origin = sf::Vector2f(bounds.size.x + bounds.position.x,
                             bounds.size.y / 2.0f + bounds.position.y);
        highScoreText->setOrigin(origin);

        highScoreText->setPosition(sf::Vector2f(Constants::WINDOW_WIDTH - 20.0f, 20.0f));
    }
}

void GameOverState::handleEvents(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        game.window.close();
        return;
    }

    restartButton.handleEvent(event, game.window);
    exitButton.handleEvent(event, game.window);

    if (restartButton.isPressed()) {

        game.changeState(std::make_unique<GameState>(game));
    }

    if (exitButton.isPressed()) {

        game.popState();
    }
}

void GameOverState::update(float deltaTime) {

}

void GameOverState::render(sf::RenderWindow& window) {

    if (backgroundSprite) window.draw(*backgroundSprite);

    if (fontLoaded) {
        if (gameOverText) {
            window.draw(*gameOverText);
        }
        if (scoreText) {
            window.draw(*scoreText);
        }
        if (highScoreText) {
            window.draw(*highScoreText);
        }
    }

    restartButton.render(window);
    exitButton.render(window);
}
