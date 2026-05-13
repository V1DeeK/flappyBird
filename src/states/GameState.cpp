#include "GameState.hpp"
#include "../Game.hpp"
#include "../entities/bird.hpp"
#include "../entities/column.hpp"
#include "../Constants.hpp"
#include "GameOverState.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <algorithm>
#include <fstream>
#include <vector>

static sf::Font globalFont;
static bool fontLoaded = false;

GameState::GameState(Game& game)
    : game(game)
    , gameStarted(false)
    , rng(std::random_device{}())
    , gapYDist(150.0f, Constants::WINDOW_HEIGHT - 150.0f)
    , landOffset(0.0f)
    , hitSoundPlayed(false)
{

    bird = std::make_unique<Bird>();

    if (!fontLoaded) {
        if (globalFont.openFromFile("../assets/fonts/ScoreFont.ttf")) {
            fontLoaded = true;
        }
    }

    std::vector<std::string> skyPaths = {
        "../assets/textures/SkyFonGame.png",
        "assets/textures/SkyFonGame.png"
    };
    for (const auto& path : skyPaths) {
        if (skyTexture.loadFromFile(path)) {
            skySprite = std::make_unique<sf::Sprite>(skyTexture);

            float scaleX = static_cast<float>(Constants::WINDOW_WIDTH) / skyTexture.getSize().x;
            float scaleY = static_cast<float>(Constants::WINDOW_HEIGHT) / skyTexture.getSize().y;
            skySprite->setScale(sf::Vector2f(scaleX, scaleY));
            break;
        }
    }

    std::vector<std::string> landPaths = {
        "../assets/textures/LandGame.png",
        "assets/textures/LandGame.png"
    };
    for (const auto& path : landPaths) {
        if (landTexture.loadFromFile(path)) {
            landSprite1 = std::make_unique<sf::Sprite>(landTexture);
            landSprite2 = std::make_unique<sf::Sprite>(landTexture);

            float landHeight = static_cast<float>(landTexture.getSize().y);
            float landY = Constants::WINDOW_HEIGHT - landHeight;

            landSprite1->setPosition(sf::Vector2f(0.0f, landY));
            landSprite2->setPosition(sf::Vector2f(static_cast<float>(landTexture.getSize().x), landY));
            break;
        }
    }

    std::vector<std::string> wingPaths = {
        "../assets/sounds/Wing.wav",
        "assets/sounds/Wing.wav"
    };
    for (const auto& path : wingPaths) {
        if (wingSoundBuffer.loadFromFile(path)) {
            wingSound = std::make_unique<sf::Sound>(wingSoundBuffer);
            break;
        }
    }

    std::vector<std::string> hitPaths = {
        "../assets/sounds/Hit.wav",
        "assets/sounds/Hit.wav"
    };
    for (const auto& path : hitPaths) {
        if (hitSoundBuffer.loadFromFile(path)) {
            hitSound = std::make_unique<sf::Sound>(hitSoundBuffer);
            break;
        }
    }
}

void GameState::handleEvents(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        game.window.close();
    }

    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Escape) {
            game.popState();
        }
        if (keyEvent->code == sf::Keyboard::Key::Space) {
            if (!gameStarted) {
                gameStarted = true;
                spawnClock.restart();
            }
            bird->jump();

            if (wingSound) wingSound->play();
        }
    }
}

void GameState::update(float dt) {
    if (gameStarted) {

        float landSpeed = Constants::COLUMN_SPEED;
        landOffset -= landSpeed * dt;

        float landWidth = static_cast<float>(landTexture.getSize().x);

        if (landOffset <= -landWidth) {
            landOffset += landWidth;
        }

            if (landSprite1 && landSprite2) {
                float landHeight = static_cast<float>(landTexture.getSize().y);
                float landY = Constants::WINDOW_HEIGHT - landHeight;

                landSprite1->setPosition(sf::Vector2f(landOffset, landY));
                landSprite2->setPosition(sf::Vector2f(landOffset + landWidth, landY));
            }

        bird->update(dt);

        if (columns.empty() || spawnClock.getElapsedTime().asSeconds() >= Constants::COLUMN_SPAWN_INTERVAL) {
            float gapY = gapYDist(rng);
            columns.emplace_back(Constants::WINDOW_WIDTH, gapY);
            spawnClock.restart();
        }

        for (auto& column : columns) {
            column.update(dt);
        }

        columns.erase(
            std::remove_if(columns.begin(), columns.end(),
                [](const Column& c) { return c.getX() + 80.0f < 0; }),
            columns.end()
        );

        sf::FloatRect birdBounds = bird->getBounds();

        if (birdBounds.size.x <= 0 || birdBounds.size.y <= 0) {

            return;
        }

        for (auto& column : columns) {

            if (!column.isPassed() && birdBounds.position.x > column.getX() + 80.0f) {
                column.markPassed();
                score++;
            }
        }

        float collisionScale = 0.7f;

        float widthReduction = birdBounds.size.x * (1.0f - collisionScale) / 2.0f;

        float heightReduction = birdBounds.size.y * (1.0f - collisionScale) / 2.0f;

        sf::FloatRect collisionBounds(
            sf::Vector2f(birdBounds.position.x + widthReduction, birdBounds.position.y + heightReduction),
            sf::Vector2f(birdBounds.size.x * collisionScale, birdBounds.size.y * collisionScale)
        );

        for (const auto& column : columns) {

            sf::FloatRect topBounds = column.getTopBounds();

            sf::FloatRect bottomBounds = column.getBottomBounds();

            if (topBounds.size.x <= 0 || topBounds.size.y <= 0 ||
                bottomBounds.size.x <= 0 || bottomBounds.size.y <= 0) {

                continue;
            }

            bool pipeReachedBird = (

                topBounds.position.x < collisionBounds.position.x + collisionBounds.size.x &&

                topBounds.position.x + topBounds.size.x > collisionBounds.position.x &&

                topBounds.position.x + topBounds.size.x >= Constants::BIRD_X_POSITION - 50.0f
            );

            if (pipeReachedBird) {

                bool topCollision = (

                    collisionBounds.position.x < topBounds.position.x + topBounds.size.x &&

                    collisionBounds.position.x + collisionBounds.size.x > topBounds.position.x &&

                    collisionBounds.position.y < topBounds.position.y + topBounds.size.y &&

                    collisionBounds.position.y + collisionBounds.size.y > topBounds.position.y
                );

                bool bottomCollision = (

                    collisionBounds.position.x < bottomBounds.position.x + bottomBounds.size.x &&

                    collisionBounds.position.x + collisionBounds.size.x > bottomBounds.position.x &&

                    collisionBounds.position.y < bottomBounds.position.y + bottomBounds.size.y &&

                    collisionBounds.position.y + collisionBounds.size.y > bottomBounds.position.y
                );

                if (topCollision || bottomCollision) {

                    if (!hitSoundPlayed) {

                        if (hitSound) {

                            hitSound->play();
                        }

                        hitSoundPlayed = true;
                    }

                    game.changeState(std::make_unique<GameOverState>(game, score, getHighScore()));

                    return;
                }
            }
        }

        if (bird->hasStarted() && birdBounds.size.x > 0 && birdBounds.size.y > 0) {
            bool hitTop = birdBounds.position.y < -10.0f;
            bool hitBottom = birdBounds.position.y + birdBounds.size.y > Constants::WINDOW_HEIGHT + 10.0f;

            if (hitTop || hitBottom) {

                if (!hitSoundPlayed) {
                    if (hitSound) hitSound->play();
                    hitSoundPlayed = true;
                }
                game.changeState(std::make_unique<GameOverState>(game, score, getHighScore()));
                return;
            }
        }
    }
}

void GameState::render(sf::RenderWindow& window) {

    if (skySprite) window.draw(*skySprite);

    if (landSprite1) window.draw(*landSprite1);
    if (landSprite2) window.draw(*landSprite2);

    for (const auto& column : columns) {
        column.draw(window);
    }

    bird->draw(window);

    if (fontLoaded) {
        sf::Text scoreText(globalFont);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(32);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(sf::Vector2f(20.0f, 20.0f));
        window.draw(scoreText);
    }

    if (!gameStarted && fontLoaded) {
        sf::Text prompt(globalFont);
        prompt.setString("Press Space to Start");
        prompt.setCharacterSize(36);
        prompt.setFillColor(sf::Color::White);

        sf::FloatRect bounds = prompt.getLocalBounds();
        sf::Vector2f origin(bounds.size.x / 2.0f + bounds.position.x,
                           bounds.size.y / 2.0f + bounds.position.y);
        prompt.setOrigin(origin);
        prompt.setPosition(sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f, Constants::WINDOW_HEIGHT / 2.0f - 50.0f));
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
