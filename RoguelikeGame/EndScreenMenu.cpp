#include "stdafx.h"
#include "EndScreenMenu.h"

void EndScreenMenu::initTextBox()
{
	this->textBox = new gui::TextBox(25, sf::Color::White, true);
	this->textBox->setFont(this->font);
	this->textBox->setPosition(
		sf::Vector2f(
			this->container.getPosition().x + this->container.getSize().x / 2.f,
			this->container.getPosition().y + this->container.getSize().y / 5 * 2
		)
	);
	this->textBox->setLimit(true, 12);
}

EndScreenMenu::EndScreenMenu(sf::RenderWindow& window, sf::Font& font)
	:font(font)
{
	this->initTextBox();
	// Background
	this->background.setSize(sf::Vector2f(
		static_cast<float>(window.getSize().x),
		static_cast<float>(window.getSize().y)
	));
	this->background.setFillColor(sf::Color(20, 20, 20, 200));

	// Container
	this->container.setSize(sf::Vector2f(
		static_cast<float>(window.getSize().x) / 2.f + 200.f,
		static_cast<float>(window.getSize().y) / 2.f
	));
	this->container.setPosition(sf::Vector2f(
		static_cast<float>(window.getSize().x) / 2.f - this->container.getSize().x / 2.f,
		static_cast<float>(window.getSize().y) / 5.f * 1
	));
	this->container.setFillColor(sf::Color(20, 20, 20, 100));

	// Text
	this->menuText1.setFont(font);
	this->menuText1.setFillColor(sf::Color(255, 255, 255));
	this->menuText1.setCharacterSize(40);
	this->menuText1.setString("you died!");
	this->menuText1.setPosition(this->container.getPosition().x + this->container.getSize().x / 2.f - this->menuText1.getGlobalBounds().width / 2.f, this->container.getPosition().y + 30.f);

	this->menuText2.setFont(font);
	this->menuText2.setFillColor(sf::Color(255, 255, 255));
	this->menuText2.setCharacterSize(40);
	this->menuText2.setString("enter your name");
	this->menuText2.setPosition(this->container.getPosition().x + this->container.getSize().x / 2.f - this->menuText2.getGlobalBounds().width / 2.f, this->container.getPosition().y + this->container.getSize().y / 5 * 1);

	this->nameGot = false;
}

EndScreenMenu::~EndScreenMenu()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) {
		delete it->second;
	}
}

void EndScreenMenu::textBoxTypedOn(sf::Event ev)
{
	this->textBox->typedOn(ev);
}

std::string EndScreenMenu::getName()
{
	return this->textBox->getText();
}

bool EndScreenMenu::alreadyGotName()
{
	return this->nameGot;
}

const bool EndScreenMenu::isButtonPressed(const std::string key)
{
	return this->buttons[key]->isPressed();
}

// Functions
void EndScreenMenu::addButton(const std::string key, const std::string text, unsigned int pos)
{
	float width = 250.f;
	float height = 50.f;
	float x = this->container.getPosition().x + this->container.getSize().x / 2.f - width / 2.f;
	float y = this->container.getPosition().y + this->container.getSize().y / 5 * pos;

	this->buttons[key] = new gui::Button(
		x, y, width, height,
		&this->font, text, 30,
		sf::Color(255, 255, 255), sf::Color(150, 150, 150), sf::Color(200, 200, 200),
		sf::Color(30, 30, 150, 0), sf::Color(60, 60, 200, 0), sf::Color(90, 90, 240, 0)
	);
}

void EndScreenMenu::update(const sf::Vector2i& mousePosWindow, sf::RenderWindow& window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && !this->nameGot) {
		std::cout << this->textBox->getText();
		this->name = this->textBox->getText();
		this->nameGot = true;
		this->textBox->setSelected(false);
	}

	this->container.setPosition(sf::Vector2f(
		static_cast<float>(window.getSize().x) / 2.f - this->container.getSize().x / 2.f,
		static_cast<float>(window.getSize().y) / 5.f * 1
	));
	this->menuText1.setPosition(this->container.getPosition().x + this->container.getSize().x / 2.f - this->menuText1.getGlobalBounds().width / 2.f, this->container.getPosition().y + 30.f);
	this->menuText2.setPosition(this->container.getPosition().x + this->container.getSize().x / 2.f - this->menuText2.getGlobalBounds().width / 2.f, this->container.getPosition().y + this->container.getSize().y / 5 * 1);

	this->textBox->setPosition(
		sf::Vector2f(
			this->container.getPosition().x + this->container.getSize().x / 2.f - 150.f,
			this->container.getPosition().y + this->container.getSize().y / 5 * 2
		)
	);

	for (auto& i : this->buttons) {
		i.second->update(mousePosWindow);
	}
}

void EndScreenMenu::render(sf::RenderTarget& target)
{
	target.draw(this->background);
	target.draw(this->container);
	
	for (auto& i : this->buttons) {
		i.second->render(target);
	}

	target.draw(this->menuText1);
	target.draw(this->menuText2);

	this->textBox->render(target);
}