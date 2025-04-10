#include <SFML/Graphics.hpp>
#include <vector> 
#include <string>

using namespace sf;

class Inventory 
{
public: 
    Inventory(float x, float y, int rows, int columns, float size) : position(x, y), rows(rows), columns(columns), size(size) 
    { 
        if (!phoneTexture.loadFromFile("Без имени.png")) { // Загрузка текстуры телефона
        }
        if (!inventoryBackroundTexture.loadFromFile("inventory.jpg")){
		}
		inventoryBackroundSprite.setTexture(inventoryBackroundTexture);
		inventoryBackroundSprite.setPosition(x,y); //масштабируем
		inventoryBackroundSprite.setScale((size*columns)/inventoryBackroundTexture.getSize().x, (size*rows)/inventoryBackroundTexture.getSize().y);
        for (int i = 0; i < rows; i++) { 
            std::vector<RectangleShape> row;
            for (int j = 0; j < columns; j++) { 
                RectangleShape cell(Vector2f(size, size));
                cell.setPosition(x + j * size, y + i * size);
                cell.setFillColor(Color::Transparent);
                cell.setOutlineThickness(2.f);
                cell.setOutlineColor(Color(100, 50, 10));
                row.push_back(cell);
            }
            grid.push_back(row);
        }
        
        items.resize(rows, std::vector<Text>(columns));
        sprites.resize(rows, std::vector<Sprite>(columns));
        hasItem.resize(rows, std::vector<bool>(columns, false));
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                items[i][j].setFont(font);
                items[i][j].setCharacterSize(60);
                items[i][j].setFillColor(Color::White);
                items[i][j].setPosition(x + j * size + 10, y + i * size + 10);
                
                sprites[i][j].setTexture(phoneTexture);
                sprites[i][j].setPosition(x + j * size + 5, y + i * size + 5);
                float scale = (size - 10) / phoneTexture.getSize().x;
                sprites[i][j].setScale(scale, scale);
            }
        }
        
        // Установка телефона в первую ячейку
        items[0][0].setString(""); 
        hasItem[0][0] = true; // Отмечаем ячейку как занятую
        cursor.setSize(Vector2f(size, size));
        cursor.setFillColor(Color(0, 0, 0, 0));
        cursor.setOutlineThickness(3.f);
        cursor.setOutlineColor(Color::Yellow);
        cursor.setPosition(x, y); 
    }
    
    void draw(RenderWindow& window) {
        if (!isVisible) return;
        
        for (auto& row : grid) {
            for (auto& cell : row) {
                window.draw(cell);
            }
        }
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (items[i][j].getString() != "") {
                    window.draw(items[i][j]);
                }
                if (hasItem[i][j]) { // Рисуем спрайт, если ячейка занята
                    window.draw(sprites[i][j]);
                }
            }
        }
        if (isHolding) {
            window.draw(heldSprite); // Рисуем удерживаемый предмет
        }
        window.draw(cursor);
    }

    bool addItem(const std::string& item) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (!hasItem[i][j]) {
                    items[i][j].setString(item);
                    hasItem[i][j] = true;
                    return true;
                }
            }
        }
        return false;
    }

    void setVisibility(bool visibility) {
        isVisible = visibility;
    }
    
    bool isVisible = false; 

    void handleInput(RenderWindow& window) {
        if (!isVisible) return;

        // Управление курсором с клавиатуры
        if (Keyboard::isKeyPressed(Keyboard::D) && cursorX < columns - 1) {
            cursorX++; 
            updateCursorPosition();
            while (Keyboard::isKeyPressed(Keyboard::D)) {}
        }
        if (Keyboard::isKeyPressed(Keyboard::A) && cursorX > 0) {
            cursorX--;
            updateCursorPosition();
            while (Keyboard::isKeyPressed(Keyboard::A)) {}
        }
        if (Keyboard::isKeyPressed(Keyboard::W) && cursorY > 0) {
            cursorY--;
            updateCursorPosition();
            while (Keyboard::isKeyPressed(Keyboard::W)) {}
        }
        if (Keyboard::isKeyPressed(Keyboard::S) && cursorY < rows - 1) {
            cursorY++;  
            updateCursorPosition();
            while (Keyboard::isKeyPressed(Keyboard::S)) {}
        }
        
        // Перетаскивание 
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

        if (Mouse::isButtonPressed(Mouse::Left) && !isHolding) {
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    if (grid[i][j].getGlobalBounds().contains(mousePos) && hasItem[i][j]) {
                        isHolding = true;
                        cursorX = j;
                        cursorY = i;
                        heldItem = items[i][j].getString();
                        heldSprite = sprites[i][j];
                        items[i][j].setString("");
                        hasItem[i][j] = false;
                        mouseOffset = mousePos - sprites[i][j].getPosition();
                        break;
                    }
                }
                if (isHolding) break;
            }
        }

        if (isHolding) {
            heldSprite.setPosition(mousePos - mouseOffset);
        }

        if (!Mouse::isButtonPressed(Mouse::Left) && isHolding) {
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    if (grid[i][j].getGlobalBounds().contains(mousePos) && !hasItem[i][j]) {
                        items[i][j].setString(heldItem);
                        hasItem[i][j] = true;
                        sprites[i][j] = heldSprite;
                        sprites[i][j].setPosition(grid[i][j].getPosition() + Vector2f(5, 5));
                        isHolding = false;
                        break;
                    }
                }
                if (!isHolding) break;
            }
            if (isHolding) { // Возврат на исходную позицию, если не удалось разместить
                items[cursorY][cursorX].setString(heldItem);
                hasItem[cursorY][cursorX] = true;
                sprites[cursorY][cursorX] = heldSprite;
                sprites[cursorY][cursorX].setPosition(grid[cursorY][cursorX].getPosition() + Vector2f(5, 5));
                isHolding = false;
            }
        }
    }

private: 
    std::vector<std::vector<RectangleShape>> grid;
    std::vector<std::vector<Text>> items;
    std::vector<std::vector<Sprite>> sprites;
    std::vector<std::vector<bool>> hasItem;
    Font font;
    Texture phoneTexture; //телефон
	Texture inventoryBackroundTexture; // инвентарь
	Sprite inventoryBackroundSprite;
    Vector2f position;
    int rows, columns;
    float size;
    RectangleShape cursor; 
    int cursorX = 0;
    int cursorY = 0;
    bool isHolding = false; 
    std::string heldItem; 
    Sprite heldSprite;
    Vector2f mouseOffset; // Для перетаскивания

    void updateCursorPosition() {
        cursor.setPosition(position.x + cursorX * size, position.y + cursorY * size);
    }
};

int main() {
    RenderWindow window(VideoMode(1000, 1000), "Character Movement");
    window.setFramerateLimit(60);

    Texture texture;
    if (!texture.loadFromFile("5408931622327281741.jpg")) {
        return -1;
    }

    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Снимок экрана_2025-04-01_18-56-42.png")) {
        return -1;
    }

    Sprite backgroundSprite(backgroundTexture);
    Sprite character;
    character.setTexture(texture);
    character.setPosition(500, 500);

    const int totalFrames = 4;
    const int frameWidth = 595 / 4;
    const int frameHeight = 500;

    enum SpriteType { S = 0, A = 1, D = 2, W = 3 };
    SpriteType currentSprite = S;

    int currentFrame = 0;
    const float moveSpeed = 5.f;

    character.setTextureRect(IntRect(frameWidth * currentSprite, 0, frameWidth, frameHeight));

    Inventory inventory(200, 200, 4, 4, 150); 
    const float minX = 100.f, maxX = 800.f;
    const float minY = 100.f, maxY = 800.f;

    Clock clock;
    Clock animationClock;
    bool flag = false;
    //RectangleShape inv_back(Vector2f(300, 300));
    //inv_back.setFillColor(Color(90, 90, 90));
    //inv_back.setPosition(250, 200);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        Vector2f movement(0.f, 0.f);
        bool spriteChanged = false;

        if (Keyboard::isKeyPressed(Keyboard::A)) {
            movement.x = -moveSpeed;
            if (currentSprite != A) {
                currentSprite = A;
                spriteChanged = true;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            movement.x = moveSpeed;
            if (currentSprite != D) {
                currentSprite = D;
                spriteChanged = true;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            movement.y = -moveSpeed;
            if (currentSprite != W) {
                currentSprite = W;
                spriteChanged = true;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::S)) {
            movement.y = moveSpeed;
            if (currentSprite != S) {
                currentSprite = S;
                spriteChanged = true;
            }
        }

        if (character.getPosition().x + movement.x >= minX && 
            character.getPosition().x + movement.x + character.getGlobalBounds().width <= maxX) {
            character.move(movement.x, 0);
        }

        if (character.getPosition().y + movement.y >= minY && 
            character.getPosition().y + movement.y + character.getGlobalBounds().height <= maxY) {
            character.move(0, movement.y);
        }

        if (Keyboard::isKeyPressed(Keyboard::I)) {
            flag = !flag;
            inventory.setVisibility(flag);
            while (Keyboard::isKeyPressed(Keyboard::I)) {}
        }

        if (spriteChanged) {
            currentFrame = 0;
            animationClock.restart();
        }

        character.setTextureRect(IntRect(
            frameWidth * currentSprite,
            0,
            frameWidth,
            frameHeight
        ));

        window.clear();
        window.draw(backgroundSprite);
        window.draw(character);
        if (flag) {
            //window.draw(inv_back);
            inventory.handleInput(window); // Передаем окно для обработки мыши
        }
        inventory.draw(window);
        window.display();
    }


    return 0;
}
