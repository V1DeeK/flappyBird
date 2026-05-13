cmake_minimum_required(VERSION 3.15)
project(Flappy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 COMPONENTS Graphics Audio REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    SFML::Graphics
    SFML::Audio
)

target_include_directories(${PROJECT_NAME} PRIVATE
    C:/SFML302/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()
