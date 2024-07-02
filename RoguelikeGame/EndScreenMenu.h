#pragma once

#include "Gui.h"

class EndScreenMenu
{
private:
	sf::Font& font;
	sf::Text menuText1;
	sf::Text menuText2;
	bool nameGot;
	std::string name;

	sf::RectangleShape background;
	sf::RectangleShape container;
	
	gui::TextBox* textBox;

	std::map<std::string, gui::Button*> buttons;

	void initTextBox();

public:
	EndScreenMenu(sf::RenderWindow& window, sf::Font& font);
	virtual ~EndScreenMenu();

	// Functions
	void textBoxTypedOn(sf::Event ev);
	std::string getName();
	bool alreadyGotName();

	const bool isButtonPressed(const std::string key);
	void addButton(const std::string key, const std::string text, unsigned int pos);

	void update(const sf::Vector2i& mousePosWindow, sf::RenderWindow& window);
	void render(sf::RenderTarget& target);
};
