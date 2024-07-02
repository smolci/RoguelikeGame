#include "stdafx.h"
#include "WinScreen.h"

WinScreen::WinScreen(sf::RenderWindow& window, sf::Font& font)
	:font(font)
{
	// Background
	this->background.setSize(sf::Vector2f(
		static_cast<float>(window.getSize().x),
		static_cast<float>(window.getSize().y)
	));
	this->background.setFillColor(sf::Color(20, 20, 20, 200));

	// Container
	this->container.setSize(sf::Vector2f(
		static_cast<float>(window.getSize().x) / 2.f,
		static_cast<float>(window.getSize().y) / 2.f
	));
	this->container.setPosition(sf::Vector2f(
		static_cast<float>(window.getSize().x) / 2.f - this->container.getSize().x / 2.f,
		static_cast<float>(window.getSize().y) / 5.f * 1
	));
	this->container.setFillColor(sf::Color(20, 20, 20, 100));

	// Text
	this->menuText.setFont(font);
	this->menuText.setFillColor(sf::Color(255, 255, 255));
	this->menuText.setCharacterSize(40);
	this->menuText.setString("you win!");
	this->menuText.setPosition(this->container.getPosition().x + this->container.getSize().x / 2.f - this->menuText.getGlobalBounds().width / 2.f, this->container.getPosition().y + 30.f);
}

WinScreen::~WinScreen()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) {
		delete it->second;
	}
}

const bool WinScreen::isButtonPressed(const std::string key)
{
	return this->buttons[key]->isPressed();
}

// Functions
void WinScreen::addButton(const std::string key, const std::string text, unsigned int pos)
{
	float width = 250.f;
	float height = 50.f;
	float x = this->container.getPosition().x + this->container.getSize().x / 2.f - width / 2.f;
	float y = this->container.getPosition().y + this->container.getSize().y / 3 * pos;

	this->buttons[key] = new gui::Button(
		x, y, width, height,
		&this->font, text, 30,
		sf::Color(255, 255, 255), sf::Color(150, 150, 150), sf::Color(200, 200, 200),
		sf::Color(30, 30, 150, 0), sf::Color(60, 60, 200, 0), sf::Color(90, 90, 240, 0)
	);
}

void WinScreen::update(const sf::Vector2i& mousePosWindow, sf::RenderWindow& window)
{

	this->container.setPosition(sf::Vector2f(
		static_cast<float>(window.getSize().x) / 2.f - this->container.getSize().x / 2.f,
		static_cast<float>(window.getSize().y) / 5.f * 1
	));
	this->menuText.setPosition(this->container.getPosition().x + this->container.getSize().x / 2.f - this->menuText.getGlobalBounds().width / 2.f, this->container.getPosition().y + 30.f);

	for (auto& i : this->buttons) {
		i.second->update(mousePosWindow);
	}
}

void WinScreen::render(sf::RenderTarget& target)
{
	target.draw(this->background);
	target.draw(this->container);

	for (auto& i : this->buttons) {
		i.second->render(target);
	}

	target.draw(this->menuText);
}