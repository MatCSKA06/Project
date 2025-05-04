#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Enemy {
public:
    Enemy(const sf::Vector2u& windowSize); // Конструктор
    ~Enemy(); // Деструктор

    // sf::Vector2u getTextureSize() const;
    // sf::Vector2f getSpriteSize() const;

    float Meaning_of_Velocity_gradient(float MainEnemy_Player_radius); //Выдаёт скорость врага
    void MainEnemy_Movement(float MainEnemy_velocity); // отвечает за движение врага
    void Updating_Direction_Of_Sprite(const sf::Vector2f& direction); // Отвечает за отрисовывание нужного спрайта при движении в определённую сторону 

    void Draw_Enemy(sf::RenderWindow& window);

// Новые методы (добавленные для динамической скорости)
    void UpdatePosition(const sf::Vector2f& playerPos, float deltaTime, float speedCoeff = 0.05f);
    void UpdateSpriteDirection(const sf::Vector2f& playerPos);
   


private:
    sf::Vector2f position;
    


    std::vector<float> Const_Meaning_of_MainEnemy_Player_radius;
    std::vector<float> Const_Meaning_of_MainEnemy_Velocity;

    sf::Texture Enemy_Texture;
    sf::Sprite Enemy_Sprite;

    sf::Texture MainEnemy_Texture_Up;
    sf::Texture MainEnemy_Texture_Down;
    sf::Texture MainEnemy_Texture_Left;
    sf::Texture MainEnemy_Texture_Right;
};
