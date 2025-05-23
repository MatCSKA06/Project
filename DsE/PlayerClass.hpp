#pragma once

#include <SFML/Graphics.hpp>
#include "MapClass.hpp"
#include <string>
#include <vector>

class Player {
public:

    sf::FloatRect Get_Player_Feet_Bounds() const; // для коллизии
    explicit Player(const sf::Vector2u& windowSize, sf::Vector2f spawnPos = sf::Vector2f(600.f, 600.f));
    ~Player();
    void Draw_Player(sf::RenderWindow& window);
    void Keyboard_Handle_Input(); 
    void Update_Player_Position(float deltaTime, const Map& GameMap);

    sf::Sprite Player_Sprite;

    sf::FloatRect getGlobalBounds() const {
        return Player_Sprite.getGlobalBounds();
    }
    
    sf::Vector2f getPosition() const { 
        return Player_Sprite.getPosition(); 
    }

    sf::Vector2f getCameraCenterOffset() const {
        sf::Vector2f Player_Position = Player_Sprite.getPosition();
        sf::FloatRect bounds = Player_Sprite.getGlobalBounds();
    
        return sf::Vector2f(
            Player_Position.x + bounds.width  * 0.5f,
            Player_Position.y + bounds.height * 0.5f
        );
    }


private:

    sf::Texture Player_Texture;

    sf::Vector2f Player_Velocity;
    float Player_Speed = 600.0f;

    std::vector<sf::Sprite> obstacles;
    sf::Texture Rock_Texture;

    // Текстуры для 8 проекций положения в данный момент
    sf::Texture Player_Texture_Up;
    sf::Texture Player_Texture_Down;
    sf::Texture Player_Texture_Left;
    sf::Texture Player_Texture_Right;
    sf::Texture Player_Texture_UpLeft;
    sf::Texture Player_Texture_DownLeft;
    sf::Texture Player_Texture_DownRight;
    sf::Texture Player_Texture_UpRight;

    // Текстуры для анимации "идёт вниз"
    sf::Texture Player_Texture_GoingDown1;
    sf::Texture Player_Texture_GoingDown2;

    // Текстуры для анимации "идёт вниз"
    float walkDownTimer = 0.f;
    bool  walkDownToggle = false;

    // Текстуры для анимации "идёт вверх"
    sf::Texture Player_Texture_GoingUp1;
    sf::Texture Player_Texture_GoingUp2;

    // анимационные переменные для движения вверх
    float walkUpTimer = 0.f;
    bool  walkUpToggle = false;

    // сохраняем последнее направление
    enum class PlayerDirection { Down, Up, Left, Right, UpRight, UpLeft, DownRight, DownLeft };
    PlayerDirection lastDirection = PlayerDirection::Down;

    // анимационные переменные
    const float walkDownSwitchTime = 0.4f;  // секунда на кадр

};