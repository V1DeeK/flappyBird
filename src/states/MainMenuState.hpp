#pragma once

#include "../State.hpp"
#include "../ui/Button.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class Game; // forward declaration

class MainMenuState : public State {
public:
    explicit MainMenuState(Game& game);

    void handleEvents(const sf::Event& event) override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;

private:
    Game& game;
    Button playButton;
    Button exitButton;
    
    // Фон
    sf::Texture backgroundTexture;
    std::unique_ptr<sf::Sprite> backgroundSprite;
};