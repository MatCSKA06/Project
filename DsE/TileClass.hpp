#pragma once
#include <SFML/Graphics.hpp>

enum class TileType {
    floor,
    Wall,
    Empty
};

class Tile {
public:
    // Конструктор
    Tile(TileType type, int PosMapX, int PosMapY, float TileSize);
    // Деструктор
    ~Tile();

    void Set_Tile_Texure(const sf::Texture& texture);
    void Draw_Tile(sf::RenderWindow& window) const;
    TileType getType() const { return Current_Type; }

    int getX() const { return Current_PosMapX; }
    int getY() const { return Current_PosMapY; }

private:
    TileType Current_Type;
    int Current_PosMapX, Current_PosMapY;
    float Current_Tile_Size;
    sf::Sprite Tile_Sprite;
    sf::Sprite Current_Tile_Sprite;
};