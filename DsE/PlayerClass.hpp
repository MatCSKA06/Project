#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Player {
public:

    explicit Player(const sf::Vector2u& windowSize);
    ~Player();
    void Draw_Player(sf::RenderWindow& window);
    void Keyboard_Handle_Input(); 
    void Update_Player_Position(float deltaTime);

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
    float Player_Speed = 800.0f;

    sf::Texture Player_Texture_Up;
    sf::Texture Player_Texture_Down;
    sf::Texture Player_Texture_Left;
    sf::Texture Player_Texture_Right;
    sf::Texture Player_Texture_UpLeft;
    sf::Texture Player_Texture_DownLeft;
    sf::Texture Player_Texture_DownRight;
    sf::Texture Player_Texture_UpRight;

};