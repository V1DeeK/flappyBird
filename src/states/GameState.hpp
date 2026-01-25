#pragma once

#include "../State.hpp"
#include "../entities/bird.hpp"
#include "../entities/column.hpp"
#include <memory>
#include <vector>
#include <random>
#include <SFML/System/Clock.hpp>
#include <SFML/Audio.hpp>

class Game; // forward declaration

class GameState : public State {
public:
    explicit GameState(Game& game);

    void handleEvents(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    Game& game;
    bool gameStarted = false;
    std::unique_ptr<Bird> bird;
    std::vector<Column> columns;
    sf::Clock spawnClock;
    std::mt19937 rng;
    std::uniform_real_distribution<float> gapYDist;
    int score = 0;
    
    // Текстуры и спрайты
    sf::Texture skyTexture;
    std::unique_ptr<sf::Sprite> skySprite;
    sf::Texture landTexture;
    std::unique_ptr<sf::Sprite> landSprite1;
    std::unique_ptr<sf::Sprite> landSprite2;
    float landOffset = 0.0f;
    
    // Звуки
    sf::SoundBuffer wingSoundBuffer;
    std::unique_ptr<sf::Sound> wingSound;
    sf::SoundBuffer hitSoundBuffer;
    std::unique_ptr<sf::Sound> hitSound;
    bool hitSoundPlayed = false;
    
    int getHighScore() const;
};
