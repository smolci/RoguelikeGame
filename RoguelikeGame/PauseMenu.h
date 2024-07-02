#pragma once

#include "Gui.h"

class PauseMenu
{
private:
	sf::Font& font;
	sf::Text menuText;

	sf::RectangleShape background;
	sf::RectangleShape container;

	std::map<std::string, gui::Button*> buttons;

public:
	PauseMenu(sf::RenderWindow& window, sf::Font& font);
	virtual ~PauseMenu();

	// Functions
	const bool isButtonPressed(const std::string key);
	void addButton(const std::string key, const std::string text, unsigned int pos);

	void update(const sf::Vector2i& mousePosWindow, sf::RenderWindow& window);
	void render(sf::RenderTarget& target);
};
