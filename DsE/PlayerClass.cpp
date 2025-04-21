#include "PlayerClass.hpp"
#include "UIClass.hpp"
#include <cmath>
#include <iostream>

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

    Player_Sprite.setTexture(Player_Texture_Down);

    sf::Vector2u Texture_Size_Player = Player_Texture_Down.getSize();
    float Window_Center_X = windowSize.x / 2.0f;
    float Window_Center_Y = windowSize.y / 2.0f;

    float Position_Of_Player_Spawn_X = Window_Center_X - ((Texture_Size_Player.x) / 2.0f);
    float Position_Of_Player_Spawn_Y = Window_Center_Y - ((Texture_Size_Player.y) / 2.0f);
    Player_Sprite.setPosition(Position_Of_Player_Spawn_X, Position_Of_Player_Spawn_Y);
}

Player::~Player() {
    // Пока пустой
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
        Player_Sprite.setTexture(Player_Texture_UpLeft);
    }
    else if (up && right) {
        Player_Velocity.x = 1;
        Player_Velocity.y = -1;
        Player_Sprite.setTexture(Player_Texture_UpRight);
    }
    else if (down && left) {
        Player_Velocity.x = -1;
        Player_Velocity.y = 1;
        Player_Sprite.setTexture(Player_Texture_DownLeft);
    }
    else if (down && right) {
        Player_Velocity.x = 1;
        Player_Velocity.y = 1;
        Player_Sprite.setTexture(Player_Texture_DownRight);
    }
    else if (up) {
        Player_Velocity.y = -1;
        Player_Sprite.setTexture(Player_Texture_Up);
    }
    else if (down) {
        Player_Velocity.y = 1;
        Player_Sprite.setTexture(Player_Texture_Down);
    }
    else if (left) {
        Player_Velocity.x = -1;
        Player_Sprite.setTexture(Player_Texture_Left);
    }
    else if (right) {
        Player_Velocity.x = 1;
        Player_Sprite.setTexture(Player_Texture_Right);
    }

    if (Player_Velocity.x != 0 && Player_Velocity.y != 0) {
        Player_Velocity /= std::sqrt(2.0f);  // чтобы не было ускорения по диагонали
    }
    
}


void Player::Draw_Player(sf::RenderWindow& window) {
    window.draw(Player_Sprite); // Отрисовка спрайта
}

void Player::Update_Player_Position(float deltaTime) {
    Player_Sprite.move(Player_Velocity * Player_Speed * deltaTime);
}