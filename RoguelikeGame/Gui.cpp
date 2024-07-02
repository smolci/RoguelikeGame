#include "stdafx.h"
#include "Gui.h"

gui::Button::Button(
	float x,
	float y,
	float width,
	float height,
	sf::Font* font,
	std::string text,
	unsigned int character_size,
	sf::Color text_idle_color,
	sf::Color text_hover_color,
	sf::Color text_active_color,
	sf::Color idle_color,
	sf::Color hover_color,
	sf::Color active_color,
	sf::Color outline_idle_color,
	sf::Color outline_hover_color,
	sf::Color outline_active_color,
	short unsigned id
){
	this->buttonState = BTN_IDLE;
	this->id = id;
	
	this->shape.setSize(sf::Vector2f(width, height));
	this->shape.setPosition(sf::Vector2f(x, y));
	this->shape.setFillColor(idle_color);
	this->shape.setOutlineThickness(1.f);
	this->shape.setOutlineColor(outline_idle_color);
	
	this->font = font;
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setFillColor(text_idle_color);
	this->text.setCharacterSize(character_size);
	this->text.setStyle(sf::Text::Bold);
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 2.f
	);

	this->textIdleColor = text_idle_color;
	this->textHoverColor = text_hover_color;
	this->textActiveColor = text_active_color;

	this->idleColor = idle_color;
	this->hoverColor = hover_color;
	this->activeColor = active_color;

	this->outlineIdleColor = outline_idle_color;
	this->outlineHoverColor = outline_hover_color;
	this->outlineActiveColor = outline_active_color;
}

gui::Button::~Button()
{

}

//Accessors
const bool gui::Button::isPressed() const
{
	if (this->buttonState == BTN_ACTIVE) return true;
	return false;
}

const std::string gui::Button::getText() const
{
	return this->text.getString();
}

const short unsigned& gui::Button::getId() const
{
	return this->id;
}

//Modifiers
void gui::Button::setText(const std::string text)
{
	this->text.setString(text);
}

void gui::Button::setId(const short unsigned id)
{
	this->id = id;
}

void gui::Button::setPosition(const float x, const float y)
{
	this->shape.setPosition(sf::Vector2f(x, y));
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 2.f
	);
}

//Functions
void gui::Button::update(const sf::Vector2i& mousePosWindow)
{
	//Reset it to idle
	this->buttonState = BTN_IDLE;

	//Hover
	if (this->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) {
		this->buttonState = BTN_HOVER;

		//Button pressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			this->buttonState = BTN_ACTIVE;
		}
	}

	switch (this->buttonState)
	{
	//Hover
	case BTN_HOVER:
		this->shape.setFillColor(this->hoverColor);
		this->text.setFillColor(this->textHoverColor);
		this->shape.setOutlineColor(this->outlineHoverColor);
		break;
	//Pressed
	case BTN_ACTIVE:
		this->shape.setFillColor(this->activeColor);
		this->text.setFillColor(this->textActiveColor);
		this->shape.setOutlineColor(this->outlineActiveColor);
		break;
	//Idle
	default:
		this->shape.setFillColor(this->idleColor);
		this->text.setFillColor(this->textIdleColor);
		this->shape.setOutlineColor(this->outlineIdleColor);
		break;
	}
}

void gui::Button::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
	target.draw(this->text);
}

// -------------------- CHECKBOX --------------------
gui::Checkbox::Checkbox(float x, float y, float size, const std::string& texturePath, bool fullscreen)
{
	if (!checkmarkTexture.loadFromFile(texturePath)) {
		std::cout << "Couldn't load checkbox texture\n";
	}

	checkmark.setTexture(checkmarkTexture);
	checkmark.setTextureRect(sf::IntRect(checkmarkTexture.getSize().x / 2, 0, checkmarkTexture.getSize().x / 2, checkmarkTexture.getSize().y));
	checkmark.setScale(size / checkmarkTexture.getSize().x * 2, size / checkmarkTexture.getSize().y);
	checkmark.setPosition(x, y);
	checked = fullscreen;
	if (checked)
		checkmark.setTextureRect(sf::IntRect(0, 0, checkmarkTexture.getSize().x / 2, checkmarkTexture.getSize().y));
	else
		checkmark.setTextureRect(sf::IntRect(checkmarkTexture.getSize().x / 2, 0, checkmarkTexture.getSize().x / 2, checkmarkTexture.getSize().y));
}

bool gui::Checkbox::isClicked(const sf::Vector2f& mousePos)
{
	return checkmark.getGlobalBounds().contains(mousePos);
}

void gui::Checkbox::toggle()
{
	checked = !checked;
	if (checked)
		checkmark.setTextureRect(sf::IntRect(0, 0, checkmarkTexture.getSize().x / 2, checkmarkTexture.getSize().y));
	else
		checkmark.setTextureRect(sf::IntRect(checkmarkTexture.getSize().x / 2, 0, checkmarkTexture.getSize().x / 2, checkmarkTexture.getSize().y));
}

bool gui::Checkbox::isChecked() const
{
	return checked;
}

void gui::Checkbox::render(sf::RenderTarget& target)
{
	target.draw(checkmark);
}

// -------------------- DROP DOWN MENU --------------------
gui::DropDownMenu::DropDownMenu(float x, float y, float width, float height,
	sf::Font& font, std::string list[], unsigned numOfEle, const unsigned default_index)
	: x(x), y(y), width(width), height(height), font(font), showMenu(false), maxKeyTime(2.5f), keyTime(0.f)
{
	this->active = new gui::Button(
		x, y, width, height,
		&this->font, list[default_index], 15,
		sf::Color(255, 255, 255), sf::Color(150, 150, 150), sf::Color(200, 200, 200),
		sf::Color(60, 70, 80, 0), sf::Color(60, 70, 80), sf::Color(40, 50, 60),
		sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200)
	);

	for (unsigned i = 0; i < numOfEle; i++)
	{
		this->menu.push_back(
			new gui::Button(
				x, y + (i + 1) * height, width, height,
				&this->font, list[i], 15,
				sf::Color(255, 255, 255), sf::Color(150, 150, 150), sf::Color(200, 200, 200),
				sf::Color(60, 70, 80, 0), sf::Color(60, 70, 80), sf::Color(40, 50, 60),
				sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0),
				i
			)
		);
	}
}

gui::DropDownMenu::~DropDownMenu()
{
	delete this->active;
	for (size_t i = 0; i < this->menu.size(); i++)
	{
		delete this->menu[i];
	}
}

//Accessors
const unsigned short& gui::DropDownMenu::getActiveElementId() const
{
	return this->active->getId();
}

const bool gui::DropDownMenu::getKeyTime()
{
	if (this->keyTime >= this->maxKeyTime) {
		this->keyTime = 0.f;
		return true;
	}
	return false;
}

//Functions
void gui::DropDownMenu::updateKeyTime(const float& dt)
{
	if (this->keyTime < this->maxKeyTime)
		this->keyTime += 10.f * dt;
}

void gui::DropDownMenu::update(const sf::Vector2i& mousePosWindow, const float& dt)
{
	this->updateKeyTime(dt);
	this->active->update(mousePosWindow);

	if (this->active->isPressed() && this->getKeyTime()) {
		if (this->showMenu)
			this->showMenu = false;
		else
			this->showMenu = true;

		for (size_t i = 0; i < this->menu.size(); i++)
		{
			this->menu[i]->setPosition(x, y + (i + 1) * height);
		}
	}

	if (this->showMenu) {
		for (auto &i : this->menu) {
			i->update(mousePosWindow);

			if (i->isPressed() && this->getKeyTime()) {
				this->showMenu = false;
				this->active->setText(i->getText());
				this->active->setId(i->getId());
			}
		}
	}	
}

void gui::DropDownMenu::render(sf::RenderTarget& target)
{
	this->active->render(target);
	if (this->showMenu) {
		for (auto &i : this->menu) {
			i->render(target);
		}
	}
}


//------------------- Texture selector -----------------
//Constructor
gui::TextureSelector::TextureSelector(float x, float y, float width, float height, float gridSize, const sf::Texture* texture_sheet, sf::Font& font, std::string text)
	:maxKeyTime(2.5f), keyTime(0.f)
{
	this->gridSize = gridSize;
	this->active = false;
	this->hidden = false;
	float offset = 120.f;

	this->bounds.setSize(sf::Vector2f(width, height));
	this->bounds.setPosition(x + offset, y);
	this->bounds.setFillColor(sf::Color(50, 50, 50, 100));
	this->bounds.setOutlineThickness(1.f);
	this->bounds.setOutlineColor(sf::Color(255, 255, 255, 200));

	this->sheet.setTexture(*texture_sheet);
	this->sheet.setPosition(x + offset, y);

	if (this->sheet.getGlobalBounds().width > this->bounds.getGlobalBounds().width) {
		this->sheet.setTextureRect(sf::IntRect(0, 0, static_cast<int>(this->bounds.getGlobalBounds().width), static_cast<int>(this->bounds.getGlobalBounds().height)));
	}

	if (this->sheet.getGlobalBounds().height > this->bounds.getGlobalBounds().height) {
		this->sheet.setTextureRect(sf::IntRect(0, 0, static_cast<int>(this->bounds.getGlobalBounds().width), static_cast<int>(this->bounds.getGlobalBounds().height)));
	}

	
	this->selector.setPosition(x, y);
	this->selector.setSize(sf::Vector2f(gridSize, gridSize));
	this->selector.setFillColor(sf::Color::Transparent);
	this->selector.setOutlineThickness(1.f);
	this->selector.setOutlineColor(sf::Color::Green);

	this->textureRect.width = static_cast<int>(gridSize);
	this->textureRect.height = static_cast<int>(gridSize);

	this->hideTexSel = new gui::Button(
		0, 0, 100, 100,
		&font, text, 20,
		sf::Color(255, 255, 255), sf::Color(150, 150, 150), sf::Color(200, 200, 200),
		sf::Color(70, 70, 70, 200), sf::Color(50, 50, 50, 250), sf::Color(90, 90, 90, 50)
	);;
}

//Destructor
gui::TextureSelector::~TextureSelector()
{
	delete this->hideTexSel;
}

//Accessors
const bool& gui::TextureSelector::getActive() const
{
	return this->active;
}

const sf::IntRect& gui::TextureSelector::getTextureRect() const
{
	return this->textureRect;
}

//Functions
const bool gui::TextureSelector::getKeyTime()
{
	if (this->keyTime >= this->maxKeyTime) {
		this->keyTime = 0.f;
		return true;
	}
	return false;
}

void gui::TextureSelector::updateKeyTime(const float& dt)
{
	if (this->keyTime < this->maxKeyTime)
		this->keyTime += 10.f * dt;
}

void gui::TextureSelector::update(const sf::Vector2i& mousePosWindow, const float& dt)
{
	this->updateKeyTime(dt);
	this->hideTexSel->update(mousePosWindow);

	if (this->hideTexSel->isPressed() && this->getKeyTime()) {
		if (this->hidden)
			this->hidden = false;
		else
			this->hidden = true;
	}

	if (!this->hidden) {
		if (this->bounds.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
			this->active = true;
		else
			this->active = false;

		if (this->active) {
			this->mousePosGrid.x = (mousePosWindow.x - static_cast<int>(this->bounds.getPosition().x)) / static_cast<unsigned>(this->gridSize);
			this->mousePosGrid.y = (mousePosWindow.y - static_cast<int>(this->bounds.getPosition().y)) / static_cast<unsigned>(this->gridSize);
			this->selector.setPosition(
				this->bounds.getPosition().x + this->mousePosGrid.x * this->gridSize,
				this->bounds.getPosition().y + this->mousePosGrid.y * this->gridSize
			);

			//Update texture rectangle
			this->textureRect.left = static_cast<int>(this->selector.getPosition().x - this->bounds.getPosition().x);
			this->textureRect.top = static_cast<int>(this->selector.getPosition().y - this->bounds.getPosition().y);
		}
	}
}

void gui::TextureSelector::render(sf::RenderTarget& target)
{
	if (!this->hidden) {
		target.draw(this->bounds);
		target.draw(this->sheet);

		if (this->active)
			target.draw(this->selector);
	}

	this->hideTexSel->render(target);
}

//------------------- Health bar -----------------
gui::HealthBar::HealthBar(float width, float height, const sf::Color& backgroundColor, const sf::Color& barColor, const sf::Vector2f& position)
	:position(position), maxWidth(width)
{
	background.setSize(sf::Vector2f(width, height));
	background.setFillColor(backgroundColor);
	background.setPosition(position);

	bar.setSize(sf::Vector2f(width, height));
	bar.setFillColor(barColor);
	bar.setPosition(position);
}

gui::HealthBar::~HealthBar()
{
}

void gui::HealthBar::setPercantage(float percentage)
{
	float width = maxWidth * percentage;
	bar.setSize(sf::Vector2f(width, bar.getSize().y));
}

void gui::HealthBar::setPosition(const sf::Vector2f& pos)
{
	this->position = pos;
	background.setPosition(position);
	bar.setPosition(position);
}

void gui::HealthBar::render(sf::RenderTarget& target)
{
	target.draw(background);
	target.draw(bar);
}

//------------------- TextBox -----------------
void gui::TextBox::inputLogic(int charTyped)
{
	if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
		this->text << static_cast<char>(charTyped);
	}
	else if (charTyped == DELETE_KEY) {
		if (this->text.str().length() > 0) {
			this->deleteLastChar();
		}
	}
	this->textBox.setString(text.str() + "_");
}

void gui::TextBox::deleteLastChar()
{
	std::string t = text.str();
	std::string newT = "";
	for (int i = 0; i < t.length() - 1; i++) {
		newT += t[i];
	}
	this->text.str("");
	this->text << newT;

	this->textBox.setString(text.str());
}

gui::TextBox::TextBox()
{

}

gui::TextBox::TextBox(int size, sf::Color color, bool selected)
{
	this->textBox.setCharacterSize(size);
	this->textBox.setFillColor(color);
	this->isSelected = selected;
	if (selected) {
		this->textBox.setString("_");
	}
	else {
		this->textBox.setString("");
	}
}

void gui::TextBox::setFont(sf::Font& font)
{
	this->textBox.setFont(font);
}

void gui::TextBox::setPosition(sf::Vector2f pos)
{
	this->textBox.setPosition(pos);
}

void gui::TextBox::setLimit(bool value)
{
	this->hasLimit = value;
}

void gui::TextBox::setLimit(bool value, int limit)
{
	this->hasLimit = value;
	this->limit = limit - 1;
}

void gui::TextBox::setSelected(bool sel)
{
	this->isSelected = sel;
	if (!sel) {
		std::string t = text.str();
		std::string newT = "";
		for (int i = 0; i < t.length(); i++) {
			newT += t[i];
		}
		this->textBox.setString(newT);
	}
}

std::string gui::TextBox::getText()
{
	return this->text.str();
}

void gui::TextBox::clearText()
{
	this->text.str("");
}

void gui::TextBox::typedOn(sf::Event input)
{
	if (this->isSelected) {
		int charTyped = input.text.unicode;
		if (charTyped < 128) {
			if (this->hasLimit) {
				if (this->text.str().length() <= this->limit) {
					this->inputLogic(charTyped);
				}
				else if (this->text.str().length() > this->limit && charTyped == DELETE_KEY) {
					this->deleteLastChar();
				}
			}
			else {
				this->inputLogic(charTyped);
			}
		}
	}
}

void gui::TextBox::render(sf::RenderTarget& target)
{
	target.draw(this->textBox);
}
