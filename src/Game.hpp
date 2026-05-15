#pragma once

#include "State.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>

class Game {
public:
    Game();
    void run();

    // Сделаем window public для упрощения (можно заменить на геттер позже)
    sf::RenderWindow window;

    // Методы управления состояниями
    void pushState(std::unique_ptr<State> state);
    void popState();
    void changeState(std::unique_ptr<State> state);

private:
    std::stack<std::unique_ptr<State>> states;
};

/*
    что такое       #include <memory>  , напомни в чем отличие от define 
    что такое #include <stack>

    что такое public и private

    чет я вообще эти буквы не понял std::stack<std::unique_ptr<State>> states;
    unique_ptr Этот указатель единственный владелец объекта.
Как только указатель уничтожится — объект автоматически удалится

*/;