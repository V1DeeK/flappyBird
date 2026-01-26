#include "GameState.hpp"
#include "../Game.hpp"
#include "../entities/bird.hpp"
#include "../entities/column.hpp"
#include "../Constants.hpp"
#include "../ResourceManager.hpp"
#include "../Collisions.hpp"
#include "GameOverState.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <algorithm>
#include <fstream>
#include <vector>

GameState::GameState(Game& game)
    : game(game)
    , gameStarted(false)
    , rng(std::random_device{}())
    , gapYDist(Constants::GAP_Y_MIN, Constants::WINDOW_HEIGHT - Constants::GAP_Y_MAX_OFFSET)
    , landOffset(0.0f)
    , hitSoundPlayed(false)
{
    auto& rm = ResourceManager::getInstance();
    
    // Create default bird
    bird = std::make_unique<Bird>();
    
    // Load font using ResourceManager
    std::vector<std::string> fontPaths = {
        "../assets/fonts/ScoreFont.ttf",
        "assets/fonts/ScoreFont.ttf"
    };
    rm.loadFontFromPaths("score_font", fontPaths);
    
    // Load sky texture using ResourceManager
    std::vector<std::string> skyPaths = {
        "../assets/textures/SkyFonGame.png",
        "assets/textures/SkyFonGame.png"
    };
    if (rm.loadTextureFromPaths("sky", skyPaths)) {
        skyTexture = rm.getTexture("sky");
        skySprite = std::make_unique<sf::Sprite>(skyTexture);
        float scaleX = static_cast<float>(Constants::WINDOW_WIDTH) / skyTexture.getSize().x;
        float scaleY = static_cast<float>(Constants::WINDOW_HEIGHT) / skyTexture.getSize().y;
        skySprite->setScale(sf::Vector2f(scaleX, scaleY));
    }
    
    // Load land texture using ResourceManager
    std::vector<std::string> landPaths = {
        "../assets/textures/LandGame.png",
        "assets/textures/LandGame.png"
    };
    if (rm.loadTextureFromPaths("land", landPaths)) {
        landTexture = rm.getTexture("land");
        landSprite1 = std::make_unique<sf::Sprite>(landTexture);
        landSprite2 = std::make_unique<sf::Sprite>(landTexture);
        
        float landHeight = static_cast<float>(landTexture.getSize().y);
        float landY = Constants::WINDOW_HEIGHT - landHeight;
        
        landSprite1->setPosition(sf::Vector2f(0.0f, landY));
        landSprite2->setPosition(sf::Vector2f(static_cast<float>(landTexture.getSize().x), landY));
    }
    
    // Load sounds using ResourceManager
    std::vector<std::string> wingPaths = {
        "../assets/sounds/Wing.wav",
        "assets/sounds/Wing.wav"
    };
    rm.loadSoundBufferFromPaths("wing", wingPaths);
    wingSound = rm.createSound("wing");
    
    std::vector<std::string> hitPaths = {
        "../assets/sounds/Hit.wav",
        "assets/sounds/Hit.wav"
    };
    rm.loadSoundBufferFromPaths("hit", hitPaths);
    hitSound = rm.createSound("hit");
}

void GameState::handleEvents(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        game.window.close();
    }

    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Escape) {
            game.popState(); // вернуться в меню
        }
        if (keyEvent->code == sf::Keyboard::Key::Space) {
            if (!gameStarted) {
                gameStarted = true;
                spawnClock.restart(); // Перезапускаем таймер спавна при старте игры
            }
            bird->jump();
            // Воспроизводим звук крыла
            if (wingSound) wingSound->play();
        }
    }
}

void GameState::update(float dt) {
    if (gameStarted) {
        // Обновляем прокрутку земли
        float landSpeed = Constants::COLUMN_SPEED;
        landOffset -= landSpeed * dt;
        
        // Получаем ширину текстуры земли
        float landWidth = static_cast<float>(landTexture.getSize().x);
        
        // Если первый спрайт ушел за экран, перемещаем его вправо
        if (landOffset <= -landWidth) {
            landOffset += landWidth;
        }
        
            // Обновляем позиции спрайтов земли
            if (landSprite1 && landSprite2) {
                float landHeight = static_cast<float>(landTexture.getSize().y);
                float landY = Constants::WINDOW_HEIGHT - landHeight;
                
                landSprite1->setPosition(sf::Vector2f(landOffset, landY));
                landSprite2->setPosition(sf::Vector2f(landOffset + landWidth, landY));
            }
        
        bird->update(dt);

        // Спавн труб
        // Спавним первую трубу сразу при старте, затем каждые COLUMN_SPAWN_INTERVAL секунд
        if (columns.empty() || spawnClock.getElapsedTime().asSeconds() >= Constants::COLUMN_SPAWN_INTERVAL) {
            float gapY = gapYDist(rng);
            columns.emplace_back(Constants::WINDOW_WIDTH, gapY);
            spawnClock.restart();
        }

        // Обновление труб
        for (auto& column : columns) {
            column.update(dt);
        }

        // Remove columns off screen
        columns.erase(
            std::remove_if(columns.begin(), columns.end(),
                [](const Column& c) { return c.getX() + Constants::PIPE_WIDTH < 0; }),
            columns.end()
        );

        // Check if bird passed columns (for scoring)
        for (auto& column : columns) {
            if (Collisions::checkBirdPassedColumn(*bird, column)) {
                column.markPassed();
                score++;
            }
        }

        // Check collisions with pipes
        if (Collisions::checkBirdPipeCollision(*bird, columns)) {
            if (!hitSoundPlayed) {
                if (hitSound) hitSound->play();
                hitSoundPlayed = true;
            }
            game.changeState(std::make_unique<GameOverState>(game, score, getHighScore()));
            return;
        }

        // Check collisions with boundaries (top/bottom)
        if (Collisions::checkBirdBoundaryCollision(*bird)) {
            if (!hitSoundPlayed) {
                if (hitSound) hitSound->play();
                hitSoundPlayed = true;
            }
            game.changeState(std::make_unique<GameOverState>(game, score, getHighScore()));
            return;
        }
    }
}

void GameState::render(sf::RenderWindow& window) {
    // Порядок отрисовки слоёв (от дальнего к ближнему):
    // 1. Фон неба (самый дальний слой)
    if (skySprite) window.draw(*skySprite);
    
    // 2. Земля (на фоне неба)
    if (landSprite1) window.draw(*landSprite1);
    if (landSprite2) window.draw(*landSprite2);
    
    // 3. Трубы (на земле)
    for (const auto& column : columns) {
        column.draw(window);
    }
    
    // 4. Птица (поверх всего)
    bird->draw(window);

    // Score
    auto& rm = ResourceManager::getInstance();
    if (rm.hasFont("score_font")) {
        sf::Text scoreText(rm.getFont("score_font"));
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(Constants::FONT_SIZE_DEFAULT);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(sf::Vector2f(Constants::SCORE_TEXT_X, Constants::SCORE_TEXT_Y));
        window.draw(scoreText);
    }

    if (!gameStarted && rm.hasFont("score_font")) {
        sf::Text prompt(rm.getFont("score_font"));
        prompt.setString("Press Space to Start");
        prompt.setCharacterSize(Constants::FONT_SIZE_LARGE);
        prompt.setFillColor(sf::Color::White);

        sf::FloatRect bounds = prompt.getLocalBounds();
        sf::Vector2f origin(bounds.size.x / 2.0f + bounds.position.x, 
                           bounds.size.y / 2.0f + bounds.position.y);
        prompt.setOrigin(origin);
        prompt.setPosition(sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f, Constants::WINDOW_HEIGHT / 2.0f - Constants::PROMPT_TEXT_Y_OFFSET));
        window.draw(prompt);
    }
}

int GameState::getHighScore() const {
    std::ifstream file(Constants::HIGHSCORE_FILE);
    int highScore = 0;
    if (file >> highScore) {
        return highScore;
    }
    return 0;
}
