#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "UIClass.hpp"
#include <algorithm>
#include <map>
#include <cmath>
#include <string>
#include <vector>
//struct DroppedItem {
   //     std::string name;
    //    std::string type;
     //   std::string description;
      //  float alpha = 0.f;
      //  sf::Sprite sprite;
      //  sf::Vector2f position;
      //  sf::Text label;
   // };
  //  struct ItemInfo {
  //  std::string type;
  //  std::string description;
	//};

class Inventory {
private:
   struct DroppedItem {
        std::string name;
        std::string type;
        std::string description;
        float alpha = 0.f;
        sf::Sprite sprite;
        sf::Vector2f position;
        sf::Text label;
    };
    struct ItemInfo {
    std::string type;
    std::string description;
	};
    
    sf::RectangleShape infoBox; //adding a table in inventory
    sf::Text infoText; //its too
    std::map<std::string, sf::Texture> itemTextures;
    std::map<std::string, ItemInfo> itemInfoMap;
    std::vector<std::vector<sf::RectangleShape>> grid;
    std::vector<std::vector<sf::Text>> items;
    std::vector<std::vector<sf::Sprite>> sprites;
    std::vector<std::vector<bool>> hasItem;
    std::vector<DroppedItem> droppedItems;
    
    //adding a hand inventory
    sf::RectangleShape rightHand;
    sf::RectangleShape leftHand;
    sf::Text rightHandItemText;
    sf::Text leftHandItemText;
    std::string rightHandItemName;
    std::string leftHandItemName;
    sf::Sprite rightHandSprite;
    sf::Sprite leftHandSprite;
    bool hasRightHandItem = false; //first this
    bool hasLeftHandItem = false; //tihs too
// end
    sf::Font font;
    sf::Texture phoneTexture;
    sf::Texture inventoryBackroundTexture;
    sf::Sprite inventoryBackroundSprite;
    sf::Vector2f position;
    int rows, columns;
    float size;
    sf::RectangleShape cursor;
    sf::Color cursorColor;
    int cursorX = 0;
    int cursorY = 0;
    bool isHolding = false;
    std::string heldItem;
    sf::Sprite heldSprite;
    sf::Vector2f mouseOffset;
    void updateCursorPosition();
    void addItemAtPosition(int row, int col, const std::string& name, const sf::Texture& texture);

public:
    bool isVisible = false;
    Inventory(float x, float y, int rows, int columns, float size);
    const sf::Font& getFont() const;
    void loadItemTexture(const std::string& itemName, const std::string& textureFile);
    void registerItemInfo(const std::string& itemName, const std::string& type, const std::string& description);	
    void draw(sf::RenderWindow& window);
    bool addItem(const std::string& item);
    void setVisibility(bool visibility);
    void handleInput(sf::RenderWindow& window, sf::Vector2f characterPosition);
    void tryPickupItem(sf::Vector2f characterPos);
    const std::vector<DroppedItem>& getDroppedItems() const;
    std::vector<DroppedItem>& getDroppedItems();
    sf::Sprite dropHeldItem();
    Inventory(const Inventory&) = delete;
    Inventory& operator=(const Inventory&) = delete;
};
