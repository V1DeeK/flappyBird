#include "GameState.hpp"
#include "../Game.hpp"
#include "../entities/bird.hpp"
#include "../entities/column.hpp"
#include "../Constants.hpp"
#include "GameOverState.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <algorithm>
#include <fstream>
#include <vector>

// Локальный шрифт для надписи (можно вынести в ResourceManager позже)
static sf::Font globalFont;
static bool fontLoaded = false;

GameState::GameState(Game& game)
    : game(game)
    , gameStarted(false)
    , rng(std::random_device{}())
    , gapYDist(150.0f, Constants::WINDOW_HEIGHT - 150.0f)
    , landOffset(0.0f)
    , hitSoundPlayed(false)
{
    // Создаем дефолтную птичку
    bird = std::make_unique<Bird>();
    // Загружаем шрифт один раз (для надписи "Нажми и лети")
    if (!fontLoaded) {
        if (globalFont.openFromFile("../assets/fonts/ScoreFont.ttf")) {
            fontLoaded = true;
        }
    }
    
    // Загружаем текстуру неба
    std::vector<std::string> skyPaths = {
        "../assets/textures/SkyFonGame.png",
        "assets/textures/SkyFonGame.png"
    };
    for (const auto& path : skyPaths) {
        if (skyTexture.loadFromFile(path)) {
            skySprite = std::make_unique<sf::Sprite>(skyTexture);
            // Растягиваем на весь экран
            float scaleX = static_cast<float>(Constants::WINDOW_WIDTH) / skyTexture.getSize().x;
            float scaleY = static_cast<float>(Constants::WINDOW_HEIGHT) / skyTexture.getSize().y;
            skySprite->setScale(sf::Vector2f(scaleX, scaleY));
            break;
        }
    }
    
    // Загружаем текстуру земли
    std::vector<std::string> landPaths = {
        "../assets/textures/LandGame.png",
        "assets/textures/LandGame.png"
    };
    for (const auto& path : landPaths) {
        if (landTexture.loadFromFile(path)) {
            landSprite1 = std::make_unique<sf::Sprite>(landTexture);
            landSprite2 = std::make_unique<sf::Sprite>(landTexture);
            
            // Вычисляем высоту земли (обычно это нижняя часть экрана)
            float landHeight = static_cast<float>(landTexture.getSize().y);
            float landY = Constants::WINDOW_HEIGHT - landHeight;
            
            // Устанавливаем позиции для двух спрайтов (для бесшовной прокрутки)
            landSprite1->setPosition(sf::Vector2f(0.0f, landY));
            landSprite2->setPosition(sf::Vector2f(static_cast<float>(landTexture.getSize().x), landY));
            break;
        }
    }
    
    // Загружаем звуки
    std::vector<std::string> wingPaths = {
        "../assets/sounds/Wing.wav",
        "assets/sounds/Wing.wav"
    };
    for (const auto& path : wingPaths) {
        if (wingSoundBuffer.loadFromFile(path)) {
            wingSound = std::make_unique<sf::Sound>(wingSoundBuffer);
            break;
        }
    }
    
    std::vector<std::string> hitPaths = {
        "../assets/sounds/Hit.wav",
        "assets/sounds/Hit.wav"
    };
    for (const auto& path : hitPaths) {
        if (hitSoundBuffer.loadFromFile(path)) {
            hitSound = std::make_unique<sf::Sound>(hitSoundBuffer);
            break;
        }
    }
}

void GameState::handleEvents(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        game.window.close();
    }

    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Escape) {
            game.popState(); // вернуться в меню
        }
        if (keyEvent->code == sf::Keyboard::Key::Space) {
            if (!gameStarted) {
                gameStarted = true;
                spawnClock.restart(); // Перезапускаем таймер спавна при старте игры
            }
            bird->jump();
            // Воспроизводим звук крыла
            if (wingSound) wingSound->play();
        }
    }
}

void GameState::update(float dt) {
    if (gameStarted) {
        // Обновляем прокрутку земли
        float landSpeed = Constants::COLUMN_SPEED;
        landOffset -= landSpeed * dt;
        
        // Получаем ширину текстуры земли
        float landWidth = static_cast<float>(landTexture.getSize().x);
        
        // Если первый спрайт ушел за экран, перемещаем его вправо
        if (landOffset <= -landWidth) {
            landOffset += landWidth;
        }
        
            // Обновляем позиции спрайтов земли
            if (landSprite1 && landSprite2) {
                float landHeight = static_cast<float>(landTexture.getSize().y);
                float landY = Constants::WINDOW_HEIGHT - landHeight;
                
                landSprite1->setPosition(sf::Vector2f(landOffset, landY));
                landSprite2->setPosition(sf::Vector2f(landOffset + landWidth, landY));
            }
        
        bird->update(dt);

        // Спавн труб
        // Спавним первую трубу сразу при старте, затем каждые COLUMN_SPAWN_INTERVAL секунд
        if (columns.empty() || spawnClock.getElapsedTime().asSeconds() >= Constants::COLUMN_SPAWN_INTERVAL) {
            float gapY = gapYDist(rng);
            columns.emplace_back(Constants::WINDOW_WIDTH, gapY);
            spawnClock.restart();
        }

        // Обновление труб
        for (auto& column : columns) {
            column.update(dt);
        }

        // Удаление труб за экраном
        columns.erase(
            std::remove_if(columns.begin(), columns.end(),
                [](const Column& c) { return c.getX() + 80.0f < 0; }),
            columns.end()
        );

        // Проверка прохода между трубами
        sf::FloatRect birdBounds = bird->getBounds();
        
        // Проверяем, что границы птички валидны
        if (birdBounds.size.x <= 0 || birdBounds.size.y <= 0) {
            // Птичка не создана или невидима, пропускаем проверки
            return;
        }
        
        for (auto& column : columns) {
            // Если птица прошла трубу (её X больше X трубы + ширина), засчитываем очко
            if (!column.isPassed() && birdBounds.position.x > column.getX() + 80.0f) {
                column.markPassed();
                score++;
            }
        }

        // ========== ПРОВЕРКА КОЛЛИЗИЙ С ТРУБАМИ ==========
        // Коллизии (birdBounds уже определен выше и проверен на валидность) - комментарий
        // birdBounds - границы птицы, полученные из bird->getBounds()
        // Проверены на валидность (size.x > 0 && size.y > 0)
        
        // Создаем уменьшенный прямоугольник для более прощающей коллизии - комментарий
        // collisionScale - коэффициент масштабирования для коллизий (0.7 = 70% от исходного размера)
        // Уменьшаем размеры на 30% чтобы сделать коллизии более прощающими (игроку легче играть)
        // float - число с плавающей точкой
        float collisionScale = 0.7f; // Уменьшаем размеры на 30%
        
        // widthReduction - уменьшение по ширине (отступ с каждой стороны)
        // birdBounds.size.x - исходная ширина птицы
        // (1.0f - collisionScale) - доля уменьшения (0.3 = 30%)
        // / 2.0f - деление на 2 (отступ с каждой стороны)
        // Результат: на сколько пикселей нужно сдвинуть прямоугольник коллизий по X
        float widthReduction = birdBounds.size.x * (1.0f - collisionScale) / 2.0f;
        
        // heightReduction - уменьшение по высоте (отступ с каждой стороны)
        // Аналогично widthReduction, но для высоты
        float heightReduction = birdBounds.size.y * (1.0f - collisionScale) / 2.0f;
        
        // sf::FloatRect collisionBounds(...) - создание уменьшенного прямоугольника для коллизий
        // sf::FloatRect - прямоугольник с координатами float (используется для проверки пересечений)
        // collisionBounds - прямоугольник, используемый для проверки коллизий (меньше реального размера птицы)
        // sf::Vector2f(...) - позиция прямоугольника (с учетом отступов)
        // birdBounds.position.x + widthReduction - X позиция (сдвинута вправо на отступ)
        // birdBounds.position.y + heightReduction - Y позиция (сдвинута вниз на отступ)
        // sf::Vector2f(...) - размер прямоугольника (уменьшенный)
        // birdBounds.size.x * collisionScale - ширина (70% от исходной)
        // birdBounds.size.y * collisionScale - высота (70% от исходной)
        sf::FloatRect collisionBounds(
            sf::Vector2f(birdBounds.position.x + widthReduction, birdBounds.position.y + heightReduction),
            sf::Vector2f(birdBounds.size.x * collisionScale, birdBounds.size.y * collisionScale)
        );
        
        // for (const auto& column : columns) - цикл по всем трубам
        // for - цикл for-each (range-based for loop)
        // const auto& column - переменная цикла: константная ссылка на Column
        // const - не изменяем трубу в цикле
        // auto - автоматическое определение типа (компилятор определит что это Column)
        // & - ссылка (не копия, эффективнее)
        // : columns - контейнер, по которому итерируемся (std::vector<Column>)
        // Проверяем коллизию с каждой трубой в списке
        for (const auto& column : columns) {
            // В SFML 3.0 нужно использовать contains или проверять пересечение вручную - комментарий
            // SFML 3.0 изменил API, поэтому проверяем пересечение вручную через AABB (Axis-Aligned Bounding Box)
            
            // sf::FloatRect topBounds = column.getTopBounds(); - получение границ верхней трубы
            // topBounds - прямоугольник, описывающий границы верхней трубы
            // column.getTopBounds() - метод Column, возвращающий границы верхней трубы
            // Используется для проверки коллизий
            sf::FloatRect topBounds = column.getTopBounds();
            
            // sf::FloatRect bottomBounds = column.getBottomBounds(); - получение границ нижней трубы
            // bottomBounds - прямоугольник, описывающий границы нижней трубы
            // column.getBottomBounds() - метод Column, возвращающий границы нижней трубы
            sf::FloatRect bottomBounds = column.getBottomBounds();
            
            // Проверяем, что границы труб валидны - комментарий
            // if (topBounds.size.x <= 0 || ...) - проверка валидности границ
            // topBounds.size.x <= 0 - проверка что ширина валидна (больше нуля)
            // topBounds.size.y <= 0 - проверка что высота валидна
            // || - логическое ИЛИ (OR) - если хотя бы одно условие истинно
            // bottomBounds.size.x <= 0 || bottomBounds.size.y <= 0 - аналогично для нижней трубы
            // Если границы невалидны, пропускаем эту трубу
            if (topBounds.size.x <= 0 || topBounds.size.y <= 0 ||
                bottomBounds.size.x <= 0 || bottomBounds.size.y <= 0) {
                // continue; - пропуск текущей итерации цикла
                // Переходим к следующей трубе, не проверяя коллизии с этой
                continue; // Пропускаем невалидные трубы
            }
            
            // ========== ПРОВЕРКА ПЕРЕСЕЧЕНИЯ ПО X (труба достигла птицы) ==========
            // Проверка пересечения прямоугольников - комментарий
            // Проверяем только если труба пересекается с птичкой по X - комментарий
            // Труба должна быть в диапазоне от левой границы птички до правой границы птички - комментарий
            // И труба должна быть справа от начальной позиции птички (чтобы не было коллизий с трубами, которые еще не дошли) - комментарий
            
            // bool pipeReachedBird = (...) - проверка что труба достигла птицы по X
            // bool - логический тип (true/false)
            // pipeReachedBird - флаг, указывающий что труба находится в зоне птицы по горизонтали
            // Проверяем пересечение по X перед проверкой по Y (оптимизация: сначала быстрая проверка)
            bool pipeReachedBird = (
                // topBounds.position.x < collisionBounds.position.x + collisionBounds.size.x
                // Левая граница трубы меньше правой границы птицы (труба не прошла полностью мимо справа)
                topBounds.position.x < collisionBounds.position.x + collisionBounds.size.x &&
                // topBounds.position.x + topBounds.size.x > collisionBounds.position.x
                // Правая граница трубы больше левой границы птицы (труба не прошла полностью мимо слева)
                // Оба условия вместе означают: труба пересекается с птицей по X
                topBounds.position.x + topBounds.size.x > collisionBounds.position.x &&
                // topBounds.position.x + topBounds.size.x >= Constants::BIRD_X_POSITION - 50.0f
                // Правая граница трубы >= позиции птицы - 50 пикселей
                // Труба должна быть близко к птичке (чтобы не было коллизий с далекими трубами)
                // Constants::BIRD_X_POSITION - X позиция птицы (150 пикселей)
                // - 50.0f - небольшой запас (50 пикселей)
                topBounds.position.x + topBounds.size.x >= Constants::BIRD_X_POSITION - 50.0f
            ); // Труба должна быть близко к птичке
            
            // if (pipeReachedBird) - если труба достигла птицы по X, проверяем коллизию по Y
            // Проверяем коллизию только если труба находится в зоне птицы по горизонтали
            if (pipeReachedBird) {
                // ========== ПРОВЕРКА КОЛЛИЗИИ С ВЕРХНЕЙ ТРУБОЙ (AABB) ==========
                // bool topCollision = (...) - проверка коллизии с верхней трубой
                // bool - логический тип
                // topCollision - флаг, указывающий что произошла коллизия с верхней трубой
                // AABB (Axis-Aligned Bounding Box) - проверка пересечения двух прямоугольников
                // Два прямоугольника пересекаются если выполняются все 4 условия:
                bool topCollision = (
                    // 1. Левая граница птицы меньше правой границы трубы
                    // collisionBounds.position.x < topBounds.position.x + topBounds.size.x
                    collisionBounds.position.x < topBounds.position.x + topBounds.size.x &&
                    // 2. Правая граница птицы больше левой границы трубы
                    // collisionBounds.position.x + collisionBounds.size.x > topBounds.position.x
                    collisionBounds.position.x + collisionBounds.size.x > topBounds.position.x &&
                    // 3. Верхняя граница птицы меньше нижней границы трубы
                    // collisionBounds.position.y < topBounds.position.y + topBounds.size.y
                    collisionBounds.position.y < topBounds.position.y + topBounds.size.y &&
                    // 4. Нижняя граница птицы больше верхней границы трубы
                    // collisionBounds.position.y + collisionBounds.size.y > topBounds.position.y
                    collisionBounds.position.y + collisionBounds.size.y > topBounds.position.y
                );
                // Если все 4 условия истинны, прямоугольники пересекаются = коллизия
                
                // ========== ПРОВЕРКА КОЛЛИЗИИ С НИЖНЕЙ ТРУБОЙ (AABB) ==========
                // bool bottomCollision = (...) - проверка коллизии с нижней трубой
                // Аналогично topCollision, но для нижней трубы
                bool bottomCollision = (
                    // 1. Левая граница птицы меньше правой границы трубы
                    collisionBounds.position.x < bottomBounds.position.x + bottomBounds.size.x &&
                    // 2. Правая граница птицы больше левой границы трубы
                    collisionBounds.position.x + collisionBounds.size.x > bottomBounds.position.x &&
                    // 3. Верхняя граница птицы меньше нижней границы трубы
                    collisionBounds.position.y < bottomBounds.position.y + bottomBounds.size.y &&
                    // 4. Нижняя граница птицы больше верхней границы трубы
                    collisionBounds.position.y + collisionBounds.size.y > bottomBounds.position.y
                );
                
                // if (topCollision || bottomCollision) - проверка что произошла коллизия с любой трубой
                // || - логическое ИЛИ (OR) - если хотя бы одна коллизия произошла
                // Если птица столкнулась с верхней ИЛИ нижней трубой, игра заканчивается
                if (topCollision || bottomCollision) {
                    // Воспроизводим звук удара - комментарий
                    // if (!hitSoundPlayed) - проверка что звук еще не был воспроизведен
                    // ! - логическое отрицание (NOT)
                    // hitSoundPlayed - флаг, предотвращающий повторное воспроизведение звука
                    if (!hitSoundPlayed) {
                        // if (hitSound) - проверка что звук загружен
                        // hitSound - умный указатель на sf::Sound (может быть nullptr)
                        if (hitSound) {
                            // hitSound->play(); - воспроизведение звука удара
                            // -> - доступ к членам через указатель
                            // play() - метод воспроизведения звука
                            hitSound->play();
                        }
                        // hitSoundPlayed = true; - установка флага (звук воспроизведен)
                        // Предотвращает повторное воспроизведение звука
                        hitSoundPlayed = true;
                    }
                    // game.changeState(...) - переход в состояние окончания игры
                    // game.changeState(...) - метод Game для замены текущего состояния
                    // std::make_unique<GameOverState>(...) - создание нового состояния GameOverState
                    // make_unique - функция, создающая unique_ptr и выделяющая память
                    // GameOverState(...) - конструктор состояния окончания игры
                    // game - ссылка на объект Game (передается в конструктор)
                    // score - текущий счет игрока
                    // getHighScore() - метод получения рекорда из файла
                    game.changeState(std::make_unique<GameOverState>(game, score, getHighScore()));
                    // return; - выход из метода update()
                    // Игра закончена, не нужно продолжать обновление
                    return;
                }
            }
        }

        // Потолок/пол - проверяем только если птичка начала двигаться и границы валидны
        // В SFML 3.0 FloatRect имеет position и size
        // Добавляем небольшой запас, чтобы избежать ложных срабатываний
        if (bird->hasStarted() && birdBounds.size.x > 0 && birdBounds.size.y > 0) {
            bool hitTop = birdBounds.position.y < -10.0f;
            bool hitBottom = birdBounds.position.y + birdBounds.size.y > Constants::WINDOW_HEIGHT + 10.0f;
            
            if (hitTop || hitBottom) {
                // Воспроизводим звук удара
                if (!hitSoundPlayed) {
                    if (hitSound) hitSound->play();
                    hitSoundPlayed = true;
                }
                game.changeState(std::make_unique<GameOverState>(game, score, getHighScore()));
                return;
            }
        }
    }
}

void GameState::render(sf::RenderWindow& window) {
    // Порядок отрисовки слоёв (от дальнего к ближнему):
    // 1. Фон неба (самый дальний слой)
    if (skySprite) window.draw(*skySprite);
    
    // 2. Земля (на фоне неба)
    if (landSprite1) window.draw(*landSprite1);
    if (landSprite2) window.draw(*landSprite2);
    
    // 3. Трубы (на земле)
    for (const auto& column : columns) {
        column.draw(window);
    }
    
    // 4. Птица (поверх всего)
    bird->draw(window);

    // Счёт
    if (fontLoaded) {
        sf::Text scoreText(globalFont);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(32);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(sf::Vector2f(20.0f, 20.0f));
        window.draw(scoreText);
    }

    if (!gameStarted && fontLoaded) {
        sf::Text prompt(globalFont);
        prompt.setString("Press Space to Start");
        prompt.setCharacterSize(36);
        prompt.setFillColor(sf::Color::White);

        sf::FloatRect bounds = prompt.getLocalBounds();
        sf::Vector2f origin(bounds.size.x / 2.0f + bounds.position.x, 
                           bounds.size.y / 2.0f + bounds.position.y);
        prompt.setOrigin(origin);
        prompt.setPosition(sf::Vector2f(Constants::WINDOW_WIDTH / 2.0f, Constants::WINDOW_HEIGHT / 2.0f - 50.0f));
        window.draw(prompt);
    }
}

int GameState::getHighScore() const {
    std::ifstream file(Constants::HIGHSCORE_FILE);
    int highScore = 0;
    if (file >> highScore) {
        return highScore;
    }
    return 0;
}
