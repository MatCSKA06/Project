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

    position.x = Window_Center_X - (Texture_Size_MainEnemy.x / 2.0f);
    position.y = Window_Center_Y - (Texture_Size_MainEnemy.y / 2.0f);
    Enemy_Sprite.setPosition(position);
}
    Enemy::~Enemy() {
	MainEnemy_Texture_Up.~Texture();
    	MainEnemy_Texture_Down.~Texture();
    	MainEnemy_Texture_Left.~Texture();
    	MainEnemy_Texture_Right.~Texture();
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

void Enemy::UpdatePosition(const sf::Vector2f& playerPos, float deltaTime, float speedCoeff) {
    sf::Vector2f enemyPos = Enemy_Sprite.getPosition();
    sf::Vector2f direction = playerPos - enemyPos;
    float distance = std::hypot(direction.x, direction.y);
    
    if (distance > 0.0f) {
        direction /= distance;
    }
    
    float speed = distance * speedCoeff;
    Enemy_Sprite.move(direction * speed * deltaTime);
}

void Enemy::UpdateSpriteDirection(const sf::Vector2f& playerPos) {
    sf::Vector2f enemyPos = Enemy_Sprite.getPosition();
    sf::Vector2f direction = playerPos - enemyPos;
    
    if (std::abs(direction.x) > std::abs(direction.y)) {
        if (direction.x > 0) {
            Enemy_Sprite.setTexture(MainEnemy_Texture_Right);
        } else {
            Enemy_Sprite.setTexture(MainEnemy_Texture_Left);
        }
    } else {
        if (direction.y > 0) {
            Enemy_Sprite.setTexture(MainEnemy_Texture_Down);
        } else {
            Enemy_Sprite.setTexture(MainEnemy_Texture_Up);
        }
    }
}
void Enemy::Draw_Enemy(sf::RenderWindow& window) {
    window.draw(Enemy_Sprite); // Отрисовка спрайта
}

