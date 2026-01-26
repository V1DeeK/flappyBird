#include "GameOverState.hpp"
#include "../Game.hpp"
#include "../Constants.hpp"
#include "../ResourceManager.hpp"
#include "../ui/Button.hpp"
#include "GameState.hpp"
#include <SFML/Graphics/Text.hpp>
#include <memory>
#include <fstream>
#include <vector>

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
    if (score > highScore) {
        std::ofstream file(Constants::HIGHSCORE_FILE);
        if (file.is_open()) {
            file << score;
            this->highScore = score;
        }
    }

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
    
    std::vector<std::string> fontPaths = {
        "../assets/fonts/ScoreFont.ttf",
        "assets/fonts/ScoreFont.ttf"
    };
    rm.loadFontFromPaths("score_font", fontPaths);
    fontLoaded = rm.hasFont("score_font");

    if (fontLoaded) {
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

        scoreText = std::make_unique<sf::Text>(rm.getFont("score_font"));
        scoreText->setString("Score: " + std::to_string(score));
        scoreText->setCharacterSize(Constants::FONT_SIZE_LARGE);
        scoreText->setFillColor(sf::Color::Yellow);
        
        bounds = scoreText->getLocalBounds();
        origin = sf::Vector2f(bounds.size.x / 2.0f + bounds.position.x, 
                             bounds.size.y / 2.0f + bounds.position.y);
        scoreText->setOrigin(origin);
        scoreText->setPosition(sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f, Constants::GAMEOVER_SCORE_Y));

        highScoreText = std::make_unique<sf::Text>(rm.getFont("score_font"));
        highScoreText->setString("High Score: " + std::to_string(this->highScore));
        highScoreText->setCharacterSize(Constants::FONT_SIZE_LARGE);
        highScoreText->setFillColor(sf::Color::Red);
        highScoreText->setStyle(sf::Text::Bold);
        
        bounds = highScoreText->getLocalBounds();
        origin = sf::Vector2f(bounds.size.x + bounds.position.x, 
                             bounds.size.y / 2.0f + bounds.position.y);
        highScoreText->setOrigin(origin);
        highScoreText->setPosition(sf::Vector2f(Constants::WINDOW_WIDTH - Constants::HIGHSCORE_TEXT_X_OFFSET, Constants::HIGHSCORE_TEXT_Y));
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
