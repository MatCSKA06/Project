#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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

enum class GameState {
    Splash,
    Menu,
    Playing,
    Settings,
    Exit
};

int main() {
    sf::RenderWindow window(
        sf::VideoMode(1920, 1080),
        "Devourer's Eddu",
        sf::Style::Fullscreen
    );
    window.setFramerateLimit(60);

//////////////////////////////////////////////////////////////////////////////////////////////////////

    // Начальный статус игры (заставка)
    GameState state = GameState::Splash;

//////////////////////////////////////////////////////////////////////////////////////////////////////

    // Загружаем тексуры кнопок в меню
    sf::Texture texPlay, texSettings, texExit;
    if (!texPlay.loadFromFile("Assets/Button_To_Play.png") or
    !texSettings.loadFromFile("Assets/Button_To_Settings.png") or
    !texExit.loadFromFile("Assets/Button_To_Exit.png"))
    {
        std::cerr << "Error loading menu button textures\n";
    }

    // Загружаем текстуры "активных" кнопок в меню
    sf::Texture texPlayActive, texSettingsActive, texExitActive;
    if (!texPlayActive.loadFromFile("Assets/Button_To_Play_Active.png") ||
    !texSettingsActive.loadFromFile("Assets/Button_To_Settings_Active.png") ||
    !texExitActive.loadFromFile("Assets/Button_To_Exit_Active.png"))
    {
        std::cerr << "Error loading active button textures\n";
    }
    
//////////////////////////////////////////////////////////////////////////////////////////////////////

    // Работаем с начальным логотипом выплывающим
    sf::Texture texLogo;
    if (!texLogo.loadFromFile("Assets/Company_Logo.png")) {
        std::cerr << "Error loading company logo\n";
    }
    sf::Sprite logoSprite(texLogo);
    logoSprite.setOrigin(texLogo.getSize().x / 2, texLogo.getSize().y / 2); // Центрируем
    logoSprite.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f); // Центр окна
    logoSprite.setColor(sf::Color(255, 255, 255, 0)); // Сначала полностью прозрачный

//////////////////////////////////////////////////////////////////////////////////////////////////////

    // Создаём спрайты кнопок
    sf::Sprite Button_To_Play(texPlay), 
    Button_To_Settings(texSettings), 
    Button_To_Exit(texExit);

    // Создаём спрайты "активных" кнопок
    sf::Sprite Button_To_Play_Active(texPlayActive), 
    Button_To_Settings_Active(texSettingsActive), 
    Button_To_Exit_Active(texExitActive);

//////////////////////////////////////////////////////////////////////////////////////////////////////

    // Одинаково их позиционируем
    float x = 1920.f/5 - texPlay.getSize().x/2;

    Button_To_Play.setPosition(x, 800.f);
    Button_To_Play_Active.setPosition(x, 800.f);

    Button_To_Settings.setPosition(x, 1070.f);
    Button_To_Settings_Active.setPosition(x, 1070.f);

    Button_To_Exit.setPosition(x, 1340.f);
    Button_To_Exit_Active.setPosition(x, 1340.f);


//////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::Texture texBackground;
    if (!texBackground.loadFromFile("Assets/Menu_Background.png")) {
        std::cerr << "Error loading menu background\n";
    }
    sf::Sprite menuBackground(texBackground);
    auto winSize = window.getSize();
    auto texSize = texBackground.getSize();
    menuBackground.setScale(
    float(winSize.x) / texSize.x,
    float(winSize.y) / texSize.y);

    Enemy MainEnemy(window.getSize());
    Player MainPlayer(window.getSize());
    sf::Clock clock;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    if (!backgroundTexture.loadFromFile("Assets/Background.png")) {
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

//////////////////////////////////////////////////////////////////////////////////////////////////////

    // Звуки и музыка
    sf::SoundBuffer Splash_Sound;
    if (!Splash_Sound.loadFromFile("Sounds/Splash_Sound.ogg")) std::cerr<<"Splash sound error\n";
    sf::Sound SplashSound(Splash_Sound);
    SplashSound.setVolume(30.0f);
    SplashSound.play();

    sf::Music Menu_Music;
    if (!Menu_Music.openFromFile("Music/hxlvv - forgive.ogg")) std::cerr<<"Menu music error\n";
    Menu_Music.setLoop(true);

//////////////////////////////////////////////////////////////////////////////////////////////////////

    bool inventoryVisible = false;

//////////////////////////////////////////////////////////////////////////////////////////////////////

    // Работаем с заставкой
    sf::Clock Splash_Clock; // Таймер заставки
    float Splash_Alpha = 0.f; // Прозрачность
    bool Logo_Fading_In = true;    // Сначала осветляемся
    const float Fade_Time_Splash = 1.5f;

//////////////////////////////////////////////////////////////////////////////////////////////////////

    // Работаем с меню
    sf::Clock Menu_Fade_Clock; // Таймер плавного появления меню !ТОЛЬКО ПОСЛЕ! заставки
    float Menu_Alpha = 0.f; // исходная прозрачность
    bool  Menu_Fading_In = false;
    const float Menu_Fade_Time = 2.5f; // длительность Fade In в секундах
    const float Menu_Standart_Maximum_Volume = 30.f;
    float Menu_Volume_Now = 0.0f;

//////////////////////////////////////////////////////////////////////////////////////////////////////

    // Ставим стандартный вид камеры, чтобы на нём показывать все менюшки
    sf::View Default_View = window.getDefaultView();

//////////////////////////////////////////////////////////////////////////////////////////////////////

    // Всё, что происходит при открытом окне
    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();
        if (deltaTime > 0.1f) deltaTime = 0.1f;

//////////////////////////////////////////////////////////////////////////////////////////////////////

        // Если видите event - это вызов определённого события
        sf::Event event;

//////////////////////////////////////////////////////////////////////////////////////////////////////

        while (window.pollEvent(event)) {

//////////////////////////////////////////////////////////////////////////////////////////////////////

            // Если окно закрыто пользователем - то закрываем окно
            if (event.type == sf::Event::Closed){
                window.close();
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////

            if (event.type == sf::Event::KeyReleased &&
                event.key.code == sf::Keyboard::Key::Escape)
            {
                state = GameState::Menu;     // сразу в меню
                Menu_Fading_In = false;      // отключаем плавный вход
                Menu_Alpha = 255.f;      // делаем меню полностью непрозрачным
            }
            // if (state == GameState::Menu && event.type == sf::Event::KeyReleased &&
            //     event.key.code == sf::Keyboard::Key::Escape)
            // {
            //     state = GameState::Menu;
            //     Menu_Fading_In = false;
            //     Menu_Alpha = 0.f;
            // }
        }

//////////////////////////////////////////////////////////////////////////////////////////////////////

        // Получаем позицию клика в режиме игры - меню, и меняем статус при нажатии соответствующей кнопки
        if (state == GameState::Menu && event.type == sf::Event::MouseButtonReleased
            && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f Click_Do_Bounds = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

            if (Button_To_Play.getGlobalBounds().contains(Click_Do_Bounds)) {
                state = GameState::Playing;
            }
            else if (Button_To_Settings.getGlobalBounds().contains(Click_Do_Bounds)) {
                 state = GameState::Settings;
             }
            else if (Button_To_Exit.getGlobalBounds().contains(Click_Do_Bounds)) {
                state = GameState::Exit;
            }
        }
        //Избегаем багов и на другую кнопку мышки ничего не делаем
        if (state == GameState::Menu && event.type == sf::Event::MouseButtonReleased
            && event.mouseButton.button == sf::Mouse::Right)
        {
            continue;
        }

//////////////////////////////////////////////////////////////////////////////////////////////////////
    
        // Кусочек логики инвентаря
        if (Keyboard::isKeyPressed(Keyboard::Key::I)) {
            inventoryVisible = !inventoryVisible;
            inventory.setVisibility(inventoryVisible);
            while (Keyboard::isKeyPressed(Keyboard::Key::I)) {}
        }

        if (!inventoryVisible) {
            MainPlayer.Keyboard_Handle_Input();
            MainPlayer.Update_Player_Position(deltaTime);
        }

//////////////////////////////////////////////////////////////////////////////////////////////////////

        window.clear();

//////////////////////////////////////////////////////////////////////////////////////////////////////

        // Распределение по состояниям игры
        switch (state) {

//////////////////////////////////////////////////////////////////////////////////////////////////////

            // Состояние игры - заставка
            case GameState::Splash: {
                window.setView(Default_View);
                float Elapsed_Splash_1 = Splash_Clock.getElapsedTime().asSeconds();
            
                if (Logo_Fading_In) { // Плавное появление при запуске окна
                    Splash_Alpha = std::min(255.f, Elapsed_Splash_1 * 255.f / Fade_Time_Splash);
                    if (Elapsed_Splash_1 >= Fade_Time_Splash) {
                        Logo_Fading_In = false;
                        Splash_Clock.restart();
                    }
                }
                else { // Плавное исчезновение после Fade-In стадии
                    if (Elapsed_Splash_1 > Fade_Time_Splash) {
                        float Elapsed_Splash_2 = Elapsed_Splash_1 - Fade_Time_Splash;
                        float progress = Elapsed_Splash_2 / Fade_Time_Splash;  
                        Menu_Music.setVolume(std::min(progress * Menu_Standart_Maximum_Volume, Menu_Standart_Maximum_Volume));
                        Splash_Alpha = std::max(0.f, 255.f - Elapsed_Splash_2 * 255.f / Fade_Time_Splash);
                        if (Elapsed_Splash_2 >= Fade_Time_Splash) {
                            state = GameState::Menu;
                            Menu_Alpha = 0.f;
                            Menu_Fading_In = true;
                            Menu_Fade_Clock.restart();

                                if (Menu_Music.getStatus() != sf::SoundSource::Playing) {
                                    Menu_Music.play();
                                    Menu_Music.setVolume(0.f);  // стартуем тихо
                                }
                        }
                    }
                }
            
                logoSprite.setColor(sf::Color(255,255,255, static_cast<Uint8>(Splash_Alpha)));
                window.draw(logoSprite);
                break;
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////

            // Состояние игры - меню
            case GameState::Menu:{
                window.setView(Default_View);

                // Лучше Vector2i, потому что дробные координаты в пиксельном мире это так себе
                sf::Vector2i Pixel_Cursor_Position = sf::Mouse::getPosition(window);

                // Преобразуем пиксельные координаты в мировые с учётом изменений размеров картинок
                sf::Vector2f Mouse_Cursor_Position = window.mapPixelToCoords(Pixel_Cursor_Position);

                float Elapsed_Menu = Menu_Fade_Clock.getElapsedTime().asSeconds();

                if (Menu_Fading_In) {
                    Menu_Alpha = std::min(255.f, Elapsed_Menu * 255.f / Menu_Fade_Time);

                    // На всякий случай заранее спасаемся от багов
                    if (Menu_Alpha >= 255.f){
                         Menu_Fading_In = false;
                    }
                }

                if (Menu_Fading_In) {
                    float Menu_Music_Alpha = std::min(Menu_Standart_Maximum_Volume, 
                                         Elapsed_Menu * Menu_Standart_Maximum_Volume / Menu_Fade_Time);
                    Menu_Music.setVolume(Menu_Music_Alpha);
                }

                if (Elapsed_Menu >= Menu_Fade_Time) {
                    Menu_Fading_In = false;
                    // На всякий случай зафиксируем полную громкость и непрозрачность
                    Menu_Alpha = 255.f;
                    Menu_Volume_Now = Menu_Standart_Maximum_Volume;
                    Menu_Music.setVolume(Menu_Volume_Now);
                }

                sf::Uint8 a = static_cast<sf::Uint8>(Menu_Alpha);
                menuBackground.setColor({255,255,255,a});

                Button_To_Play.setColor({255,255,255,a});
                Button_To_Settings.setColor({255,255,255,a});
                Button_To_Exit.setColor({255,255,255,a});

                Button_To_Play_Active.setColor({255,255,255,a});
                Button_To_Settings_Active.setColor({255,255,255,a});
                Button_To_Exit_Active.setColor({255,255,255,a});
            
                window.draw(menuBackground);
                if (Button_To_Play.getGlobalBounds().contains(Mouse_Cursor_Position))
                    window.draw(Button_To_Play_Active);
                else
                    window.draw(Button_To_Play);

                if (Button_To_Settings.getGlobalBounds().contains(Mouse_Cursor_Position))
                    window.draw(Button_To_Settings_Active);
                else
                    window.draw(Button_To_Settings);

                if (Button_To_Exit.getGlobalBounds().contains(Mouse_Cursor_Position))
                    window.draw(Button_To_Exit_Active);
                else
                    window.draw(Button_To_Exit);

                break;
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////

            // Состояние игры - игровой процесс
            case GameState::Playing:{
                camera.setCenter(MainPlayer.getCameraCenterOffset());
                window.setView(camera);
                window.draw(backgroundSprite);
                MainEnemy.Draw_Enemy(window);
                MainPlayer.Draw_Player(window);


//////////////////////////////////////////////////////////////////////////////////////////////////////

                // Это тоже по хорошему (всю логику) впихнуть в класс и вызывать как функции, но пока тут
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
                    //adding a "TRYPICKUP" Zone for the items
                    if (item.alpha>0.f) {
                        sf::CircleShape pickUpZone(80.f); 
                        pickUpZone.setOrigin(80.f,80.f); 
                        //het a left-up angle of the sprite position, add a vector from this angle to the center
                        pickUpZone.setPosition(item.sprite.getPosition()+sf::Vector2f(item.sprite.getGlobalBounds().width/2,item.sprite.getGlobalBounds().height/2));
                        // make a semi-transparent field by using a static cast divide by 2
                        pickUpZone.setFillColor(sf::Color(255,255,255,0)); 
                        pickUpZone.setOutlineColor(sf::Color(255,255,255,static_cast<sf::Uint8>(item.alpha/2)));
                        pickUpZone.setOutlineThickness(3.f);
                        window.draw(pickUpZone);
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
                break;
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////

            // Состояние игры - настройки
            case GameState::Settings:{
                window.setView(Default_View);
                // рисуем экран настроек
                // (можно просто вывести текст или другую UI)
            break;
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////

            // Состояние игры - Выход из игры
            case GameState::Exit:{
                window.close();
            // рисуем экран авторов
                break;
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////

        }
        window.display();
    }
    return 0;
}