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

using namespace sf; // Побочка с ссылкой на то, что где-то я упущу этот нюанс в начале

//////////////////////////////////////////////////////////////////////////////////////////////////////

// Разбиение игры по текущим статусам (положениям)
enum class GameState {
    Splash,
    Menu,
    Playing,
    Settings,
    Exit,
    Playing_Menu
};

//////////////////////////////////////////////////////////////////////////////////////////////////////

// Функция нормализации размеров спрайтов
sf::Vector2f Normalize_Sprite_Scale(
    const sf::Vector2u& windowSize,
    const sf::Vector2u& spriteSize,
    float targetWidthRatio = 1.0f,
    float targetHeightRatio = 1.0f) 
{
    float scaleX = (windowSize.x * targetWidthRatio) / static_cast<float>(spriteSize.x);
    float scaleY = (windowSize.y * targetHeightRatio) / static_cast<float>(spriteSize.y);
    return sf::Vector2f(scaleX, scaleY);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    sf::RenderWindow window(
        sf::VideoMode(1920, 1080),
        "Devourer's Eddu",
        sf::Style::Fullscreen
    );
    window.setFramerateLimit(60);
    // Размер текущего окна (где игра запущена)
    auto winSize = window.getSize();

//////////////////////////////////////////////////////////////////////////////////////////////////////

    // Начальный статус игры (заставка)
    GameState state = GameState::Splash;

//////////////////////////////////////////////////////////////////////////////////////////////////////

    // Загружаем текстуру, ставим спрайт курсора, нормируем его
    sf::Texture Cursor_Texture;
    if (!Cursor_Texture.loadFromFile("Assets/Custom_Cursor.png")) {
        std::cerr << "Error loading cursor texture\n";
    }
    sf::Sprite Custom_Cursor(Cursor_Texture);

//////////////////////////////////////////////////////////////////////////////////////////////////////

    // Работаем с начальным логотипом выплывающим (загружаем, ставим, нормируем, центруем, убираем прозрачность)
    sf::Texture texLogo;
    if (!texLogo.loadFromFile("Assets/Company_Logo.png")) {
        std::cerr << "Error loading company logo\n";
    }
    sf::Sprite logoSprite(texLogo);

    sf::Vector2f Logo_Sprite_Now_Size = Normalize_Sprite_Scale(window.getSize(), 
                                                texLogo.getSize(),
                                                1.0f, 0.77f);
    logoSprite.setScale(Logo_Sprite_Now_Size);

    logoSprite.setOrigin(texLogo.getSize().x / 2, texLogo.getSize().y / 2); // Центрируем
    logoSprite.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f); // Центр окна
    logoSprite.setColor(sf::Color(255, 255, 255, 0)); // Сначала полностью прозрачный

//////////////////////////////////////////////////////////////////////////////////////////////////////

    // Загружаем тексуры кнопок в меню
    sf::Texture texPlay, texSettings, texExit, texQuit;
    if (!texPlay.loadFromFile("Assets/Button_To_Play.png") or
    !texSettings.loadFromFile("Assets/Button_To_Settings.png") or
    !texExit.loadFromFile("Assets/Button_To_Exit.png") or
    !texQuit.loadFromFile("Assets/Button_To_Quit.png"))
    {
        std::cerr << "Error loading menu button textures\n";
    }

    // Загружаем текстуры "активных" кнопок в меню
    sf::Texture texPlayActive, texSettingsActive, texExitActive, texQuitActive;
    if (!texPlayActive.loadFromFile("Assets/Button_To_Play_Active.png") or
    !texSettingsActive.loadFromFile("Assets/Button_To_Settings_Active.png") or
    !texExitActive.loadFromFile("Assets/Button_To_Exit_Active.png") or
    !texQuitActive.loadFromFile("Assets/Button_To_Quit_Active.png"))
    {
        std::cerr << "Error loading active button textures\n";
    }
    
//////////////////////////////////////////////////////////////////////////////////////////////////////

    // Создаём и нормируем спрайты кнопок
    sf::Sprite Button_To_Play(texPlay), 
    Button_To_Settings(texSettings), 
    Button_To_Exit(texExit),
    Button_To_Quit(texQuit);

    float targetX = (0.32f/1.2f);
    float targetY = (0.18f/1.2f);

    sf::Vector2f Button_To_Play_Now_Size = Normalize_Sprite_Scale(window.getSize(), 
                                            texPlay.getSize(),
                                            targetX, targetY);
    Button_To_Play.setScale(Button_To_Play_Now_Size);

    sf::Vector2f Button_To_Settings_Now_Size = Normalize_Sprite_Scale(window.getSize(), 
                                                texSettings.getSize(),
                                                targetX, targetY);
    Button_To_Settings.setScale(Button_To_Settings_Now_Size);

    sf::Vector2f Button_To_Exit_Now_Size = Normalize_Sprite_Scale(window.getSize(), 
                                            texExit.getSize(),
                                            targetX, targetY);
    Button_To_Exit.setScale(Button_To_Exit_Now_Size);

    sf::Vector2f Button_To_Quit_Now_Size = Normalize_Sprite_Scale(window.getSize(), 
                                            texQuit.getSize(),
                                            targetX, targetY);
    Button_To_Quit.setScale(Button_To_Quit_Now_Size);

//////////////////////////////////////////////////////////////////////////////////////////////////////

    // Создаём и нормируем спрайты "активных" кнопок
    sf::Sprite Button_To_Play_Active(texPlayActive), 
    Button_To_Settings_Active(texSettingsActive), 
    Button_To_Exit_Active(texExitActive),
    Button_To_Quit_Active(texQuitActive);

    sf::Vector2f Button_To_Play_Active_Now_Size = Normalize_Sprite_Scale(window.getSize(), 
                                                    texPlayActive.getSize(),
                                                    targetX, targetY);
    Button_To_Play_Active.setScale(Button_To_Play_Active_Now_Size);

    sf::Vector2f Button_To_Settings_Active_Now_Size = Normalize_Sprite_Scale(window.getSize(), 
                                                    texSettingsActive.getSize(),
                                                    targetX, targetY);
    Button_To_Settings_Active.setScale(Button_To_Settings_Active_Now_Size);

    sf::Vector2f Button_To_Exit_Active_Now_Size = Normalize_Sprite_Scale(window.getSize(), 
                                                texExitActive.getSize(),
                                                targetX, targetY);
    Button_To_Exit_Active.setScale(Button_To_Exit_Active_Now_Size);

    sf::Vector2f Button_To_Quit_Active_Now_Size = Normalize_Sprite_Scale(window.getSize(), 
                                                texQuitActive.getSize(),
                                                targetX, targetY);
    Button_To_Quit_Active.setScale(Button_To_Quit_Active_Now_Size);

//////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::Texture texMenuBackground, texBackgroundMenuPlayer, texBackgroundSettings;
    if (!texMenuBackground.loadFromFile("Assets/Menu_Background.png") or
    !texBackgroundMenuPlayer.loadFromFile("Assets/Playing_Menu_Background.png") or 
    !texBackgroundSettings.loadFromFile("Assets/Settings_Background.png")) {
        std::cerr << "Error loading backgrounds\n";
    }

    // Ставим текстуры на спрайты фонов (меню начального, меню внутри игрового процесса)
    sf::Sprite Menu_Background(texMenuBackground);
    sf::Sprite Playing_Menu_Background(texBackgroundMenuPlayer);
    sf::Sprite Settings_Background(texBackgroundSettings);

    // Нормируем и ставим размеры спрайтов фонов под текущее разрешение экрана
    sf::Vector2f Menu_Background_Now_Size = Normalize_Sprite_Scale(window.getSize(), 
                                            texMenuBackground.getSize(),
                                            1.0f, 1.0f);
    Menu_Background.setScale(Menu_Background_Now_Size);

    sf::Vector2f Playing_Menu_Background_Now_Size = Normalize_Sprite_Scale(window.getSize(), 
                                                    texBackgroundMenuPlayer.getSize(),
                                                    1.0f, 1.0f);
    Playing_Menu_Background.setScale(Playing_Menu_Background_Now_Size);

    sf::Vector2f Settings_Background_Now_Size = Normalize_Sprite_Scale(window.getSize(), 
                                                texBackgroundSettings.getSize(),
                                                1.0f, 1.0f);
    Settings_Background.setScale(Settings_Background_Now_Size);

    Enemy MainEnemy(window.getSize());
    Player MainPlayer(window.getSize());
    sf::Clock clock;

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("Assets/Background.png")) {
        std::cerr << "Error loading background texture!\n";
    }
    sf::Sprite backgroundSprite(backgroundTexture);

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
    Menu_Music.setLoop(true); // От слова луп (петля) - залупливаем музыку в меню :)

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

    // Ставим начальный чекер сотояния. *Ничего не произошло*
    std::string Last_Action_Checker = "Nothing Happened";

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

            // Костыль - нужен для шага с проверкой статуса
            sf::Vector2f Click_Do_Bounds_Transition = window.mapPixelToCoords({event.mouseButton.x, 
                event.mouseButton.y});

//////////////////////////////////////////////////////////////////////////////////////////////////////

            if (state == GameState::Menu && 
                Button_To_Settings.getGlobalBounds().contains(Click_Do_Bounds_Transition))
            {
                Last_Action_Checker = "From Menu to Settings";
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////

            else if (state == GameState::Menu &&
                Button_To_Play.getGlobalBounds().contains(Click_Do_Bounds_Transition))
            {
                Last_Action_Checker = "From Menu to Playing";
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////

            else if (state == GameState::Playing_Menu &&
                Button_To_Settings.getGlobalBounds().contains(Click_Do_Bounds_Transition))
            {
                Last_Action_Checker = "From Playing_Menu to Settings";
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////

            else if (state == GameState::Playing_Menu &&
                Button_To_Quit.getGlobalBounds().contains(Click_Do_Bounds_Transition))
            {
                Last_Action_Checker = "From Playing_Menu to Menu";
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////

            // Работаем с переходами, связанными с Escape
            if (event.type == sf::Event::KeyReleased && 
                event.key.code == sf::Keyboard::Key::Escape) {
                if (state == GameState::Playing) {
                    state = GameState::Playing_Menu;
                    Last_Action_Checker = "From Playing to Playing_Menu";
                }
                else if (state == GameState::Playing_Menu) {
                    state = GameState::Playing;
                    Last_Action_Checker = "From Playing_Menu to Playing";
                }
                else if (state == GameState::Settings &&
                (Last_Action_Checker == "From Playing_Menu to Settings")) {
                    state = GameState::Playing_Menu;
                    Last_Action_Checker = "From Settings to Playing_Menu";
                }
                else if (state == GameState::Settings &&
                (Last_Action_Checker == "From Menu to Settings")) {
                    state = GameState::Menu;
                    Last_Action_Checker = "From Settings to Menu";
                }
            }

//////////////////////////////////////////////////////////////////////////////////////////////////////

        }

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

        if (state == GameState::Playing_Menu && event.type == sf::Event::MouseButtonReleased
            && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f Click_Do_Bounds = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

            if (Button_To_Play.getGlobalBounds().contains(Click_Do_Bounds)) {
                state = GameState::Playing;
            }
            else if (Button_To_Settings.getGlobalBounds().contains(Click_Do_Bounds)) {
                 state = GameState::Settings;
             }
            else if (Button_To_Quit.getGlobalBounds().contains(Click_Do_Bounds)) {
                state = GameState::Menu;
            }
        }

        //Избегаем багов и на другую кнопку мышки ничего не делаем
        if (state == GameState::Menu && event.type == sf::Event::MouseButtonReleased
            && event.mouseButton.button == sf::Mouse::Right)
        {
            continue;
        }
        if (state == GameState::Playing_Menu && event.type == sf::Event::MouseButtonReleased
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
        // Распределение по состояниям игры
        switch (state) {

//////////////////////////////////////////////////////////////////////////////////////////////////////

            // Состояние игры - заставка
            case GameState::Splash: {
                window.setMouseCursorVisible(false);
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
                window.setMouseCursorVisible(false);
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

                auto Window_Size_Now = window.getSize();
                float x = Window_Size_Now.x/5 - Button_To_Play.getScale().x/2;
                // std::cout << Window_Size_Now.x << texPlay.getSize().x << std::endl;

                sf::Uint8 a = static_cast<sf::Uint8>(Menu_Alpha);
                Menu_Background.setColor({255,255,255,a});

                Button_To_Play.setColor({255,255,255,a});
                Button_To_Settings.setColor({255,255,255,a});
                Button_To_Exit.setColor({255,255,255,a});

                Button_To_Play_Active.setColor({255,255,255,a});
                Button_To_Settings_Active.setColor({255,255,255,a});
                Button_To_Exit_Active.setColor({255,255,255,a});

                Button_To_Play.setPosition(x, 800.f);
                Button_To_Play_Active.setPosition(x, 800.f);
            
                Button_To_Settings.setPosition(x, 1070.f);
                Button_To_Settings_Active.setPosition(x, 1070.f);
            
                Button_To_Exit.setPosition(x, 1340.f);
                Button_To_Exit_Active.setPosition(x, 1340.f);
            
                window.draw(Menu_Background);
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

            case GameState::Playing_Menu:{
                window.setMouseCursorVisible(false);
                window.setView(Default_View);
                // Лучше Vector2i, потому что дробные координаты в пиксельном мире это так себе
                sf::Vector2i Pixel_Cursor_Position = sf::Mouse::getPosition(window);

                auto Window_Size_Now = window.getSize();
                float x = Window_Size_Now.x/2 - Button_To_Quit.getScale().x/2;
                std::cout << Window_Size_Now.x/2 << std::endl;

                // Преобразуем пиксельные координаты в мировые с учётом изменений размеров картинок
                sf::Vector2f Mouse_Cursor_Position = window.mapPixelToCoords(Pixel_Cursor_Position);
                window.draw(Playing_Menu_Background);

                Button_To_Play.setPosition(x, 500.f);
                Button_To_Play_Active.setPosition(x, 500.f);
            
                Button_To_Settings.setPosition(x, 770.f);
                Button_To_Settings_Active.setPosition(x, 770.f);

                Button_To_Quit.setPosition(x, 1040.f);
                Button_To_Quit_Active.setPosition(x, 1040.f);

                if (Button_To_Play.getGlobalBounds().contains(Mouse_Cursor_Position))
                    window.draw(Button_To_Play_Active);
                else
                    window.draw(Button_To_Play);

                if (Button_To_Settings.getGlobalBounds().contains(Mouse_Cursor_Position))
                    window.draw(Button_To_Settings_Active);
                else
                    window.draw(Button_To_Settings);

                if (Button_To_Quit.getGlobalBounds().contains(Mouse_Cursor_Position))
                    window.draw(Button_To_Quit_Active);
                else
                    window.draw(Button_To_Quit);

                break;

            }

//////////////////////////////////////////////////////////////////////////////////////////////////////

            // Состояние игры - игровой процесс
            case GameState::Playing:{
                window.setMouseCursorVisible(false);
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
            window.setMouseCursorVisible(false);
            case GameState::Settings:{
                window.setView(Default_View);
                window.draw(Settings_Background);
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