#include "MapClass.hpp"
#include <iostream>
#include <random>
#include <algorithm>

Map::Map(int WidthInTiles, int HeightInTiles, float TileSize)
    : Map_Width_Tiles(WidthInTiles),
      Map_Height_Tiles(HeightInTiles),
      Map_Tile_Size(TileSize)
{
    Tile_Map.resize(Map_Height_Tiles);
    for (int y = 0; y < Map_Height_Tiles; ++y) {
        Tile_Map[y].reserve(Map_Width_Tiles);
        for (int x = 0; x < Map_Width_Tiles; ++x) {
            Tile_Map[y].emplace_back(TileType::Empty, x, y, Map_Tile_Size);
        }
    }
}

Map::~Map(){};

bool Map::Load_Tile_Texture(TileType Type, const std::string& FileName) {
    sf::Texture Texture;
    if (!Texture.loadFromFile(FileName)) {
        std::cerr << "Loading texture failed: " << FileName << std::endl;
        return false;
    }
    Tile_Textures[Type] = std::move(Texture);
    return true;
}

// Рисуем всегда рабочую простейшую карту для примера
void Map::Generate_Simple_Map() {
    for (int y = 0; y < Map_Height_Tiles; y++) {
        for (int x = 0; x < Map_Width_Tiles; x++) {
            TileType Type;

            // Стены по краям карты
            if ((y == 0) or (y == Map_Height_Tiles - 1) or (x == 0) or (x == Map_Width_Tiles - 1)) {
                Type = TileType::Wall;
            } else {
                Type = TileType::floor;
            }

            Tile_Map[y][x] = Tile(Type, x, y, Map_Tile_Size);

            // Если для такого типа есть текстура — установить её
            auto it = Tile_Textures.find(Type);
            if (it != Tile_Textures.end()) {
                Tile_Map[y][x].Set_Tile_Texure(it->second);
            }
        }
    }
}

// Функция, ставящая тайл в нужное место, согласно логической сетке
void Map::Set_Tile(int X, int Y, TileType Type) {
    if ((X < 0) or (X >= Map_Width_Tiles) or (Y < 0) or (Y >= Map_Height_Tiles))
        return;
    Tile_Map[Y][X] = Tile(Type, X, Y, Map_Tile_Size);
    auto it = Tile_Textures.find(Type);
    if (it != Tile_Textures.end())
        Tile_Map[Y][X].Set_Tile_Texure(it->second);
}

void Map::Generate_Labyrinth() {
    // 1) Заполняем всю карту стенами
    for (int y = 0; y < Map_Height_Tiles; ++y)
        for (int x = 0; x < Map_Width_Tiles; ++x)
            Set_Tile(x, y, TileType::Wall);

    // 2) Инициализируем генератор случайных чисел
    std::random_device rd;
    std::mt19937 rng(rd());

    // 3) Выбираем стартовую ячейку (1,1) — нечётные индексы,
    //    чтобы «ходы» и «стены» чередовались ровно
    Carve_Maze(1, 1, rng);
}

void Map::Carve_Maze(int X, int Y, std::mt19937& RNG) {
    // Пометим текущую как пол
    Set_Tile(X, Y, TileType::floor);

    // Четыре направления (dx, dy) с шагом 2
    std::vector<std::pair<int,int>> directions = {
        { 0, -2}, { 0,  2},
        {-2,  0}, { 2,  0}
    };
    std::shuffle(directions.begin(), directions.end(), RNG);

    for (auto [dx, dy] : directions) {
        int nx = X + dx;
        int ny = Y + dy;
        // Проверяем границы и что цель — стена (ещё не вырезаны)
        if (nx > 0 && nx < Map_Width_Tiles - 1
         && ny > 0 && ny < Map_Height_Tiles - 1
         && Get_Tile_Type(nx, ny) == TileType::Wall) 
        {
            // «Пробиваем» стену между (X,Y) и (nx,ny)
            int wx = X + dx/2;
            int wy = Y + dy/2;
            Set_Tile(wx, wy, TileType::floor);
            // Рекурсивно идём дальше
            Carve_Maze(nx, ny, RNG);
        }
    }
}

void Map::Draw_Map(sf::RenderWindow& window) const {
    for (int y = 0; y < Map_Height_Tiles; y++) {
        for (int x = 0; x < Map_Width_Tiles; ++x) {
            Tile_Map[y][x].Draw_Tile(window);
        }
    }
}

TileType Map::Get_Tile_Type(int X, int Y) const {
    if ((X < 0) or (X >= Map_Width_Tiles) or (Y < 0) or (Y >= Map_Height_Tiles))
        return TileType::Empty;
    return Tile_Map[Y][X].getType();
}