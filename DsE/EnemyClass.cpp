#include "EnemyClass.hpp"
#include <iostream>
#include <cmath>  // для округления

// Конструктор
Enemy::Enemy(const sf::Vector2u& windowSize) { // Выставляем начальные условия
    Const_Meaning_of_MainEnemy_Player_radius.reserve(20);
    Const_Meaning_of_MainEnemy_Velocity.reserve(20);

    for (float i = 0.0f; i <= 2.0f; i += 0.1f) {
        Const_Meaning_of_MainEnemy_Player_radius.push_back(roundf(i * 10) / 10);
    }

    for (float i = 0.0f; i <= 20.0f; i += 1.0f) {
        Const_Meaning_of_MainEnemy_Velocity.push_back(i);
    }

    // Вгружаем текстуры
    if (!MainEnemy_Texture_Up.loadFromFile("Assets/MainEnemy_Texture_Up.png"))
        std::cerr << "Error loading texture";
    if (!MainEnemy_Texture_Down.loadFromFile("Assets/MainEnemy_Texture_Down.png"))
        std::cerr << "Error loading texture";
    if (!MainEnemy_Texture_Left.loadFromFile("Assets/MainEnemy_Texture_Left.png"))
        std::cerr << "Error loading texture";
    if (!MainEnemy_Texture_Right.loadFromFile("Assets/MainEnemy_Texture_Right.png"))
        std::cerr << "Error loading texture";

    Enemy_Sprite.setTexture(MainEnemy_Texture_Down);

    sf::Vector2u Texture_Size_MainEnemy = MainEnemy_Texture_Down.getSize();

    float Window_Center_X = windowSize.x / 5.0f;
    float Window_Center_Y = windowSize.y / 4.0f;

    float Position_Of_MainEnemy_Spawn_X = Window_Center_X - ((Texture_Size_MainEnemy.x) / 2.0f);
    float Position_Of_MainEnemy_Spawn_Y = Window_Center_Y - ((Texture_Size_MainEnemy.y) / 2.0f);
    Enemy_Sprite.setPosition(Position_Of_MainEnemy_Spawn_X, Position_Of_MainEnemy_Spawn_Y);
}

Enemy::~Enemy() {
    // Деструктор (пока пустой)
}



// Вытаскиваем значение скорости, которое присвоим MainEnemy
float Enemy::Meaning_of_Velocity_gradient(float MainEnemy_Player_radius) {
    float rounded_radius = std::round(MainEnemy_Player_radius * 10.0f) / 10.0f;

    for (size_t i = 0; i < Const_Meaning_of_MainEnemy_Player_radius.size(); ++i) {
        if (Const_Meaning_of_MainEnemy_Player_radius[i] == rounded_radius) { 
            return Const_Meaning_of_MainEnemy_Velocity[i];
        }
    }

    // Если не нашли совпадение:
    throw std::runtime_error("Враг слишком далеко");
}


void Enemy::Draw_Enemy(sf::RenderWindow& window) {
    window.draw(Enemy_Sprite); // Отрисовка спрайта
}



