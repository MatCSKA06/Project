#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "TileClass.hpp"
#include <random>

class Map {
public:
    Map(int WidthInTiles, int HeightInTiles, float TileSize);
    ~Map();

    // Загрузка текстуры для конкретного типа плитки
    bool Load_Tile_Texture(TileType Type, const std::string& FileName);

    void Carve_Maze(int X, int Y, std::mt19937& RNG);

    // Генерация простой карты: границы — стены, всё внутри — пол
    void Generate_Simple_Map();

    // Установить процедурно вместо пола стену или наоборот
    void Set_Tile(int X, int Y, TileType Type);

    // Генерация процедурного лабиринта
    void Generate_Labyrinth();

    // Отрисовка всех плиток карты
    void Draw_Map(sf::RenderWindow& window) const;

    // Получить тип плитки в указанных координатах (для коллизий и логики)
    TileType Get_Tile_Type(int X, int Y) const;

    // Даже без реализации в .cpp, просто берём и выдираем размер тайла
    float Get_Tile_Size() const { return Map_Tile_Size; }

    // Для адекватного появления игрока
    int Get_Width() const { return Map_Width_Tiles; }
    int Get_Height() const { return Map_Height_Tiles; }

private:
    int Map_Width_Tiles;
    int Map_Height_Tiles;
    float Map_Tile_Size;

    // Двумерный вектор с тайлами
    std::vector<std::vector<Tile>> Tile_Map;

    // Словарь текстур по типу плитки
    std::map<TileType, sf::Texture> Tile_Textures;

};
