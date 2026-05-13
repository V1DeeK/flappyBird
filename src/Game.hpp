#pragma once

#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>

class Game {
public:
    Game();
    void run();
    sf::RenderWindow window;
    void pushState(std::unique_ptr<State> state);
    void popState();
    void changeState(std::unique_ptr<State> state);

private:
    std::stack<std::unique_ptr<State>> states;
};
