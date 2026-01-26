#include "GameOverState.hpp"
#include "../Game.hpp"
#include "../Constants.hpp"
#include "../ResourceManager.hpp"
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
        sf::Vector2f(Constants::BUTTON_WIDTH, Constants::BUTTON_HEIGHT),
        sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f - Constants::BUTTON_X_OFFSET, Constants::WINDOW_HEIGHT / 2.0f + Constants::GAMEOVER_RESTART_BUTTON_Y),
        "Restart"
      )
    , exitButton(
        sf::Vector2f(Constants::BUTTON_WIDTH, Constants::BUTTON_HEIGHT),
        sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f - Constants::BUTTON_X_OFFSET, Constants::WINDOW_HEIGHT / 2.0f + Constants::GAMEOVER_EXIT_BUTTON_Y),
        "Exit"
      )
{
    // Сохраняем рекорд, если нужно
    if (score > highScore) {
        std::ofstream file(Constants::HIGHSCORE_FILE);
        if (file.is_open()) {
            file << score;
            this->highScore = score; // Обновляем локальный рекорд
        }
    }

    // Load background using ResourceManager
    auto& rm = ResourceManager::getInstance();
    std::vector<std::string> bgPaths = {
        "../assets/textures/FonGameOver.png",
        "assets/textures/FonGameOver.png"
    };
    if (rm.loadTextureFromPaths("gameover_background", bgPaths)) {
        backgroundTexture = rm.getTexture("gameover_background");
        backgroundSprite = std::make_unique<sf::Sprite>(backgroundTexture);
        float scaleX = static_cast<float>(Constants::WINDOW_WIDTH) / backgroundTexture.getSize().x;
        float scaleY = static_cast<float>(Constants::WINDOW_HEIGHT) / backgroundTexture.getSize().y;
        backgroundSprite->setScale(sf::Vector2f(scaleX, scaleY));
    }
    
    // Load font using ResourceManager
    std::vector<std::string> fontPaths = {
        "../assets/fonts/ScoreFont.ttf",
        "assets/fonts/ScoreFont.ttf"
    };
    rm.loadFontFromPaths("score_font", fontPaths);
    fontLoaded = rm.hasFont("score_font");

    if (fontLoaded) {
        // "Game Over" text
        gameOverText = std::make_unique<sf::Text>(rm.getFont("score_font"));
        gameOverText->setString("Game Over");
        gameOverText->setCharacterSize(Constants::FONT_SIZE_TITLE);
        gameOverText->setFillColor(sf::Color::Red);
        gameOverText->setStyle(sf::Text::Bold);
        
        sf::FloatRect bounds = gameOverText->getLocalBounds();
        sf::Vector2f origin(bounds.size.x / 2.0f + bounds.position.x, 
                           bounds.size.y / 2.0f + bounds.position.y);
        gameOverText->setOrigin(origin);
        gameOverText->setPosition(sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f, Constants::GAMEOVER_TITLE_Y));

        // Score text
        scoreText = std::make_unique<sf::Text>(rm.getFont("score_font"));
        scoreText->setString("Score: " + std::to_string(score));
        scoreText->setCharacterSize(Constants::FONT_SIZE_LARGE);
        scoreText->setFillColor(sf::Color::Yellow);
        
        bounds = scoreText->getLocalBounds();
        origin = sf::Vector2f(bounds.size.x / 2.0f + bounds.position.x, 
                             bounds.size.y / 2.0f + bounds.position.y);
        scoreText->setOrigin(origin);
        scoreText->setPosition(sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f, Constants::GAMEOVER_SCORE_Y));

        // High score text (top right corner)
        highScoreText = std::make_unique<sf::Text>(rm.getFont("score_font"));
        highScoreText->setString("High Score: " + std::to_string(this->highScore));
        highScoreText->setCharacterSize(Constants::FONT_SIZE_LARGE);
        highScoreText->setFillColor(sf::Color::Red);
        highScoreText->setStyle(sf::Text::Bold);
        
        bounds = highScoreText->getLocalBounds();
        // Align to right edge
        origin = sf::Vector2f(bounds.size.x + bounds.position.x, 
                             bounds.size.y / 2.0f + bounds.position.y);
        highScoreText->setOrigin(origin);
        // Position in top right corner with offset
        highScoreText->setPosition(sf::Vector2f(Constants::WINDOW_WIDTH - Constants::HIGHSCORE_TEXT_X_OFFSET, Constants::HIGHSCORE_TEXT_Y));
    }
}

void GameOverState::handleEvents(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        game.window.close();
        return;
    }

    // Обрабатываем события кнопок
    restartButton.handleEvent(event, game.window);
    exitButton.handleEvent(event, game.window);

    // Обработка нажатий кнопок
    if (restartButton.isPressed()) {
        // Начинаем новую игру
        game.changeState(std::make_unique<GameState>(game));
    }

    if (exitButton.isPressed()) {
        // Возвращаемся в главное меню
        game.popState();
    }
}

void GameOverState::update(float deltaTime) {
    // Ничего не обновляется
}

void GameOverState::render(sf::RenderWindow& window) {
    // Рисуем фон
    if (backgroundSprite) window.draw(*backgroundSprite);
    
    // Отображаем тексты
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
    
    // Отображаем кнопки
    restartButton.render(window);
    exitButton.render(window);
}

