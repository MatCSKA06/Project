#include <SFML/Graphics.hpp>
#include <iostream>
#include "EnemyClass.hpp"
#include "PlayerClass.hpp"
#include "UIClass.hpp"
#include <algorithm>
#include <map>
#include <cmath>
#include <string>
#include <vector>

using namespace sf;

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Devourer's Eddu");
    window.setFramerateLimit(60);

    Enemy MainEnemy(window.getSize());
    Player MainPlayer(window.getSize());
    sf::Clock clock;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    if (!backgroundTexture.loadFromFile("Assets/MIPT.jpg")) {
        std::cerr << "Error loading background texture!\n";
    }
    backgroundSprite.setTexture(backgroundTexture);

    sf::View camera(sf::FloatRect(0, 0, 1920, 1080));
    sf::View UIView = window.getDefaultView();
    UIView.move(600.f, 300.f);

    Inventory inventory(600.f, 300.f, 4, 4, 150.f); 
    inventory.loadItemTexture("Phone", "Assets/phone.png");
    inventory.registerItemInfo("Phone", "Gadget", "Using to call pididi");
    inventory.loadItemTexture("Gun", "Assets/Berreta_Texture.png");
    inventory.registerItemInfo("Gun", "Pistol", "kill pididi");
    inventory.loadItemTexture("Unknown Key", "Assets/Key_Texture.png");
    inventory.loadItemTexture("Key", "Assets/Key_Texture.png");

    inventory.addItem("Gun");
    inventory.addItem("Key");

    bool inventoryVisible = false;

    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Key::I)) {
            inventoryVisible = !inventoryVisible;
            inventory.setVisibility(inventoryVisible);
            while (Keyboard::isKeyPressed(Keyboard::Key::I)) {}
        }

        if (!inventoryVisible) {
            MainPlayer.Keyboard_Handle_Input();
            MainPlayer.Update_Player_Position(deltaTime);
        }

        camera.setCenter(MainPlayer.getCameraCenterOffset());
        window.setView(camera);

        window.clear();
        window.draw(backgroundSprite);
        MainEnemy.Draw_Enemy(window);
        MainPlayer.Draw_Player(window);

        for (auto& item : inventory.getDroppedItems()) {
            window.draw(item.sprite);

            FloatRect itemBounds = item.sprite.getGlobalBounds();
            FloatRect characterBounds = MainPlayer.getGlobalBounds();
            bool isNear = characterBounds.intersects(itemBounds);

            if (isNear && item.alpha < 255.f)
                item.alpha += 300.f * deltaTime;
            else if (!isNear && item.alpha > 0.f)
                item.alpha -= 300.f * deltaTime;

            item.alpha = std::clamp(item.alpha, 0.f, 255.f);

            if (item.alpha > 0.f) {
                RectangleShape infoBox(Vector2f(200, 80));
                infoBox.setFillColor(Color(0, 0, 0, static_cast<Uint8>(item.alpha)));
                infoBox.setOutlineColor(Color(0, 255, 0, static_cast<Uint8>(item.alpha)));
                infoBox.setOutlineThickness(2);
                infoBox.setPosition(item.sprite.getPosition() + Vector2f(0, -70));

                Text infoText;
                static Font cachedFont;
                static bool fontLoaded = false;
                if (!fontLoaded) {
                    cachedFont.loadFromFile("Fonts/arialmt.ttf");
                    fontLoaded = true;
                }
                infoText.setFont(cachedFont);
                infoText.setCharacterSize(16);
                infoText.setFillColor(Color(255, 255, 255, static_cast<Uint8>(item.alpha)));
                infoText.setString("Name: " + item.name + "\nType: " + item.type + "\n" + item.description);
                infoText.setPosition(infoBox.getPosition() + Vector2f(10, 5));

                window.draw(infoBox);
                window.draw(infoText);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Key::E)) {
            auto& droppedItems = inventory.getDroppedItems();
            for (auto it = droppedItems.begin(); it != droppedItems.end();) {
                FloatRect itemBounds = it->sprite.getGlobalBounds();
                FloatRect characterBounds = MainPlayer.getGlobalBounds();
                bool isNear = characterBounds.intersects(itemBounds);

                if (isNear && it->alpha > 0.f) {
                    inventory.addItem(it->name);
                    it = droppedItems.erase(it);
                    break;
                } else {
                    ++it;
                }
            }
            while (Keyboard::isKeyPressed(Keyboard::Key::E)) {}
        }

        window.setView(UIView);
        if (inventoryVisible) {
            inventory.handleInput(window, MainPlayer.getPosition());
        }

        inventory.draw(window);
        window.display();
    }
}
