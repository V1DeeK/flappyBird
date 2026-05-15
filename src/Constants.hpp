#pragma once

namespace Constants
{

    inline constexpr int WINDOW_HEIGHT = 800;
    inline constexpr int WINDOW_WIDTH = 600;
    inline constexpr const char* WINDOW_TITLE = "Flappy Bird";

    inline constexpr float GRAVITY = 980.0f; 
    inline constexpr float BIRD_JUMP_IMPULSE = -400.0f;
    inline constexpr float BIRD_X_POSITION = 150.0f;   

    inline constexpr float COLUMN_SPEED = 220.0f;
    inline constexpr float COLUMN_GAP = 200.0f;
    inline constexpr float COLUMN_SPAWN_INTERVAL = 1.5f;

    inline constexpr int FONT_SIZE = 32;

    inline constexpr const char* HIGHSCORE_FILE = "highscore.txt";
}

    //размеры окна игры 
    /*
    constexpr —(константа времени компиляции) значение известно на этапе компиляции (компилятор может его подставить напрямую).
    inline — позволяет определить переменную в заголовке, не вызывая ошибки "multiple definition", если файл подключён в нескольких .cpp
    Без inline при линковке будет ошибка: «уже определено в другом файле».
    char* Это указатель на строку в стиле C (не C++).

    "Flappy Bird" — это литерал строки, хранится в памяти как массив символов: 'F','l','a',...,'\0'.
    const char* — указатель на первый символ этой строки.
    */
