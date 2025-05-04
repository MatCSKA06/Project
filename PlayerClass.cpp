#include "PlayerClass.hpp"
#include "UIClass.hpp"
#include <cmath>
#include <iostream>

// Загрузка текстур для статических положений
Player::Player(const sf::Vector2u& windowSize) : Player_Velocity(0, 0) {
    if (!Player_Texture_Up.loadFromFile("Assets/Player_Texture_Up.png"))
        std::cerr << "Error loading texture";
    if (!Player_Texture_Down.loadFromFile("Assets/Player_Texture_Down.png"))
        std::cerr << "Error loading texture";
    if (!Player_Texture_Left.loadFromFile("Assets/Player_Texture_Left.png"))
        std::cerr << "Error loading texture";
    if (!Player_Texture_Right.loadFromFile("Assets/Player_Texture_Right.png"))
        std::cerr << "Error loading texture";
    if (!Player_Texture_UpLeft.loadFromFile("Assets/Player_Texture_UpLeft.png"))
        std::cerr << "Error loading texture";
    if (!Player_Texture_UpRight.loadFromFile("Assets/Player_Texture_UpRight.png"))
        std::cerr << "Error loading texture";
    if (!Player_Texture_DownLeft.loadFromFile("Assets/Player_Texture_DownLeft.png"))
        std::cerr << "Error loading texture";
    if (!Player_Texture_DownRight.loadFromFile("Assets/Player_Texture_DownRight.png"))
        std::cerr << "Error loading texture";

    // Загрузка текстур для анимации
    if (!Player_Texture_GoingDown1.loadFromFile("Assets/Player_Texture_GoingDown1.png"))
        std::cerr << "Error loading texture";
    if (!Player_Texture_GoingDown2.loadFromFile("Assets/Player_Texture_GoingDown2.png"))
        std::cerr << "Error loading texture";
    if (!Player_Texture_GoingUp1.loadFromFile("Assets/Player_Texture_GoingUp1.png"))
        std::cerr << "Error loading texture";
    if (!Player_Texture_GoingUp2.loadFromFile("Assets/Player_Texture_GoingUp2.png"))
        std::cerr << "Error loading texture";

    if (!Rock_Texture.loadFromFile("Assets/Rock.png")) {
        std::cerr << "Error loading Rock texture\n";
    }

    sf::Sprite rock;
    rock.setTexture(Rock_Texture);
    rock.setPosition(500.f, 300.f); // координаты, куда поставить препятствие
        
    obstacles.push_back(rock);

    // Устанавливаем положение, в котором спавнится игрок
    Player_Sprite.setTexture(Player_Texture_Down);

    // Всё это устанавливает позицию спавна игрока (чтобы тот спавнился в центре окна)
    sf::Vector2u Texture_Size_Player = Player_Texture_Down.getSize();
    float Window_Center_X = windowSize.x / 2.0f;
    float Window_Center_Y = windowSize.y / 2.0f;
    float Position_Of_Player_Spawn_X = Window_Center_X - ((Texture_Size_Player.x) / 2.0f);
    float Position_Of_Player_Spawn_Y = Window_Center_Y - ((Texture_Size_Player.y) / 2.0f);
    Player_Sprite.setPosition(Position_Of_Player_Spawn_X, Position_Of_Player_Spawn_Y);
}

Player::~Player() = default;

    // Пока пустой деструктор

sf::FloatRect Player::Get_Player_Feet_Bounds() const {
    sf::FloatRect fullBounds = Player_Sprite.getGlobalBounds();
    return sf::FloatRect(
        fullBounds.left,
        fullBounds.top + fullBounds.height * 0.8f,
        fullBounds.width,
        fullBounds.height * 0.2f
    );
}

void Player::Keyboard_Handle_Input() { // Движение
    Player_Velocity = sf::Vector2f(0.f, 0.f);  // сбрасываем скорость
    bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
    bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

    if (up && left) {
        Player_Velocity.x = -1;
        Player_Velocity.y = -1;
    }
    else if (up && right) {
        Player_Velocity.x = 1;
        Player_Velocity.y = -1;
    }
    else if (down && left) {
        Player_Velocity.x = -1;
        Player_Velocity.y = 1;
    }
    else if (down && right) {
        Player_Velocity.x = 1;
        Player_Velocity.y = 1;
    }
    else if (up) {
        Player_Velocity.y = -1;
    }
    else if (down) {
        Player_Velocity.y = 1;
    }
    else if (left) {
        Player_Velocity.x = -1;
    }
    else if (right) {
        Player_Velocity.x = 1;
    }

    if (Player_Velocity.x != 0 && Player_Velocity.y != 0) {
        Player_Velocity /= std::sqrt(2.0f);  // чтобы не было ускорения по диагонали
    }
    
}


void Player::Draw_Player(sf::RenderWindow& window) {
    for (const auto& obstacle : obstacles) {
        window.draw(obstacle);
    }
    window.draw(Player_Sprite); // Отрисовка спрайта
}

void Player::Update_Player_Position(float deltaTime) {
    // Сдвигаем
    Player_Sprite.move(Player_Velocity * Player_Speed * deltaTime);
    
    // Если движется строго вниз — анимируем
    if ((Player_Velocity.y > 0.f) && (Player_Velocity.x == 0.f)) {
        lastDirection = PlayerDirection::Down;
        walkDownTimer += deltaTime;
        if (walkDownTimer >= walkDownSwitchTime) {
            walkDownTimer -= walkDownSwitchTime;
            walkDownToggle = !walkDownToggle;
        }
        Player_Sprite.setTexture(
            walkDownToggle 
              ? Player_Texture_GoingDown1 
              : Player_Texture_GoingDown2
        );
    }
    // Если движется строго вверх
    else if ((Player_Velocity.y < 0.f) && (Player_Velocity.x == 0.f)) {
    // идём вверх — анимируем между двумя кадрами
        lastDirection = PlayerDirection::Up;
        walkUpTimer += deltaTime;
        if (walkUpTimer >= walkDownSwitchTime) {   // используем уже существующий switch time
            walkUpTimer -= walkDownSwitchTime;
            walkUpToggle = !walkUpToggle;
        }
        Player_Sprite.setTexture(
            walkUpToggle
            ? Player_Texture_GoingUp1
            : Player_Texture_GoingUp2
        );
        walkDownTimer  = 0.f;
        walkDownToggle = false;
    }
    // Если движется влево
    else if ((Player_Velocity.x < 0.f) && (Player_Velocity.y == 0.f)) {
        lastDirection = PlayerDirection::Left;
        Player_Sprite.setTexture(Player_Texture_Left);
        walkDownTimer = 0.f;
        walkDownToggle = false;
    }
    // Если движется вправо
    else if ((Player_Velocity.x > 0.f) && (Player_Velocity.y == 0.f)){
        lastDirection = PlayerDirection::Right;
        Player_Sprite.setTexture(Player_Texture_Right);
        walkDownTimer = 0.f;
        walkDownToggle = false;
    }
    // Если движется вправо и вверх
    else if ((Player_Velocity.x > 0.f) && (Player_Velocity.y > 0.f)) {
        lastDirection = PlayerDirection::DownRight;
        Player_Sprite.setTexture(Player_Texture_DownRight);
        walkDownTimer = 0.f;
        walkDownToggle = false;
    }
    // Если движется вправо и вниз
    else if ((Player_Velocity.x > 0.f) && (Player_Velocity.y < 0.f)) {
        lastDirection = PlayerDirection::UpRight;
        Player_Sprite.setTexture(Player_Texture_UpRight);
        walkDownTimer = 0.f;
        walkDownToggle = false;
    }
    // Если движется влево и вверх
    else if ((Player_Velocity.x < 0.f) && (Player_Velocity.y > 0.f)) {
        lastDirection = PlayerDirection::DownLeft;
        Player_Sprite.setTexture(Player_Texture_DownLeft);
        walkDownTimer = 0.f;
        walkDownToggle = false;
    }
    // Если движется влево и вниз
    else if ((Player_Velocity.x < 0.f) && (Player_Velocity.y < 0.f)) {
        lastDirection = PlayerDirection::UpLeft;
        Player_Sprite.setTexture(Player_Texture_UpLeft);
        walkDownTimer = 0.f;
        walkDownToggle = false;
    }

    // Если стоит — показываем последний кадр игравшей анимации
    else {  // Player_Velocity == (0,0)
        walkDownTimer = 0.f;
        walkDownToggle = false;
        walkDownTimer = walkUpTimer = 0.f;
        walkDownToggle = walkUpToggle = false;
        switch (lastDirection) {
            case PlayerDirection::Down:
                Player_Sprite.setTexture(Player_Texture_Down);
                break;
            case PlayerDirection::Up:
                Player_Sprite.setTexture(Player_Texture_Up);
                break;
            case PlayerDirection::Left:
                Player_Sprite.setTexture(Player_Texture_Left);
                break;
            case PlayerDirection::Right:
                Player_Sprite.setTexture(Player_Texture_Right);
                break;
            case PlayerDirection::UpRight:
                Player_Sprite.setTexture(Player_Texture_UpRight);
                break;
            case PlayerDirection::UpLeft:
                Player_Sprite.setTexture(Player_Texture_UpLeft);
                break;
            case PlayerDirection::DownRight:
                Player_Sprite.setTexture(Player_Texture_DownRight);
                break;
            case PlayerDirection::DownLeft:
                Player_Sprite.setTexture(Player_Texture_DownLeft);
                break;
        }
    }
}
