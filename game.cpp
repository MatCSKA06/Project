#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <iostream>

using namespace sf;

class Inventory {
private:
 //словарь хранящий предметы 
    std::map<std::string, Texture> itemTextures;
 //это структура которая описывает предмет выброшенный из инвентаря
    struct DroppedItem {
        std::string name;
        std::string type;
        std::string description;
        Sprite sprite;
        Vector2f position;
        Text label;
    };

    std::vector<std::vector<RectangleShape>> grid; // сетка инвентаря
    std::vector<std::vector<Text>> items;//названия предметов в ячейках
    std::vector<std::vector<Sprite>> sprites;//представление предметов ну короче отрисовка
    std::vector<std::vector<bool>> hasItem; //флаг есть ли предмет в ячейке
    std::vector<DroppedItem> droppedItems; //cписок выброшенных предметов для работы с всплывающей табличкой с информацией о предмете 

    Font font;
    //Texture phoneTexture; уже лишнее 
    Texture inventoryBackroundTexture;
    Sprite inventoryBackroundSprite; // спрайт фона
    Vector2f position; // позиция верхнего левого угла инвентаря для ттго где он распологается
    int rows, columns;
    float size; // размер
    RectangleShape cursor; //рамка выбора ячейки
    int cursorX = 0; //  позиция курсора в интвентаре
    int cursorY = 0;
    bool isHolding = false; // держит ли игрок предмет
    std::string heldItem; //название удерживаемого предмета
    Sprite heldSprite; //   отрисовка удерживаемого предмета
    Vector2f mouseOffset; // смещение между мышью и углом спрайта

    void updateCursorPosition() { //обновляет визуальную позицию курсора в зависимости от cursorX и тп
        cursor.setPosition(position.x + cursorX * size, position.y + cursorY * size);
    }
//добавляет предмет в конкретную ячейку матрицы интвернатаря с заданной текстурой.
    void addItemAtPosition(int row, int col, const std::string& name, const Texture& texture) { 
        items[row][col].setString(name);
        hasItem[row][col] = true;
        sprites[row][col].setTexture(texture);
        sprites[row][col].setPosition(position.x + col * size + 5, position.y + row * size + 5);
        float scale = 0.1f;
        sprites[row][col].setScale(scale, scale);
    }

public:
    bool isVisible = false;
        const Font& getFont() const{ //шрифт
            return font;
        }
//здесь происходит создание инвентаря задача его координат по двум осям и отрисовка( я решил что начальному предмету в интвенатре быть!)
    Inventory(float x, float y, int rows, int columns, float size) :
        position(x, y), rows(rows), columns(columns), size(size)
    {
        font.loadFromFile("arialmt.ttf");
        phoneTexture.loadFromFile("phone.png");
        inventoryBackroundTexture.loadFromFile("inventory.jpg");
        inventoryBackroundSprite.setTexture(inventoryBackroundTexture);
        inventoryBackroundSprite.setPosition(x, y);
        inventoryBackroundSprite.setScale(
            (size * columns) / inventoryBackroundTexture.getSize().x,
            (size * rows) / inventoryBackroundTexture.getSize().y
        );
        Sprite dropHeldItem; // найдешь снизу
// снизу начиная с for и до push back происходит создание ячеек инвентаря
        for (int i = 0; i < rows; i++) {
            std::vector<RectangleShape> row;
            for (int j = 0; j < columns; j++) {
                RectangleShape cell(Vector2f(size, size));
                cell.setPosition(x + j * size, y + i * size);
                cell.setFillColor(Color::Transparent);
                cell.setOutlineThickness(2.f);
                cell.setOutlineColor(Color::White);
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
                items[i][j].setCharacterSize(20);
                items[i][j].setFillColor(Color::White);
                items[i][j].setPosition(x + j * size + 10, y + i * size + 10);

                //sprites[i][j].setTexture(phoneTexture);
                sprites[i][j].setPosition(x + j * size + 5, y + i * size + 5);
                //float scale = (size - 10) / phoneTexture.getSize().x;
                //sprites[i][j].setScale(scale, scale);
            }
        }
//телефон как начальный предмет
        addItemAtPosition(0, 0, "Phone", phoneTexture);

        cursor.setSize(Vector2f(size, size));
        cursor.setFillColor(Color(0, 0, 0, 0));
        cursor.setOutlineThickness(3.f);
        cursor.setOutlineColor(Color::Yellow);
        cursor.setPosition(x, y);
    }
//загружает текстуру предмета по имени и сохраняет её в itemTextures
    void loadItemTexture(const std::string& itemName, const std::string& textureFile)
    {
        Texture texture;
        if (texture.loadFromFile(textureFile))
        {
            itemTextures[itemName]=texture;
        }
        else
        {
            std::cout<<"Error load texture"<< itemName<<std::endl;

        }
    }
//Рисует фон сетку предметы названия спрайт удерж предмета курсор
    void draw(RenderWindow& window) {
        if (!isVisible) return;

        window.draw(inventoryBackroundSprite);
        for (auto& row : grid)
            for (auto& cell : row)
                window.draw(cell);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (items[i][j].getString() != "")
                    window.draw(items[i][j]);
                if (hasItem[i][j]) window.draw(sprites[i][j]);
            }
        }

        if (isHolding)
            window.draw(heldSprite);

        window.draw(cursor);
    }
//добавляет предмет в первую свободную ячейку  и использует itemTextures для установки текстуры и масштабирует спрайт
    bool addItem(const std::string& item) { //this is for future
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (!hasItem[i][j]) {
                    items[i][j].setString(item);
                    hasItem[i][j] = true;
                    sprites[i][j].setTexture(itemTextures[item]);
                    sprites[i][j].setPosition(position.x + j * size + 5, position.y + i * size + 5);
                    float scale = (size - 10) / itemTextures[item].getSize().x;
                    sprites[i][j].setScale(scale, scale);
                    return true;
                    
                }
            }
        }
        return false;
    }

    void setVisibility(bool visibility) { //показывать или скрывать инвентарь аа??
        isVisible = visibility;
    }
//обрабатывает нажатия клавиш для перемещения курсора, реализует перетаскивание предметов мышкой
//если предмет не помещён обратно в инвентарь, создаётся выброшенный предмет (DroppedItem)
// BIG SHIT GOVNO PIDIDI
    void handleInput(RenderWindow& window, Vector2f characterPosition) {
        if (!isVisible) return;

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

        if (isHolding)
            heldSprite.setPosition(mousePos - mouseOffset);

        if (!Mouse::isButtonPressed(Mouse::Left) && isHolding) {
            bool placedInInventory = false;
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++) {
                    if (grid[i][j].getGlobalBounds().contains(mousePos) && !hasItem[i][j]) {
                        items[i][j].setString(heldItem);
                        hasItem[i][j] = true;
                        sprites[i][j] = heldSprite;
                        sprites[i][j].setPosition(grid[i][j].getPosition() + Vector2f(5, 5));
                        isHolding = false;
                        placedInInventory = true;
                        break;
                    }
                }
                if (placedInInventory) break;
            }

            if (!placedInInventory) {
                DroppedItem dropped;
                dropped.name = heldItem;
                if( heldItem=="Phone")
                {
                    dropped.type = "Gadget";
                    dropped.description ="Used to call lovely pididi";
                }
                //else if( heldItem==...)  ITS FOR FUTURE WE NEED TO ADD LIKE THIS
                dropped.position = characterPosition - Vector2f(0, -40);
                dropped.sprite = heldSprite;
                dropped.sprite.setPosition(dropped.position);
                dropped.label.setFont(font);
                dropped.label.setString(dropped.name);
                dropped.label.setCharacterSize(18);
                dropped.label.setFillColor(Color::White);
                dropped.label.setPosition(dropped.position + Vector2f(-10, -25));

                droppedItems.push_back(dropped);
                isHolding = false;
            }
        }
    }
//проверяет сть ли поблизости выброшенные предметы и подбирает их в инвентарь если игрок рядом.
    void tryPickupItem(Vector2f characterPos) {
        float pickupRadius = 80.f;
        for (auto it = droppedItems.begin(); it != droppedItems.end(); ++it) {
            float dx = it->position.x - characterPos.x;
            float dy = it->position.y - characterPos.y;
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance <= pickupRadius) {
                if (addItem(it->name)) {
                    droppedItems.erase(it);
                }
                break;
            }
        }
    }
//геттер -он возвращает список выброшенных предметов для отображения их снаружи класса
    const std::vector<DroppedItem>& getDroppedItems() const {
        return droppedItems;
    } 
    Sprite dropHeldItem() //возвращает спрайт удерживаемого предмета и сбрасывает флаг удержания (isHolding = false)
    {
        Sprite dropped;
        if(isHolding)
        {
            dropped=heldSprite;
            isHolding=false;
        }
        return dropped; 
    }
};

int main() {
    RenderWindow window(VideoMode(1200, 1000), "Character Movement");
    window.setFramerateLimit(60);

    Texture texture;
    if (!texture.loadFromFile("person.jpg")) return -1;

    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("pididi.png")) return -1;

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
    const float minX = 100.f, maxX = 1200.f;
    const float minY = 100.f, maxY = 1000.f;
    inventory.loadItemTexture("Phone","phone.png"); // так происходит загрузка  предмтов в игру , для других пишем аналогичную строку ( название, название файла)
    //for future
    Clock clock;
    Clock animationClock;
    bool flag = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        Vector2f movement(0.f, 0.f);
        bool spriteChanged = false;
        if(!flag)
        {
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            movement.x = -moveSpeed;
            if (currentSprite != A) { currentSprite = A; spriteChanged = true; }
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            movement.x = moveSpeed;
            if (currentSprite != D) { currentSprite = D; spriteChanged = true; }
        }
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            movement.y = -moveSpeed;
            if (currentSprite != W) { currentSprite = W; spriteChanged = true; }
        }
        if (Keyboard::isKeyPressed(Keyboard::S)) {
            movement.y = moveSpeed;
            if (currentSprite != S) { currentSprite = S; spriteChanged = true; }
        }
        }
        float charWidth = frameWidth * character.getScale().x;
        float charHeight = frameHeight * character.getScale().y;

        if (character.getPosition().x + movement.x >= minX &&
            character.getPosition().x + movement.x + charWidth <= maxX)
            character.move(movement.x, 0);

        if (character.getPosition().y + movement.y >= minY &&
            character.getPosition().y + movement.y + charHeight <= maxY)
            character.move(0, movement.y);

        if (Keyboard::isKeyPressed(Keyboard::I)) {
            flag = !flag;
            inventory.setVisibility(flag);
            while (Keyboard::isKeyPressed(Keyboard::I)) {}
        }

        if (Keyboard::isKeyPressed(Keyboard::E)) {
            inventory.tryPickupItem(character.getPosition());
            while (Keyboard::isKeyPressed(Keyboard::E)) {}
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
        if(Keyboard::isKeyPressed(Keyboard::E))
        {
            Sprite dropped = inventory.dropHeldItem();
            dropped.setPosition(character.getPosition());
            dropped.setScale(0.1f,0.1f);
            //droppedItems.push_back(dropped);
            while(Keyboard::isKeyPressed(Keyboard::E)) {}
        }
        window.clear();
        window.draw(backgroundSprite);
        window.draw(character);

        if (flag)
            inventory.handleInput(window, character.getPosition());
            for (const auto& item : inventory.getDroppedItems()) {
                window.draw(item.sprite);
            
                FloatRect itemBounds = item.sprite.getGlobalBounds();
                FloatRect characterBounds = character.getGlobalBounds();
            // табличка
                if (characterBounds.intersects(itemBounds)) {
                    RectangleShape infoBox(Vector2f(150, 60));
                    infoBox.setFillColor(Color(0, 0, 0, 180));
                    infoBox.setOutlineColor(Color::White);
                    infoBox.setOutlineThickness(2);
                    infoBox.setPosition(item.sprite.getPosition() + Vector2f(0, -70));
            
                    Text infoText;
                    infoText.setFont(inventory.getFont()); 
                    infoText.setCharacterSize(16);
                    infoText.setFillColor(Color::White);
                    infoText.setString("Name: " + item.name + "\nType: Gadget"+ item.type + "\n" + item.description);
                    infoText.setPosition(infoBox.getPosition() + Vector2f(10, 5));
            
                    // шрифт
                    static Font cachedFont;
                    static bool fontLoaded = false;
                    if (!fontLoaded) {
                        cachedFont.loadFromFile("arialmt.ttf");
                        fontLoaded = true;
                    }
                    infoText.setFont(cachedFont);
            
                    window.draw(infoBox); // window.draw думаю сам поймешь что и как отрисовывается
                    window.draw(infoText);
                }
            }

        inventory.draw(window);
        window.display();
    }

    return 0;
}
