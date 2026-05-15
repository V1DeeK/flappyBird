// #include "bird.hpp" - подключаем заголовочный файл с объявлением класса Bird
#include "bird.hpp"

// #include "../Constants.hpp" - подключаем файл с константами игры
// Нужен для Constants::WINDOW_HEIGHT, Constants::BIRD_X_POSITION, Constants::GRAVITY, Constants::BIRD_JUMP_IMPULSE
#include "../Constants.hpp"

// #include <iostream> - подключаем библиотеку для ввода/вывода (std::cout, std::cerr)
// В данном файле не используется, но оставлен для совместимости
#include <iostream>

// #include <vector> - подключаем библиотеку для std::vector (динамический массив)
// Используется для хранения путей к текстурам
#include <vector>

// Bird::Bird() - определение конструктора по умолчанию
// Bird:: - оператор области видимости, указывает что это метод класса Bird
// () - конструктор не принимает параметров
Bird::Bird() {
    // Загружаем текстуры птицы по умолчанию (fbird) - комментарий
    // loadTextures(...) - вызов метода загрузки текстур с дефолтными путями
    // "assets/bird/fbird1.png" - путь к первой текстуре (дефолтное состояние)
    // "assets/bird/fbird2.png" - путь ко второй текстуре (при нажатии пробела)
    // "assets/bird/fbird3.png" - путь к третьей текстуре (при подъеме)
    // "assets/bird/fbird4.png" - путь к четвертой текстуре (при падении)
    loadTextures("assets/bird/fbird1.png", "assets/bird/fbird2.png", 
                 "assets/bird/fbird3.png", "assets/bird/fbird4.png");
}

// Bird::Bird(...) - определение конструктора с параметрами
// const std::string& texturePath1 - параметр: путь к первой текстуре
// const - параметр не будет изменен
// std::string& - ссылка на строку (не копируется)
// texturePath2, texturePath3, texturePath4 - пути к остальным текстурам
Bird::Bird(const std::string& texturePath1, const std::string& texturePath2, 
           const std::string& texturePath3, const std::string& texturePath4) {
    // loadTextures(...) - вызов метода загрузки текстур с переданными путями
    // Передаем пути, которые были переданы в конструктор
    loadTextures(texturePath1, texturePath2, texturePath3, texturePath4);
}

// void Bird::loadTextures(...) - определение метода загрузки текстур
// void - метод ничего не возвращает
// loadTextures - имя метода
// const std::string& path1 - параметр: путь к первой текстуре
// path2, path3, path4 - пути к остальным текстурам
void Bird::loadTextures(const std::string& path1, const std::string& path2, 
                        const std::string& path3, const std::string& path4) {
    // Пробуем разные пути для каждой текстуры - комментарий
    
    // std::vector<std::string> paths1 = {"../" + path1, path1}; - создание вектора путей для первой текстуры
    // std::vector<std::string> - динамический массив строк
    // paths1 - имя переменной
    // {"../" + path1, path1} - список инициализации (2 элемента)
    // "../" + path1 - конкатенация строк (добавляем "../" перед путем для поиска в родительской директории)
    // path1 - оригинальный путь (для поиска в текущей директории)
    // Пробуем оба пути, потому что программа может запускаться из разных директорий (build/ или корень проекта)
    std::vector<std::string> paths1 = {"../" + path1, path1};
    std::vector<std::string> paths2 = {"../" + path2, path2};
    std::vector<std::string> paths3 = {"../" + path3, path3};
    std::vector<std::string> paths4 = {"../" + path4, path4};
    
    // bool loaded1 = false, loaded2 = false, loaded3 = false, loaded4 = false; - флаги успешной загрузки текстур
    // bool - логический тип (true/false)
    // loaded1 - флаг загрузки первой текстуры
    // = false - инициализация значением false (текстура еще не загружена)
    // , - разделитель между объявлениями переменных
    // loaded2, loaded3, loaded4 - флаги для остальных текстур
    bool loaded1 = false, loaded2 = false, loaded3 = false, loaded4 = false;
    
    // for (const auto& path : paths1) - цикл по всем путям для первой текстуры
    // for - цикл for-each (range-based for loop)
    // const auto& path - переменная цикла: константная ссылка на строку
    // auto - автоматическое определение типа (компилятор сам определит что это std::string)
    // & - ссылка (не копия, эффективнее)
    // : paths1 - контейнер, по которому итерируемся
    // Пробуем загрузить текстуру по каждому пути, пока не найдем рабочий
    for (const auto& path : paths1) {
        // if (texture1.loadFromFile(path)) - проверка успешной загрузки текстуры
        // texture1.loadFromFile(path) - метод загрузки текстуры из файла
        // loadFromFile() - возвращает true если загрузка успешна, false если нет
        // path - путь к файлу текстуры
        if (texture1.loadFromFile(path)) {
            // loaded1 = true; - устанавливаем флаг успешной загрузки
            loaded1 = true;
            // break; - выход из цикла (текстура загружена, не нужно пробовать другие пути)
            break;
        }
    }
    // Аналогичные циклы для остальных текстур
    for (const auto& path : paths2) {
        if (texture2.loadFromFile(path)) {
            loaded2 = true;
            break;
        }
    }
    for (const auto& path : paths3) {
        if (texture3.loadFromFile(path)) {
            loaded3 = true;
            break;
        }
    }
    for (const auto& path : paths4) {
        if (texture4.loadFromFile(path)) {
            loaded4 = true;
            break;
        }
    }
    
    // Если хотя бы одна текстура не загрузилась, используем дефолтные текстуры fbird - комментарий
    // if (!loaded1 || !loaded2 || !loaded3 || !loaded4) - проверка что хотя бы одна текстура не загрузилась
    // ! - логическое отрицание (NOT)
    // || - логическое ИЛИ (OR) - условие истинно если хотя бы один флаг false
    // Если хотя бы одна текстура не загрузилась, используем дефолтные текстуры
    if (!loaded1 || !loaded2 || !loaded3 || !loaded4) {
        
        // Загружаем дефолтные текстуры fbird - комментарий
        // std::vector<std::string> defaultPaths1 = {...} - создание вектора с дефолтными путями
        // {"../assets/bird/fbird1.png", "assets/bird/fbird1.png"} - два пути для поиска дефолтной текстуры
        std::vector<std::string> defaultPaths1 = {"../assets/bird/fbird1.png", "assets/bird/fbird1.png"};
        std::vector<std::string> defaultPaths2 = {"../assets/bird/fbird2.png", "assets/bird/fbird2.png"};
        std::vector<std::string> defaultPaths3 = {"../assets/bird/fbird3.png", "assets/bird/fbird3.png"};
        std::vector<std::string> defaultPaths4 = {"../assets/bird/fbird4.png", "assets/bird/fbird4.png"};
        
        // if (!loaded1) - проверка что первая текстура не загрузилась
        // Если не загрузилась, пробуем загрузить дефолтную
        if (!loaded1) {
            // for (const auto& path : defaultPaths1) - цикл по дефолтным путям
            for (const auto& path : defaultPaths1) {
                // if (texture1.loadFromFile(path)) - попытка загрузить дефолтную текстуру
                if (texture1.loadFromFile(path)) {
                    loaded1 = true;
                    break;
                }
            }
        }
        // Аналогично для остальных текстур
        if (!loaded2) {
            for (const auto& path : defaultPaths2) {
                if (texture2.loadFromFile(path)) {
                    loaded2 = true;
                    break;
                }
            }
        }
        if (!loaded3) {
            for (const auto& path : defaultPaths3) {
                if (texture3.loadFromFile(path)) {
                    loaded3 = true;
                    break;
                }
            }
        }
        if (!loaded4) {
            for (const auto& path : defaultPaths4) {
                if (texture4.loadFromFile(path)) {
                    loaded4 = true;
                    break;
                }
            }
        }
    }
    
    // Проверяем, что хотя бы первая текстура загружена перед созданием спрайта - комментарий
    // if (loaded1) - проверка что первая текстура загружена
    // Если первая текстура загружена, создаем спрайт
    if (loaded1) {
        // sprite = std::make_unique<sf::Sprite>(texture1); - создание умного указателя на спрайт
        // sprite - член класса (std::unique_ptr<sf::Sprite>)
        // = - присваивание
        // std::make_unique<sf::Sprite>(texture1) - создание unique_ptr и выделение памяти для Sprite
        // make_unique - функция, создающая unique_ptr
        // <sf::Sprite> - шаблонный параметр (тип объекта)
        // (texture1) - параметр конструктора Sprite (текстура для спрайта)
        // Sprite использует текстуру для отрисовки изображения на экране
        sprite = std::make_unique<sf::Sprite>(texture1);
        
        // float birdY = Constants::WINDOW_HEIGHT / 2.0f - 15.0f; - вычисление Y позиции птицы
        // float - число с плавающей точкой
        // birdY - переменная для Y координаты птицы
        // Constants::WINDOW_HEIGHT - высота окна (800)
        // / 2.0f - деление на 2 (центр экрана по вертикали)
        // - 15.0f - небольшое смещение вверх (чтобы птица была чуть выше центра)
        // 2.0f - суффикс f указывает что это float
        float birdY = Constants::WINDOW_HEIGHT / 2.0f - 15.0f;
        
        // sprite->setPosition(sf::Vector2f(Constants::BIRD_X_POSITION, birdY)); - установка позиции спрайта
        // sprite-> - доступ к членам через указатель (-> для указателей, . для объектов)
        // setPosition(...) - метод установки позиции спрайта на экране
        // sf::Vector2f(...) - вектор из 2 float (x, y)
        // Constants::BIRD_X_POSITION - X позиция птицы (150 пикселей от левого края)
        // birdY - Y позиция птицы (вычислена выше)
        sprite->setPosition(sf::Vector2f(Constants::BIRD_X_POSITION, birdY));
    } else {
        // Создаем пустой спрайт, чтобы избежать краша - комментарий
        // sprite = nullptr; - установка указателя в nullptr (пустой указатель)
        // nullptr - специальное значение, означающее "ничего не указывает"
        // Если текстура не загрузилась, спрайт будет nullptr
        // Перед использованием спрайта нужно проверять на nullptr
        sprite = nullptr;
    }
}

// void Bird::update(float deltaTime) - определение метода обновления птицы
// void - метод ничего не возвращает
// float deltaTime - параметр: время в секундах с последнего обновления
// deltaTime используется для плавного движения независимо от FPS
void Bird::update(float deltaTime) {
    // if (started && sprite) - проверка что птица начала двигаться и спрайт существует
    // started - флаг начала движения (true после первого прыжка)
    // && - логическое И (AND) - оба условия должны быть true
    // sprite - проверка что указатель не nullptr (если nullptr, то false)
    // Обновляем птицу только если она начала двигаться и спрайт загружен
    if (started && sprite) {
        // Ограничиваем deltaTime, чтобы избежать больших скачков - комментарий
        // if (deltaTime > 0.1f) - проверка что deltaTime не слишком большой
        // 0.1f - 100 миллисекунд (максимальное время между кадрами)
        // Если deltaTime слишком большой (например, игра была приостановлена), ограничиваем его
        // Это предотвращает "телепортацию" объектов при больших задержках
        if (deltaTime > 0.1f) {
            // deltaTime = 0.1f; - ограничение deltaTime максимумом 100ms
            deltaTime = 0.1f; // Максимум 100ms за кадр
        }
        
        // velocity.y += Constants::GRAVITY * deltaTime; - применение гравитации к скорости
        // velocity.y - Y компонента скорости (вертикальная скорость)
        // += - оператор сложения с присваиванием (эквивалентно velocity.y = velocity.y + ...)
        // Constants::GRAVITY - сила тяжести (980 пикселей/секунду²)
        // * deltaTime - умножение на время (получаем изменение скорости)
        // Гравитация увеличивает скорость падения каждый кадр
        // Положительное значение velocity.y означает движение вниз
        velocity.y += Constants::GRAVITY * deltaTime;
        
        // Ограничиваем максимальную скорость падения - комментарий
        // if (velocity.y > 1000.0f) - проверка что скорость не превышает максимум
        // 1000.0f - максимальная скорость падения (1000 пикселей/секунду)
        // Ограничение предотвращает слишком быстрое падение птицы
        if (velocity.y > 1000.0f) {
            // velocity.y = 1000.0f; - установка максимальной скорости
            velocity.y = 1000.0f;
        }
        
        // sf::Vector2f moveDelta = velocity * deltaTime; - вычисление смещения за кадр
        // sf::Vector2f - вектор из 2 float (x, y)
        // moveDelta - переменная для смещения (на сколько пикселей сдвинуть птицу)
        // velocity - вектор скорости птицы
        // * deltaTime - умножение скорости на время (получаем расстояние)
        // Формула: расстояние = скорость * время
        sf::Vector2f moveDelta = velocity * deltaTime;
        
        // sprite->move(moveDelta); - перемещение спрайта на вычисленное смещение
        // sprite-> - доступ к членам через указатель
        // move(...) - метод перемещения спрайта на заданный вектор
        // moveDelta - вектор смещения (на сколько сдвинуть)
        // Спрайт перемещается на новую позицию
        sprite->move(moveDelta);
        
        // Обновляем анимацию - комментарий
        // if (isJumping) - проверка что птица сейчас прыгает
        // isJumping - флаг активного прыжка
        if (isJumping) {
            // jumpTimer += deltaTime; - увеличение таймера прыжка
            // jumpTimer - таймер анимации прыжка (накапливает время)
            // += deltaTime - добавляем прошедшее время к таймеру
            jumpTimer += deltaTime;
            
            // if (jumpTimer < JUMP_ANIMATION_TIME / 2.0f) - проверка первой половины прыжка
            // jumpTimer - текущее время прыжка
            // JUMP_ANIMATION_TIME - общее время анимации прыжка (0.15 секунды)
            // / 2.0f - деление на 2 (половина времени)
            // Если прошло меньше половины времени прыжка, показываем texture2
            if (jumpTimer < JUMP_ANIMATION_TIME / 2.0f) {
                // Первая половина прыжка - texture2 - комментарий
                // sprite->setTexture(texture2); - установка текстуры для первой половины прыжка
                // setTexture(...) - метод установки текстуры спрайта
                // texture2 - вторая текстура (fbird2.png - при нажатии пробела)
                sprite->setTexture(texture2);
            // else if (jumpTimer < JUMP_ANIMATION_TIME) - проверка второй половины прыжка
            // Если прошло больше половины, но меньше полного времени прыжка
            } else if (jumpTimer < JUMP_ANIMATION_TIME) {
                // Вторая половина прыжка - texture3 - комментарий
                // sprite->setTexture(texture3); - установка текстуры для второй половины прыжка
                // texture3 - третья текстура (fbird3.png - поднимается)
                sprite->setTexture(texture3);
            // else - прыжок закончен
            } else {
                // Прыжок закончен - комментарий
                // isJumping = false; - сброс флага прыжка
                // Прыжок завершен, переходим к обычной анимации
                isJumping = false;
                // jumpTimer = 0.0f; - сброс таймера прыжка
                jumpTimer = 0.0f;
            }
        // else - птица не прыгает, используем обычную анимацию
        } else {
            // Если птица опускается (velocity.y > 0), используем texture4 - комментарий
            // if (velocity.y > 0) - проверка что скорость положительная (падение вниз)
            // velocity.y > 0 означает что птица падает (движется вниз)
            if (velocity.y > 0) {
                // sprite->setTexture(texture4); - установка текстуры для падения
                // texture4 - четвертая текстура (fbird4.png - опускается)
                sprite->setTexture(texture4);
            // else - птица поднимается или стоит
            } else {
                // Если поднимается или стоит, используем texture1 - комментарий
                // sprite->setTexture(texture1); - установка текстуры для подъема/стояния
                // texture1 - первая текстура (fbird1.png - дефолтное состояние)
                sprite->setTexture(texture1);
            }
        }
    }
}

// void Bird::jump() - определение метода прыжка
// void - метод ничего не возвращает
// jump() - вызывается при нажатии пробела
void Bird::jump() {
    // started = true; - установка флага начала движения
    // started - флаг, указывающий что птица начала двигаться
    // После первого прыжка птица начинает двигаться и проверяются коллизии
    started = true;
    
    // Сбрасываем скорость перед прыжком, чтобы избежать накопления - комментарий
    // velocity.y = Constants::BIRD_JUMP_IMPULSE; - установка скорости прыжка
    // velocity.y - Y компонента скорости (вертикальная скорость)
    // = - присваивание
    // Constants::BIRD_JUMP_IMPULSE - импульс прыжка (-400 пикселей/секунду)
    // Отрицательное значение означает движение вверх (Y уменьшается в SFML)
    // Импульс - мгновенное изменение скорости (не зависит от предыдущей скорости)
    velocity.y = Constants::BIRD_JUMP_IMPULSE;
    
    // isJumping = true; - установка флага активного прыжка
    // isJumping - флаг, указывающий что птица сейчас прыгает
    // Используется для анимации прыжка (переключение текстур)
    isJumping = true;
    
    // jumpTimer = 0.0f; - сброс таймера прыжка
    // jumpTimer - таймер анимации прыжка (начинаем отсчет с нуля)
    jumpTimer = 0.0f;
    
    // if (sprite) - проверка что спрайт существует
    // sprite - проверка что указатель не nullptr
    if (sprite) {
        // sprite->setTexture(texture2); - установка текстуры прыжка
        // setTexture(...) - метод установки текстуры спрайта
        // texture2 - вторая текстура (fbird2.png - при нажатии пробела)
        // Сразу устанавливаем texture2 для мгновенной реакции на прыжок
        sprite->setTexture(texture2); // Сразу устанавливаем texture2
    }
}

// void Bird::draw(sf::RenderWindow& window) const - определение метода отрисовки
// void - метод ничего не возвращает
// draw - метод отрисовки птицы на экране
// sf::RenderWindow& window - параметр: ссылка на окно для отрисовки
// & - ссылка (не копия, передается оригинал)
// const - метод не изменяет состояние объекта (только читает)
void Bird::draw(sf::RenderWindow& window) const {
    // if (sprite) - проверка что спрайт существует
    // sprite - проверка что указатель не nullptr
    // Если спрайт существует, рисуем его
    if (sprite) {
        // window.draw(*sprite); - отрисовка спрайта на окне
        // window.draw(...) - метод отрисовки объекта на окне
        // *sprite - разыменование указателя (получаем сам объект Sprite)
        // * - оператор разыменования (для указателей)
        // Спрайт рисуется на экране в текущей позиции
        window.draw(*sprite);
    }
}

// sf::FloatRect Bird::getBounds() const - определение метода получения границ
// sf::FloatRect - возвращаемый тип (прямоугольник с координатами float)
// getBounds - метод получения границ птицы для проверки коллизий
// const - метод не изменяет объект
// Используется в GameState для проверки столкновений с трубами и границами экрана
sf::FloatRect Bird::getBounds() const {
    // if (sprite) - проверка что спрайт существует
    // sprite - проверка что указатель не nullptr
    if (sprite) {
        // sf::FloatRect bounds = sprite->getGlobalBounds(); - получение глобальных границ спрайта
        // sf::FloatRect - прямоугольник с координатами float
        // bounds - переменная для хранения границ
        // sprite->getGlobalBounds() - метод получения границ спрайта в глобальных координатах
        // getGlobalBounds() - возвращает прямоугольник, описывающий область спрайта на экране
        // Границы включают позицию и размер спрайта
        sf::FloatRect bounds = sprite->getGlobalBounds();
        
        // return bounds; - возврат границ
        // return - возврат значения из функции
        // bounds - прямоугольник с границами птицы
        return bounds;
    }
    // Возвращаем пустой прямоугольник с нулевым размером - комментарий
    // return sf::FloatRect(...) - возврат пустого прямоугольника если спрайт не существует
    // sf::FloatRect(...) - создание прямоугольника
    // sf::Vector2f(0.0f, 0.0f) - позиция (0, 0)
    // sf::Vector2f(0.0f, 0.0f) - размер (0, 0) - пустой прямоугольник
    // В SFML 3.0 конструктор принимает Vector2 для position и size
    // Если спрайт не существует, возвращаем пустой прямоугольник (не будет коллизий)
    return sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f));
}
