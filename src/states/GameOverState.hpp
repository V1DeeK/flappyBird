#pragma once

#include "../State.hpp"
#include "../ui/Button.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>

class Game; // forward declaration

class GameOverState : public State {
public:
    explicit GameOverState(Game& game, int score, int highScore);

    void handleEvents(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    Game& game;
    int score;
    int highScore;
    
    // UI элементы
    Button restartButton;
    Button exitButton;
    sf::Font font;
    std::unique_ptr<sf::Text> scoreText;
    std::unique_ptr<sf::Text> highScoreText;
    std::unique_ptr<sf::Text> gameOverText;
    bool fontLoaded = false;
    
    // Фон
    sf::Texture backgroundTexture;
    std::unique_ptr<sf::Sprite> backgroundSprite;
};

