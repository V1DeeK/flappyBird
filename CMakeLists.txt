cmake_minimum_required(VERSION 3.15)
project(Flappy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск SFML
find_package(SFML 3 COMPONENTS Graphics Audio REQUIRED)

# Генерация compile_commands.json для IDE
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Исходные файлы (только .cpp файлы)
set(SOURCES
    src/main.cpp
    src/game.cpp
    src/entities/bird.cpp
    src/entities/column.cpp
    src/states/GameState.cpp
    src/states/MainMenuState.cpp
    src/states/GameOverState.cpp
    src/ui/Button.cpp
)

# Исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Подключение SFML (это автоматически добавит include директории)
target_link_libraries(${PROJECT_NAME}
    SFML::Graphics
    SFML::Audio
)

# Явно указываем include директории для IDE
# SFML::Graphics уже добавляет их автоматически, но для IDE нужно явно указать
target_include_directories(${PROJECT_NAME} PRIVATE
    C:/SFML302/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Настройки для Windows
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()

