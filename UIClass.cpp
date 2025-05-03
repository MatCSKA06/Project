 #include <SFML/Graphics.hpp>
#include <iostream>
#include "UIClass.hpp"
#include <algorithm>
#include <map>
#include <cmath>
#include <string>
#include <vector>

Inventory::Inventory(float x, float y, int rows, int columns, float size)
    : position(x, y), rows(rows), columns(columns), size(size)
{
    font.loadFromFile("Fonts/arialmt.ttf");
    phoneTexture.loadFromFile("Assets/phone.png");
    inventoryBackroundTexture.loadFromFile("Assets/Inventory.png");
    handSlotTexture.loadFromFile("Assets/texturehandlot.jpeg");
    inventoryBackroundSprite.setTexture(inventoryBackroundTexture);
    inventoryBackroundSprite.setPosition(x, y);
    inventoryBackroundSprite.setScale(
        (size * columns) / inventoryBackroundTexture.getSize().x,
        (size * rows) / inventoryBackroundTexture.getSize().y
    );

    for (int i = 0; i < rows; i++) {
        std::vector<sf::RectangleShape> row;
        for (int j = 0; j < columns; j++) {
            sf::RectangleShape cell(sf::Vector2f(size, size));
            cell.setPosition(x + j * size, y + i * size);
            cell.setFillColor(sf::Color::Transparent);
            cell.setOutlineThickness(2.f);
            cell.setOutlineColor(sf::Color::White);
            row.push_back(cell);
         }
        grid.push_back(row);
    }

    items.resize(rows, std::vector<sf::Text>(columns));
    sprites.resize(rows, std::vector<sf::Sprite>(columns));
    hasItem.resize(rows, std::vector<bool>(columns, false));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            items[i][j].setFont(font);
            items[i][j].setCharacterSize(20);
            items[i][j].setFillColor(sf::Color::White);
            items[i][j].setPosition(x + j * size + 10, y + i * size + 10);
            sprites[i][j].setPosition(x + j * size + 5, y + i * size + 5);
        }
    }

    addItemAtPosition(0, 0, "Phone", phoneTexture);

    cursor.setSize(sf::Vector2f(size, size));
    cursor.setFillColor(sf::Color(0, 0, 0, 0));
    cursor.setOutlineThickness(3.f);
    cursor.setOutlineColor(sf::Color::Yellow);
    cursor.setPosition(x, y);
    //table in inventory
    infoBox.setSize(sf::Vector2f(200,100));
    infoBox.setFillColor(sf::Color(0, 0, 0, 200));
    infoBox.setOutlineColor(sf::Color::White);
    infoBox.setOutlineThickness(2.f);
    infoText.setFont(font);
    infoText.setCharacterSize(16);
    infoText.setFillColor(sf::Color::White);

    //add a right and left hand slots 
    rightHand.setSize(sf::Vector2f(size,size));
    rightHand.setTexture(&handSlotTexture);
    rightHand.setOutlineThickness(2.f);
    rightHand.setOutlineColor(sf::Color::Cyan);
    rightHand.setPosition(position.x+columns*size+50, position.y); //indent right for 50 pixels, but we dont change a position OY

    leftHand.setSize(sf::Vector2f(size,size));
    leftHand.setTexture(&handSlotTexture);
    leftHand.setOutlineThickness(2.f);
    leftHand.setOutlineColor(sf::Color::Cyan);
    leftHand.setPosition(position.x+columns*size+50, position.y+size+20); //indent right for 50 pixels, but we CHANGE a position OY

    //texts for hands
    rightHandItemText.setFont(font);
    rightHandItemText.setCharacterSize(16);
    rightHandItemText.setFillColor(sf::Color::White);
    rightHandItemText.setPosition(rightHand.getPosition()+sf::Vector2f(10,10));

    leftHandItemText.setFont(font);
    leftHandItemText.setCharacterSize(16);
    leftHandItemText.setFillColor(sf::Color::White);
    leftHandItemText.setPosition(leftHand.getPosition()+sf::Vector2f(10,10));




}

void Inventory::updateCursorPosition() {
    cursor.setPosition(position.x + cursorX * size, position.y + cursorY * size);
}

void Inventory::addItemAtPosition(int row, int col, const std::string& name, const sf::Texture& texture) {
    items[row][col].setString(name);
    hasItem[row][col] = true;
    sprites[row][col].setTexture(texture);
    sprites[row][col].setPosition(position.x + col * size + 5, position.y + row * size + 5);
    float scale = 0.1f;
    sprites[row][col].setScale(scale, scale);
}

const sf::Font& Inventory::getFont() const {
    return font;
}

void Inventory::loadItemTexture(const std::string& itemName, const std::string& textureFile) {
    sf::Texture texture;
    if (texture.loadFromFile(textureFile)) {
        itemTextures[itemName] = texture;
    } else {
        std::cout << "Error loading texture: " << itemName << std::endl;
    }
}
void Inventory::registerItemInfo(const std::string& itemName, const std::string& type, const std::string& description )
{
    itemInfoMap[itemName]=ItemInfo{type, description};
}

void Inventory::draw(sf::RenderWindow& window) {
    if (!isVisible) return;

    window.draw(inventoryBackroundSprite);
    for (auto& row : grid)
        for (auto& cell : row)
            window.draw(cell);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++) {
            if (items[i][j].getString() != "")
                window.draw(items[i][j]);
            if (hasItem[i][j])
                window.draw(sprites[i][j]);
        }

    if (isHolding)
        window.draw(heldSprite);

    window.draw(cursor);
    //adding a table
    if(isVisible)
    {
        
        if(hasItem[cursorY][cursorX]);
        {   std::string hoveredItem=items[cursorY][cursorX].getString();
            auto it=itemInfoMap.find(hoveredItem);
            if(it !=itemInfoMap.end()){
            std::string info = "Name:"+hoveredItem+"\nType:"+it->second.type+"\n"+it->second.description;
            infoText.setString(info);
            sf::Vector2f boxPos  =grid[cursorY][cursorX].getPosition()+sf::Vector2f(size+5,0);
            if(boxPos.x +infoBox.getSize().x>window.getSize().x)
            {
                boxPos.x=grid[cursorY][cursorX].getPosition().x-infoBox.getSize().x-5;
            }
            infoBox.setPosition(boxPos);
            infoText.setPosition(boxPos+sf::Vector2f(10,10));
            window.draw(infoBox);
            window.draw(infoText);
            }
        }
    }
    //thats all

    ///////////////////////////

    //draw a hand slots
    window.draw(rightHand);
    window.draw(leftHand);
    if(hasRightHandItem)
        window.draw(rightHandSprite);
    else
        window.draw(rightHandItemText);
    if(hasLeftHandItem)
        window.draw(leftHandSprite);
    else
        window.draw(leftHandItemText);



        
}

bool Inventory::addItem(const std::string& item) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            if (!hasItem[i][j]) {
                items[i][j].setString(item);
                hasItem[i][j] = true;
                sprites[i][j].setTexture(itemTextures[item]);
                sprites[i][j].setPosition(position.x + j * size + 5, position.y + i * size + 5);
                float scale = (size - 10) / itemTextures[item].getSize().x;
                sprites[i][j].setScale(scale, scale);
                return true;
            }

    return false;
}

void Inventory::setVisibility(bool visibility) {
    isVisible = visibility;
}
void Inventory::handleInput(sf::RenderWindow& window, sf::Vector2f characterPosition) {
    if (!isVisible) return;

    // --- Управление клавишами ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && cursorX < columns - 1) {
        cursorX++;
        updateCursorPosition();
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {}
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && cursorX > 0) {
        cursorX--;
        updateCursorPosition();
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {}
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && cursorY > 0) {
        cursorY--;
        updateCursorPosition();
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {}
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && cursorY < rows - 1) {
        cursorY++;
        updateCursorPosition();
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {}
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
        if (hasItem[cursorY][cursorX]) {
            std::string newItemName = items[cursorY][cursorX].getString();
            sf::Sprite newItemSprite = sprites[cursorY][cursorX];
    
            // Если правая занята
            if (hasRightHandItem) {
                if (!hasLeftHandItem) {
                    leftHandItemName = rightHandItemName;
                    leftHandSprite = rightHandSprite;
                    leftHandSprite.setPosition(leftHand.getPosition() + sf::Vector2f(5, 5));
                    hasLeftHandItem = true;
                } else {
                    addItem(rightHandItemName);
                }
            }
    
            // Кладем в правую руку
            rightHandItemName = newItemName;
            rightHandSprite = newItemSprite;
            rightHandSprite.setPosition(rightHand.getPosition() + sf::Vector2f(5, 5));
            hasRightHandItem = true;
    
            // Удаляем из инвентаря
            items[cursorY][cursorX].setString("");
            sprites[cursorY][cursorX].setTexture(sf::Texture());
            hasItem[cursorY][cursorX] = false;
    
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {}
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
        if (hasItem[cursorY][cursorX]) {
            std::string newItemName = items[cursorY][cursorX].getString();
            sf::Sprite newItemSprite = sprites[cursorY][cursorX];
    
            // Если левая занята
            if (hasLeftHandItem) {
                if (!hasRightHandItem) {
                    rightHandItemName = leftHandItemName;
                    rightHandSprite = leftHandSprite;
                    rightHandSprite.setPosition(rightHand.getPosition() + sf::Vector2f(5, 5));
                    hasRightHandItem = true;
                } else {
                    addItem(leftHandItemName);
                }
            }
    
            // Кладем в левую руку
            leftHandItemName = newItemName;
            leftHandSprite = newItemSprite;
            leftHandSprite.setPosition(leftHand.getPosition() + sf::Vector2f(5, 5));
            hasLeftHandItem = true;
    
            // Удаляем из инвентаря
            items[cursorY][cursorX].setString("");
            sprites[cursorY][cursorX].setTexture(sf::Texture());
            hasItem[cursorY][cursorX] = false;
    
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {}
        }
    }

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // --- Захват предмета из инвентаря ---
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isHolding) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (grid[i][j].getGlobalBounds().contains(mousePos) && hasItem[i][j]) {
                    isHolding = true;
                    heldItem = items[i][j].getString();
                    heldSprite = sprites[i][j];
                    items[i][j].setString("");
                    hasItem[i][j] = false;
                    mouseOffset = mousePos - heldSprite.getPosition();
                    break;
                }
            }
            if (isHolding) break;
        }
    }

    // --- Захват предмета из рук ---
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isHolding) {
        if (rightHand.getGlobalBounds().contains(mousePos) && hasRightHandItem) {
            isHolding = true;
            heldItem = rightHandItemName;
            heldSprite = rightHandSprite;
            hasRightHandItem = false;
            rightHandItemName.clear();
            mouseOffset = mousePos - heldSprite.getPosition();
            while (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {}
        }
        else if (leftHand.getGlobalBounds().contains(mousePos) && hasLeftHandItem) {
            isHolding = true;
            heldItem = leftHandItemName;
            heldSprite = leftHandSprite;
            hasLeftHandItem = false;
            leftHandItemName.clear();
            mouseOffset = mousePos - heldSprite.getPosition();
            while (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {}
        }
    }

    // --- Перемещение предмета за курсором ---
    if (isHolding) {
        heldSprite.setPosition(mousePos - mouseOffset);
    }

    // --- Отпускание мыши: вставка предмета ---
    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && isHolding) {
        bool placedInInventory = false;

        // Попытка вставить в инвентарь
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (grid[i][j].getGlobalBounds().contains(mousePos) && !hasItem[i][j]) {
                    items[i][j].setString(heldItem);
                    hasItem[i][j] = true;
                    sprites[i][j] = heldSprite;
                    sprites[i][j].setPosition(grid[i][j].getPosition() + sf::Vector2f(5, 5));
                    isHolding = false;
                    placedInInventory = true;
                    break;
                }
            }
            if (placedInInventory) break;
        }

        // Попытка вставить в правую руку
        if (!placedInInventory && rightHand.getGlobalBounds().contains(mousePos) && !hasRightHandItem) {
            rightHandItemName = heldItem;
            rightHandSprite = heldSprite;
            rightHandSprite.setPosition(rightHand.getPosition() + sf::Vector2f(5, 5));
            hasRightHandItem = true;
            isHolding = false;
            placedInInventory = true;
        }// Попытка вставить в левую руку
        if (!placedInInventory && leftHand.getGlobalBounds().contains(mousePos) && !hasLeftHandItem) {
            leftHandItemName = heldItem;
            leftHandSprite = heldSprite;
            leftHandSprite.setPosition(leftHand.getPosition() + sf::Vector2f(5, 5));
            hasLeftHandItem = true;
            isHolding = false;
            placedInInventory = true;
        }

        // Если не удалось — выбрасываем на землю
        if (!placedInInventory) {
            DroppedItem dropped;
            dropped.name = heldItem;
            auto it = itemInfoMap.find(heldItem);
            if (it != itemInfoMap.end()) {
                dropped.type = it->second.type;
                dropped.description = it->second.description;
            }
            dropped.position = characterPosition - sf::Vector2f(0, -40);
            dropped.sprite = heldSprite;
            dropped.sprite.setPosition(dropped.position);
            dropped.label.setFont(font);
            dropped.label.setString(dropped.name);
            dropped.label.setCharacterSize(18);
            dropped.label.setFillColor(sf::Color::White);
            dropped.label.setPosition(dropped.position + sf::Vector2f(-10, -25));
            droppedItems.push_back(dropped);
            isHolding = false;
        }
    }

    // --- Выброс предмета из руки по Delete под курсором ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete)) {
        if (rightHand.getGlobalBounds().contains(mousePos) && hasRightHandItem) {
            DroppedItem dropped;
            dropped.name = rightHandItemName;
            auto pididi = itemInfoMap.find(dropped.name);
            if (pididi != itemInfoMap.end()) {
                dropped.type = pididi->second.type;
                dropped.description = pididi->second.description;
            }
            dropped.sprite = rightHandSprite;
            dropped.position = characterPosition - sf::Vector2f(0, -40);
            dropped.sprite.setPosition(dropped.position);
            droppedItems.push_back(dropped);
            hasRightHandItem = false;
            rightHandItemName.clear();
        }
        else if (leftHand.getGlobalBounds().contains(mousePos) && hasLeftHandItem) {
            DroppedItem dropped;
            dropped.name = leftHandItemName;
            auto pididi = itemInfoMap.find(dropped.name);
            if (pididi != itemInfoMap.end()) {
                dropped.type = pididi->second.type;
                dropped.description = pididi->second.description;
            }
            dropped.sprite = leftHandSprite;
            dropped.position = characterPosition - sf::Vector2f(0, -40);
            dropped.sprite.setPosition(dropped.position);
            droppedItems.push_back(dropped);
            hasLeftHandItem = false;
            leftHandItemName.clear();
        }

        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete)) {}
    }
    // add items to inventory back by using backspace
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
    {
        if(rightHand.getGlobalBounds().contains(mousePos) && hasRightHandItem)
        {   
            if(addItem(rightHandItemName))
            {
                hasRightHandItem=false;
                rightHandItemName.clear();
            }
            while(sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {}
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
    {
        if(leftHand.getGlobalBounds().contains(mousePos) && hasLeftHandItem)
        {   
            if(addItem(leftHandItemName))
            {
                hasLeftHandItem=false;
                leftHandItemName.clear();
            }
        }
            while(sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {}

    }

    //chagle color from cyan to red,mouse pos
    if(rightHand.getGlobalBounds().contains(mousePos))
    {
        rightHand.setOutlineColor(sf::Color::Red);
    }
    else
    {
        rightHand.setOutlineColor(sf::Color::Cyan);
    }
    if(leftHand.getGlobalBounds().contains(mousePos))
    {
        leftHand.setOutlineColor(sf::Color::Red);
    }
    else
    {
        leftHand.setOutlineColor(sf::Color::Cyan);
    }
}

void Inventory::tryPickupItem(sf::Vector2f characterPos) {
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

const std::vector<Inventory::DroppedItem>& Inventory::getDroppedItems() const {
    return droppedItems;
}

std::vector<Inventory::DroppedItem>& Inventory::getDroppedItems() {
    return droppedItems;
}

sf::Sprite Inventory::dropHeldItem() {
    sf::Sprite dropped;
    if (isHolding) {
        dropped = heldSprite;
        isHolding = false;
    }
    return dropped;
} 