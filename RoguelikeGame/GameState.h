#pragma once

#include "State.h"
#include "PauseMenu.h"
#include "EndScreenMenu.h"
#include "WinScreen.h"
#include "SettingsState.h"
#include "TileMap.h"
#include "EnemySpawner.h"
#include "Sword.h"

class PauseMenu;
class EndScreenMenu;
class WinScreen;
class Player;
class Enemy;
class Tilemap;

class GameState :
    public State
{
private:
    sf::Sprite background;
    sf::Texture backgroundTexture;

    sf::View view;
    sf::RenderTexture renderTexture;
    sf::Sprite renderSprite;

    sf::Font font;
    PauseMenu* pMenu;
    EndScreenMenu* endMenu;
    bool endMenuActive;
    WinScreen* winScreen;
    bool winMenuActive;
    bool continuing;

    Player* player;

    sf::Text allTimeBestText;

    unsigned int currentLevel;
    sf::Text currentLevelText;

    unsigned int score;
    sf::Text scoreText;
    bool scoreOutputted;

    unsigned int highScore;
    std::string bestName;

    EnemySpawner* spawner;
    sf::Clock clock;

    TileMap* tileMap;

    //Functions
    void initRender();
    void initBackground();
    void initView();
    void initKeybinds();
    void initFonts();
    void initText();
    void initTextures();
    void initPauseMenu();
    void initEndScreenMenu();
    void initWinScreen();
    void initPlayers();
    void initEnemies();
    void initTileMap();

public:
    GameState(StateData* state_data);
    virtual ~GameState();

    //Functions
    void textBoxTypedOn(sf::Event ev);
    void updateView(const float& dt);
    void updateInput(const float& dt);
    void updatePlayerInput(const float& dt);
    void updateText();
    void updatePMenuButtons();
    void updateEndMenuButtons();
    void updateWinScreenButtons();
    void updateTilemap(const float& dt);
    void updateSpawner(const float& dt);
    void updateEnemyCollision(const float& dt);
    void update(const float& dt);
    void render(sf::RenderTarget* target = nullptr);
};