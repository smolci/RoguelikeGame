#pragma once

#include "MainMenuState.h"

class GraphicsSettings;
class StateData;
class MainMenuState;

class Game
{
private:
	//Variables
	GraphicsSettings graphSettings;
	StateData stateData;
	sf::RenderWindow *window;
	sf::Event event;

	sf::Clock dtClock;
	float dt;

	std::stack<State*> states;

	std::map<std::string, int> supportedKeys;

	float gridSize;

	//Initializers
	void initVariables();
	void initGraphicsSettings();
	void initWindow();
	void initKeys();
	void initStateData();
	void initStates();

public:
	//Constructor / Destructor
	Game();
	virtual ~Game();

	//Functions
	void endApplication();

	//Update
	void updateDeltaTime();
	void updateEvents();
	void update();

	//Render
	void render();
	void run();
};

