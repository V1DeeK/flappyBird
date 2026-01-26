#pragma once

namespace Constants
{
    inline constexpr int WINDOW_HEIGHT = 800;
    inline constexpr int WINDOW_WIDTH = 600;
    inline constexpr const char* WINDOW_TITLE = "Flappy Bird";
    inline constexpr int FRAMERATE_LIMIT = 60;

    inline constexpr float GRAVITY = 980.0f; 
    inline constexpr float BIRD_JUMP_IMPULSE = -400.0f;
    inline constexpr float BIRD_X_POSITION = 150.0f;
    inline constexpr float BIRD_Y_OFFSET = 15.0f;
    inline constexpr float MAX_FALL_SPEED = 1000.0f;
    inline constexpr float MAX_DELTA_TIME = 0.1f;
    inline constexpr float JUMP_ANIMATION_TIME = 0.15f;

    inline constexpr float COLUMN_SPEED = 220.0f;
    inline constexpr float COLUMN_GAP = 200.0f;
    inline constexpr float COLUMN_SPAWN_INTERVAL = 1.5f;
    inline constexpr float PIPE_WIDTH = 80.0f;
    inline constexpr float MIN_PIPE_HEIGHT = 100.0f;
    inline constexpr float GAP_Y_MIN = 150.0f;
    inline constexpr float GAP_Y_MAX_OFFSET = 150.0f;

    inline constexpr float COLLISION_SCALE = 0.7f;
    inline constexpr float PIPE_COLLISION_OFFSET = 50.0f;
    inline constexpr float BOUNDARY_COLLISION_MARGIN = 10.0f;

    inline constexpr int FONT_SIZE_DEFAULT = 32;
    inline constexpr int FONT_SIZE_LARGE = 36;
    inline constexpr int FONT_SIZE_TITLE = 48;
    
    inline constexpr float BUTTON_WIDTH = 250.0f;
    inline constexpr float BUTTON_HEIGHT = 60.0f;
    inline constexpr float BUTTON_X_OFFSET = 125.0f;
    
    inline constexpr float MENU_PLAY_BUTTON_Y = 200.0f;
    inline constexpr float MENU_EXIT_BUTTON_Y = 280.0f;
    
    inline constexpr float GAMEOVER_RESTART_BUTTON_Y = 50.0f;
    inline constexpr float GAMEOVER_EXIT_BUTTON_Y = 130.0f;
    
    inline constexpr float SCORE_TEXT_X = 20.0f;
    inline constexpr float SCORE_TEXT_Y = 20.0f;
    inline constexpr float PROMPT_TEXT_Y_OFFSET = 50.0f;
    inline constexpr float GAMEOVER_TITLE_Y = 150.0f;
    inline constexpr float GAMEOVER_SCORE_Y = 250.0f;
    inline constexpr float HIGHSCORE_TEXT_X_OFFSET = 20.0f;
    inline constexpr float HIGHSCORE_TEXT_Y = 20.0f;

    inline constexpr const char* HIGHSCORE_FILE = "highscore.txt";
}
