#include "Game.hpp"
#include "Constants.hpp"
#include "states/MainMenuState.hpp"
#include <optional>

Game::Game()
    : window(sf::VideoMode(sf::Vector2u(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT)),
             Constants::WINDOW_TITLE)
{
    window.setFramerateLimit(60);

    pushState(std::make_unique<MainMenuState>(*this));
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (!states.empty()) {
                states.top()->handleEvents(*event);
            }
        }

        if (!states.empty()) {
            states.top()->update(deltaTime);
            window.clear();
            states.top()->render(window);
            window.display();
        } else {
            window.close();
        }
    }
}

void Game::pushState(std::unique_ptr<State> state) {
    states.push(std::move(state));
}

void Game::changeState(std::unique_ptr<State> state) {
    if (!states.empty())
        states.pop();
    states.push(std::move(state));
}

void Game::popState() {
    if (!states.empty()) {
        states.pop();
    }
}
