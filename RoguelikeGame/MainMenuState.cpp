#include "stdafx.h"
#include "MainMenuState.h"

// Initializers
void MainMenuState::initVariables()
{
	number_of_buttons = 4;
}

void MainMenuState::initBackground()
{
	this->background.setSize(
		sf::Vector2f
		(static_cast<float>(this->window->getSize().x), 
			static_cast<float>(this->window->getSize().y)
		)
	);
	if (!this->bgTexture.loadFromFile("./assets/background/mainmenu.png"))
		std::cout << "Failed to load background texture!" << std::endl;
	this->background.setTexture(&this->bgTexture);
}

void MainMenuState::initFonts()
{
	if (!this->font.loadFromFile("./fonts/square.ttf")) {
		std::cout << "Couldnt load font\n";
	}
}

void MainMenuState::initKeybinds()
{
	std::ifstream ifs("./config/mainmenustate_keybinds.ini");

	if (ifs.is_open()) {
		std::string key = "";
		std::string actualKey = "";
		while (ifs >> key >> actualKey) {
			this->keybinds[key] = this->supportedKeys->at(actualKey);
		}
	}

	ifs.close();
}

void MainMenuState::initButtons()
{
	this->buttons["GAME_STATE"] = new gui::Button(
		this->window->getSize().x / 5 - 150.f, this->window->getSize().y / (number_of_buttons + 4.f) * 3.f, 300.f, 50.f,
		&this->font, "start game", 25,
		sf::Color(255, 255, 255), sf::Color(150, 150, 150), sf::Color(200, 200, 200),
		sf::Color(60, 70, 80, 0), sf::Color(60, 70, 80), sf::Color(40, 50, 60)
	);

	this->buttons["SETTINGS_STATE"] = new gui::Button(
		this->window->getSize().x / 5 - 150.f, this->window->getSize().y / (number_of_buttons + 4.f) * 4.f, 300.f, 50.f,
		&this->font, "settings", 25,
		sf::Color(255, 255, 255), sf::Color(150, 150, 150), sf::Color(200, 200, 200),
		sf::Color(60, 70, 80, 0), sf::Color(60, 70, 80), sf::Color(40, 50, 60)
	);

	this->buttons["EDITOR_STATE"] = new gui::Button(
		this->window->getSize().x / 5 - 150.f, this->window->getSize().y / (number_of_buttons + 4.f) * 5.f, 300.f, 50.f,
		&this->font, "map editor", 25,
		sf::Color(255, 255, 255), sf::Color(150, 150, 150), sf::Color(200, 200, 200),
		sf::Color(60, 70, 80, 0), sf::Color(60, 70, 80), sf::Color(40, 50, 60)
	);

	this->buttons["EXIT_STATE"] = new gui::Button(
		this->window->getSize().x / 5 - 150.f, this->window->getSize().y / (number_of_buttons + 4.f) * 6.f, 300.f, 50.f,
		&this->font, "exit", 25,
		sf::Color(255, 255, 255), sf::Color(150, 150, 150), sf::Color(200, 200, 200),
		sf::Color(60, 70, 80, 0), sf::Color(60, 70, 80), sf::Color(40, 50, 60)
	);
}

MainMenuState::MainMenuState(StateData* state_data)
	: State(state_data)
{
	this->initVariables();
	this->initBackground();
	this->initFonts();
	this->initKeybinds();
	this->initButtons();
}

MainMenuState::~MainMenuState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) {
		delete it->second;
	}
}

void MainMenuState::updateInput(const float& dt)
{
	
}

void MainMenuState::updateButtons()
{
	for (auto i : this->buttons) {
		i.second->update(this->mousePosWindow);
	}

	this->buttons["GAME_STATE"]->setPosition(this->window->getSize().x / 5.f - 150.f, this->window->getSize().y / (number_of_buttons + 4.f) * 3.f);
	this->buttons["SETTINGS_STATE"]->setPosition(this->window->getSize().x / 5.f - 150.f, this->window->getSize().y / (number_of_buttons + 4.f) * 4.f);
	this->buttons["EDITOR_STATE"]->setPosition(this->window->getSize().x / 5.f - 150.f, this->window->getSize().y / (number_of_buttons + 4.f) * 5.f);
	this->buttons["EXIT_STATE"]->setPosition(this->window->getSize().x / 5.f - 150.f, this->window->getSize().y / (number_of_buttons + 4.f) * 6.f);

	// Start game
	if (this->buttons["GAME_STATE"]->isPressed()) {
		this->states->push(new GameState(this->stateData));
	}

	// Open settings
	if (this->buttons["SETTINGS_STATE"]->isPressed()) {
		this->states->push(new SettingsState(this->stateData));
	}

	if (this->buttons["EDITOR_STATE"]->isPressed()) {
		this->states->push(new EditorState(this->stateData));
	}

	// Exit game
	if (this->buttons["EXIT_STATE"]->isPressed()) {
		this->endState();
	}
}

void MainMenuState::update(const float& dt)
{
	this->updateMousePos();
	this->updateInput(dt);

	this->updateButtons();
}

void MainMenuState::renderButtons(sf::RenderTarget& target)
{
	for (auto i : this->buttons) {
		i.second->render(target);
	}
}

void MainMenuState::render(sf::RenderTarget* target)
{
	this->background.setSize(sf::Vector2f(static_cast<float>(this->stateData->graphSettings->resolution.width), static_cast<float>(this->stateData->graphSettings->resolution.height)));
	target->draw(this->background);
	this->renderButtons(*target);

	/*sf::Text mouseText;
	mouseText.setPosition(this->mousePosView.x, this->mousePosView.y - 50);
	mouseText.setFont(this->font);
	mouseText.setCharacterSize(12);
	std::stringstream ss;
	ss << this->mousePosView.x << " " << this->mousePosView.y;
	mouseText.setString(ss.str());
	target->draw(mouseText);*/
}