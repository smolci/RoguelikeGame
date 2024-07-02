#include "stdafx.h"
#include "SettingsState.h"

// Initializers
void SettingsState::initVariables()
{
	number_of_buttons = 3;

	this->modes = sf::VideoMode::getFullscreenModes();
}

void SettingsState::initBackground()
{
	this->background.setSize(
		sf::Vector2f
		(static_cast<float>(this->window->getSize().x),
			static_cast<float>(this->window->getSize().y)
		)
	);
	if (!this->bgTexture.loadFromFile("./assets/background/settings.png"))
		std::cout << "Failed to load background texture!" << std::endl;
	this->background.setTexture(&this->bgTexture);
}

void SettingsState::initFonts()
{
	if (!this->font.loadFromFile("./fonts/square.ttf")) {
		std::cout << "Couldnt load font\n";
	}
}

void SettingsState::initKeybinds()
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

void SettingsState::initGui()
{
	this->buttons["BACK"] = new gui::Button(
		this->window->getSize().x - 200.f, this->window->getSize().y / (number_of_buttons + 1.f) * 3.f, 150.f, 50.f,
		&this->font, "back", 25,
		sf::Color(255, 255, 255), sf::Color(150, 150, 150), sf::Color(200, 200, 200),
		sf::Color(60, 70, 80, 0), sf::Color(60, 70, 80), sf::Color(40, 50, 60)
	);

	this->buttons["APPLY"] = new gui::Button(
		this->window->getSize().x - 400.f, this->window->getSize().y / (number_of_buttons + 1.f) * 3.f, 150.f, 50.f,
		&this->font, "apply", 25,
		sf::Color(255, 255, 255), sf::Color(150, 150, 150), sf::Color(200, 200, 200),
		sf::Color(60, 70, 80, 0), sf::Color(60, 70, 80), sf::Color(40, 50, 60)
	);
	
	this->checkbox = new gui::Checkbox(450.f, 550.f, 50.f, "assets/checkmark.png", this->stateData->graphSettings->fullscreen);

	std::vector<std::string> modes_str;
	for (auto& i : this->modes) {
		modes_str.push_back(std::to_string(i.width) + 'x' + std::to_string(i.height));
	}

	this->dropDownMenu["RESOLUTION"] = new gui::DropDownMenu(450.f, 400.f, 200.f, 50.f, font, modes_str.data(), static_cast<unsigned>(modes_str.size()), 0);
}

void SettingsState::initText()
{
	this->optionsText.setFont(this->font);

	this->optionsText.setPosition(sf::Vector2f(100.f, 412.f));
	
	this->optionsText.setCharacterSize(25);
	this->optionsText.setFillColor(sf::Color(255, 255, 255, 225));
	this->optionsText.setStyle(sf::Text::Bold);

	this->optionsText.setString("resolution \n\n\n\n\n\nfullscreen \n\n\n\n\n\n");
}

SettingsState::SettingsState(StateData* state_data)
	:State(state_data)
{
	this->initVariables();
	this->initBackground();
	this->initFonts();
	this->initKeybinds();
	this->initText();
	this->initGui();
}

SettingsState::~SettingsState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) {
		delete it->second;
	}

	auto it2 = this->dropDownMenu.begin();
	for (it2 = this->dropDownMenu.begin(); it2 != this->dropDownMenu.end(); ++it2) {
		delete it2->second;
	}
}

void SettingsState::updateInput(const float& dt)
{

}

void SettingsState::updateGui(const float& dt)
{
	// Buttons
	for (auto &i : this->buttons) {
		i.second->update(this->mousePosWindow);
	}

	this->buttons["BACK"]->setPosition(this->window->getSize().x - 200.f, this->window->getSize().y / (number_of_buttons + 1.f) * 3.f);
	this->buttons["APPLY"]->setPosition(this->window->getSize().x - 400.f, this->window->getSize().y / (number_of_buttons + 1.f) * 3.f);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getKeyTime()) {
		sf::Vector2f mousePos = static_cast<sf::Vector2f>(this->mousePosWindow);
		if (checkbox->isClicked(mousePos)) {
			checkbox->toggle();
		}
	}

	// Exit game
	if (this->buttons["BACK"]->isPressed()) {
		this->endState();
	}

	//Apply settings
	if (this->buttons["APPLY"]->isPressed()) {
		this->stateData->graphSettings->resolution = this->modes[this->dropDownMenu["RESOLUTION"]->getActiveElementId()];

		this->background.setSize(sf::Vector2f(static_cast<float>(this->stateData->graphSettings->resolution.width),
			static_cast<float>(this->stateData->graphSettings->resolution.height)));

		this->stateData->graphSettings->fullscreen = checkbox->isChecked();

		sf::VideoMode videoMode = this->stateData->graphSettings->resolution;
		sf::Vector2u size = this->stateData->graphSettings->fullscreen ? sf::Vector2u(videoMode.width, videoMode.height) : sf::Vector2u(800, 600);
		this->window->close();
		this->window->create(videoMode, this->stateData->graphSettings->title,
			this->stateData->graphSettings->fullscreen ? sf::Style::Fullscreen : sf::Style::Default);
	}

	// Drop down menu
	for (auto &i : this->dropDownMenu) {
		i.second->update(this->mousePosWindow, dt);
	}
}

void SettingsState::update(const float& dt)
{
	this->updateMousePos();
	this->updateInput(dt);
	this->updateKeyTime(dt);

	this->updateGui(dt);
}

void SettingsState::renderGui(sf::RenderTarget& target)
{
	for (auto &i : this->buttons) {
		i.second->render(target);
	}

	this->checkbox->render(target);

	for (auto &i : this->dropDownMenu) {
		i.second->render(target);
	}
}

void SettingsState::render(sf::RenderTarget* target)
{
	target->draw(this->background);

	this->renderGui(*target);

	target->draw(this->optionsText);
}