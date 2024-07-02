#pragma once

#include "Player.h"
#include "Enemy.h"
#include "GraphicsSettings.h"

class Player;
class GraphicsSettings;
class State;

class StateData
{
public:
	StateData() {};

	//Variables
	float gridSize;
	sf::RenderWindow* window;
	GraphicsSettings* graphSettings;
	std::map<std::string, int>* supportedKeys;
	std::stack<State*>* states;
};

class State
{
private:

protected:
	StateData* stateData;
	std::stack<State*>* states;
	sf::RenderWindow* window;
	std::map<std::string, int>* supportedKeys;
	std::map<std::string, int> keybinds;
	bool end;
	bool paused;
	float keyTime;
	float maxKeyTime;
	float gridSize;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;
	sf::Vector2i mousePosGrid;

	//Resources
	std::map<std::string, sf::Texture> texture;

	//Functions
	virtual void initKeybinds() = 0;

public:
	State(StateData* state_data);
	virtual ~State();

	//Accessors
	const bool& getEnd() const;
	const bool getKeyTime();


	// Functions
	void endState();
	void pauseState();
	void unpauseState();

	virtual void textBoxTypedOn(sf::Event ev);

	virtual void updateMousePos(sf::View* view = NULL);
	virtual void updateKeyTime(const float& dt);
	virtual void updateInput(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;
};

