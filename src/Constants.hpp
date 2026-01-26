#pragma once

namespace Constants
{
    // Window settings
    inline constexpr int WINDOW_HEIGHT = 800;
    inline constexpr int WINDOW_WIDTH = 600;
    inline constexpr const char* WINDOW_TITLE = "Flappy Bird";
    inline constexpr int FRAMERATE_LIMIT = 60;

    // Bird physics
    inline constexpr float GRAVITY = 980.0f; 
    inline constexpr float BIRD_JUMP_IMPULSE = -400.0f;
    inline constexpr float BIRD_X_POSITION = 150.0f;
    inline constexpr float BIRD_Y_OFFSET = 15.0f; // Offset from center
    inline constexpr float MAX_FALL_SPEED = 1000.0f;
    inline constexpr float MAX_DELTA_TIME = 0.1f; // Max 100ms per frame
    inline constexpr float JUMP_ANIMATION_TIME = 0.15f;

    // Column settings
    inline constexpr float COLUMN_SPEED = 220.0f;
    inline constexpr float COLUMN_GAP = 200.0f;
    inline constexpr float COLUMN_SPAWN_INTERVAL = 1.5f;
    inline constexpr float PIPE_WIDTH = 80.0f;
    inline constexpr float MIN_PIPE_HEIGHT = 100.0f;
    inline constexpr float GAP_Y_MIN = 150.0f;
    inline constexpr float GAP_Y_MAX_OFFSET = 150.0f; // WINDOW_HEIGHT - GAP_Y_MAX_OFFSET

    // Collision settings
    inline constexpr float COLLISION_SCALE = 0.7f; // 70% of original size
    inline constexpr float PIPE_COLLISION_OFFSET = 50.0f; // Distance check for pipe collision
    inline constexpr float BOUNDARY_COLLISION_MARGIN = 10.0f; // Margin for top/bottom collision

    // UI settings
    inline constexpr int FONT_SIZE_DEFAULT = 32;
    inline constexpr int FONT_SIZE_LARGE = 36;
    inline constexpr int FONT_SIZE_TITLE = 48;
    
    // Button settings
    inline constexpr float BUTTON_WIDTH = 250.0f;
    inline constexpr float BUTTON_HEIGHT = 60.0f;
    inline constexpr float BUTTON_X_OFFSET = 125.0f; // WINDOW_WIDTH / 2.0f - BUTTON_X_OFFSET
    
    // MainMenu button positions
    inline constexpr float MENU_PLAY_BUTTON_Y = 200.0f; // WINDOW_HEIGHT / 2.0f + MENU_PLAY_BUTTON_Y
    inline constexpr float MENU_EXIT_BUTTON_Y = 280.0f; // WINDOW_HEIGHT / 2.0f + MENU_EXIT_BUTTON_Y
    
    // GameOver button positions
    inline constexpr float GAMEOVER_RESTART_BUTTON_Y = 50.0f; // WINDOW_HEIGHT / 2.0f + GAMEOVER_RESTART_BUTTON_Y
    inline constexpr float GAMEOVER_EXIT_BUTTON_Y = 130.0f; // WINDOW_HEIGHT / 2.0f + GAMEOVER_EXIT_BUTTON_Y
    
    // Text positions
    inline constexpr float SCORE_TEXT_X = 20.0f;
    inline constexpr float SCORE_TEXT_Y = 20.0f;
    inline constexpr float PROMPT_TEXT_Y_OFFSET = 50.0f; // WINDOW_HEIGHT / 2.0f - PROMPT_TEXT_Y_OFFSET
    inline constexpr float GAMEOVER_TITLE_Y = 150.0f;
    inline constexpr float GAMEOVER_SCORE_Y = 250.0f;
    inline constexpr float HIGHSCORE_TEXT_X_OFFSET = 20.0f; // WINDOW_WIDTH - HIGHSCORE_TEXT_X_OFFSET
    inline constexpr float HIGHSCORE_TEXT_Y = 20.0f;

    // File paths
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
