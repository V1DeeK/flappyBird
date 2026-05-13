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
