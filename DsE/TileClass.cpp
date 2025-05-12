#include "TileClass.hpp"

Tile::Tile(TileType type, int PosMapX, int PosMapY, float TileSize)
    : Current_Type(type),
      Current_PosMapX(PosMapX),
      Current_PosMapY(PosMapY),
      Current_Tile_Size(TileSize)
{
    // Устанавливаем позицию спрайта на основе координат в сетке
    sf::Vector2f Current_Tile_Position(
        Current_PosMapX * Current_Tile_Size,
        Current_PosMapY * Current_Tile_Size
    );
    Current_Tile_Sprite.setPosition(Current_Tile_Position);
}

Tile::~Tile() = default;

void Tile::Set_Tile_Texure(const sf::Texture& texture) {
    Current_Tile_Sprite.setTexture(texture);

    sf::Vector2u Texture_Size = texture.getSize();
    sf::Vector2f Current_Tile_Scale(
        Current_Tile_Size / static_cast<float>(Texture_Size.x),
        Current_Tile_Size / static_cast<float>(Texture_Size.y)
    );
    Current_Tile_Sprite.setScale(Current_Tile_Scale);
}

void Tile::Draw_Tile(sf::RenderWindow& window) const {
    if (Current_Type == TileType::Empty) return;
    window.draw(Current_Tile_Sprite);
}
