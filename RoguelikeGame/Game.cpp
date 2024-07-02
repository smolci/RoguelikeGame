#include "stdafx.h"
#include "Game.h"

//Init. functions
void Game::initVariables()
{
	this->window = nullptr;
	this->dt = 0.f;
	this->gridSize = 16.f;
}

void Game::initGraphicsSettings()
{
	this->graphSettings.loadFromFile("config/graphics.ini");
}

void Game::initWindow()
{
	if (this->graphSettings.fullscreen) {
		this->window = new sf::RenderWindow(
			this->graphSettings.resolution, 
			this->graphSettings.title, 
			sf::Style::Fullscreen, 
			this->graphSettings.contextSettings);
	}
	else {
		this->window = new sf::RenderWindow(
			this->graphSettings.resolution,
			this->graphSettings.title,
			sf::Style::Titlebar | sf::Style::Close,
			this->graphSettings.contextSettings);
	}
	this->window->setFramerateLimit(this->graphSettings.frameRateLimit);
}

void Game::initKeys()
{
	std::ifstream ifs("./config/supported_keys.ini");

	if (ifs.is_open()) {
		std::string key = "";
		int value = 0;
		while (ifs >> key >> value) {
			this->supportedKeys[key] = value;
		}
	}

	ifs.close();

	//DELETE
	for (auto i : this->supportedKeys) {
		std::cout << i.first << " " << i.second << "\n";
	}
}

void Game::initStateData()
{
	this->stateData.window = this->window;
	this->stateData.graphSettings = &this->graphSettings;
	this->stateData.supportedKeys = &this->supportedKeys;
	this->stateData.states = &this->states;
	this->stateData.gridSize = this->gridSize;
}

void Game::initStates()
{
	this->states.push(new MainMenuState(&this->stateData));
}

//Constructor / Destructor
Game::Game() 
{
	this->initVariables();
	this->initGraphicsSettings();
	this->initWindow();
	this->initKeys();
	this->initStateData();
	this->initStates();
}

Game::~Game() 
{
	delete this->window;
	while (!this->states.empty()) {
		delete this->states.top();
		this->states.pop();
	}
}

//Functions
void Game::endApplication()
{
	std::cout << "Ending application\n";
}

//Update
void Game::updateDeltaTime()
{
	this->dt = this->dtClock.restart().asSeconds();
}

void Game::updateEvents()
{
	while (this->window->pollEvent(this->event)) {
		switch (this->event.type) {
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::TextEntered:
			this->states.top()->textBoxTypedOn(this->event);
			break;
		}
	}
}

void Game::update()
{
	this->updateEvents();

	if (!this->states.empty()) {
		if (this->window->hasFocus()) {
			this->states.top()->update(this->dt);
			if (this->states.top()->getEnd()) {
				this->states.top()->endState();
				delete this->states.top();
				this->states.pop();
			}
		}
	}
	// Application end
	else {
		this->endApplication();
		this->window->close();
	}
}

void Game::render()
{
	this->window->clear();

	//Render
	if (!this->states.empty())
		this->states.top()->render(this->window);

	this->window->display();
}

void Game::run()
{
	while (this->window->isOpen()) {
		this->updateDeltaTime();
		this->update();
		this->render();
	}
}