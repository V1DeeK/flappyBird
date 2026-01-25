// #pragma once - директива препроцессора, предотвращающая множественное включение
#pragma once

// #include <SFML/Graphics.hpp> - подключаем графическую библиотеку SFML
// Нужна для Sprite, Texture, RenderWindow, Vector2f, FloatRect
#include <SFML/Graphics.hpp>

// #include <memory> - подключаем библиотеку для умных указателей
// Нужна для std::unique_ptr
#include <memory>

// class Bird - объявление класса Bird (игровая сущность - птица)
// Bird - класс, представляющий игрока (птицу) в игре Flappy Bird
class Bird {
// public: - публичная секция (методы доступны извне)
public:
    // Bird(); - конструктор по умолчанию
    // Создает птицу с дефолтными текстурами (fbird1-4)
    Bird();
    
    // Bird(...) - конструктор с параметрами (пути к текстурам)
    // const std::string& texturePath1 - путь к первой текстуре (дефолтное состояние)
    // const std::string& - константная ссылка на строку (не копируется, передается ссылка)
    // & - ссылка (не копия, эффективнее)
    // const - строка не будет изменена внутри конструктора
    // texturePath2 - путь ко второй текстуре (при нажатии пробела)
    // texturePath3 - путь к третьей текстуре (при подъеме)
    // texturePath4 - путь к четвертой текстуре (при падении)
    Bird(const std::string& texturePath1, const std::string& texturePath2, 
         const std::string& texturePath3, const std::string& texturePath4);

    // void update(float deltaTime); - метод обновления состояния птицы
    // update - обновляет позицию, скорость, анимацию птицы
    // float deltaTime - время в секундах с последнего обновления
    // Используется для плавного движения независимо от FPS
    void update(float deltaTime);
    
    // void jump(); - метод прыжка птицы
    // jump - вызывается при нажатии пробела, заставляет птицу подпрыгнуть
    // Устанавливает скорость вверх и запускает анимацию прыжка
    void jump();
    
    // void draw(sf::RenderWindow& window) const; - метод отрисовки птицы
    // draw - рисует спрайт птицы на экране
    // sf::RenderWindow& window - ссылка на окно для отрисовки
    // const - метод не изменяет состояние объекта (только читает)
    void draw(sf::RenderWindow& window) const;
    
    // sf::FloatRect getBounds() const; - метод получения границ птицы для коллизий
    // getBounds - возвращает прямоугольник, описывающий границы спрайта птицы
    // sf::FloatRect - прямоугольник с координатами float (используется для коллизий)
    // const - метод не изменяет объект
    // Используется в GameState для проверки столкновений с трубами и границами экрана
    sf::FloatRect getBounds() const;

    // bool hasStarted() const { return started; } - метод проверки начала движения
    // hasStarted - возвращает true если птица начала двигаться (после первого прыжка)
    // bool - логический тип (true или false)
    // const - метод не изменяет объект
    // { return started; } - inline реализация (код метода прямо в заголовке)
    // Используется для проверки, нужно ли проверять коллизии с границами экрана
    bool hasStarted() const { return started; }

// private: - приватная секция (доступна только внутри класса)
private:
    // void loadTextures(...) - приватный метод загрузки текстур
    // loadTextures - загружает 4 текстуры для анимации птицы
    // Приватный метод - используется только внутри класса Bird
    void loadTextures(const std::string& path1, const std::string& path2, 
                     const std::string& path3, const std::string& path4);
    
    // std::unique_ptr<sf::Sprite> sprite; - умный указатель на спрайт птицы
    // std::unique_ptr - умный указатель (автоматически удаляет объект при уничтожении)
    // sf::Sprite - класс SFML для отрисовки текстуры на экране
    // sprite - спрайт, который отображает текстуру птицы
    // unique_ptr используется потому что Sprite требует Texture при создании
    std::unique_ptr<sf::Sprite> sprite;
    
    // sf::Texture texture1; - текстура для дефолтного состояния птицы
    // sf::Texture - класс SFML для загрузки изображения из файла
    // texture1 - первая текстура (fbird1.png) - дефолтное состояние
    // Текстура должна существовать пока используется спрайт
    sf::Texture texture1; // fbird1 - дефолт
    
    // sf::Texture texture2; - текстура для анимации прыжка
    // texture2 - вторая текстура (fbird2.png) - при нажатии пробела
    sf::Texture texture2; // fbird2 - при нажатии пробела
    
    // sf::Texture texture3; - текстура для анимации подъема
    // texture3 - третья текстура (fbird3.png) - когда птица поднимается
    sf::Texture texture3; // fbird3 - поднимается
    
    // sf::Texture texture4; - текстура для анимации падения
    // texture4 - четвертая текстура (fbird4.png) - когда птица опускается
    sf::Texture texture4; // fbird4 - опускается
    
    // sf::Vector2f velocity; - вектор скорости птицы
    // sf::Vector2f - вектор из 2 float (x, y)
    // velocity - скорость птицы (x обычно 0, y изменяется из-за гравитации и прыжков)
    // velocity.y > 0 означает падение вниз, velocity.y < 0 означает подъем вверх
    sf::Vector2f velocity;
    
    // bool started = false; - флаг начала движения птицы
    // bool - логический тип (true/false)
    // started - true если птица начала двигаться (после первого прыжка)
    // = false - инициализация значением false (птица не двигается в начале)
    // Используется для проверки коллизий (не проверяем пока птица не начала двигаться)
    bool started = false;
    
    // bool isJumping = false; - флаг активного прыжка
    // isJumping - true если птица сейчас в процессе прыжка
    // Используется для анимации прыжка (переключение текстур)
    bool isJumping = false;
    
    // float jumpTimer = 0.0f; - таймер анимации прыжка
    // jumpTimer - накапливает время с начала прыжка
    // Используется для определения, какую текстуру показывать во время прыжка
    float jumpTimer = 0.0f;
    
    // static constexpr float JUMP_ANIMATION_TIME = 0.15f; - константа времени анимации прыжка
    // static - принадлежит классу, а не объекту (одна копия для всех объектов Bird)
    // constexpr - константа времени компиляции
    // float - число с плавающей точкой
    // JUMP_ANIMATION_TIME - длительность анимации прыжка в секундах (0.15 сек = 150 мс)
    // 0.15f - суффикс f указывает что это float
    static constexpr float JUMP_ANIMATION_TIME = 0.15f; // время анимации прыжка
};