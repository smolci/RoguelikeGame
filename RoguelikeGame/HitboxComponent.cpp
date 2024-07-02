#include "stdafx.h"
#include "HitboxComponent.h"

HitboxComponent::HitboxComponent(sf::Sprite& sprite, 
	float offset_x, float offset_y, 
	float width, float height)
	: sprite(sprite), offsetX(offset_x), offsetY(offset_y)
{
	this->nextPos.left = 0.f;
	this->nextPos.top = 0.f;
	this->nextPos.width = width;
	this->nextPos.height = height;

	this->hitbox.setPosition(this->sprite.getPosition().x + offset_x, this->sprite.getPosition().y + offset_y);
	this->hitbox.setSize(sf::Vector2f(width, height));
	this->hitbox.setFillColor(sf::Color::Transparent);
	this->hitbox.setOutlineThickness(-1.f);
	this->hitbox.setOutlineColor(sf::Color::Red);
}

HitboxComponent::~HitboxComponent()
{

}

//Accessors
const sf::Vector2f& HitboxComponent::getPosition()
{
	return this->hitbox.getPosition();
}

const sf::FloatRect HitboxComponent::getGlobalBounds() const
{
	return this->hitbox.getGlobalBounds();
}

const sf::FloatRect& HitboxComponent::getNextPos(const sf::Vector2f& velocity)
{
	this->nextPos.left = this->hitbox.getPosition().x + velocity.x;
	this->nextPos.top = this->hitbox.getPosition().y + velocity.y;
	return this->nextPos;
}

//Modifiers
void HitboxComponent::setPosition(sf::Vector2f& position)
{
	this->hitbox.setPosition(position);
	this->sprite.setPosition(position.x - this->offsetX, position.y - this->offsetY);
}

void HitboxComponent::setPosition(const float x, const float y)
{
	this->hitbox.setPosition(x, y);
	this->sprite.setPosition(x - this->offsetX, y - this->offsetY);
}

bool HitboxComponent::intersects(const sf::FloatRect& rect)
{
	return this->hitbox.getGlobalBounds().intersects(rect);
}

//Functions
void HitboxComponent::update()
{
	this->hitbox.setPosition(this->sprite.getPosition().x + this->offsetX, this->sprite.getPosition().y + this->offsetY);
}

void HitboxComponent::render(sf::RenderTarget& target)
{
	target.draw(this->hitbox);
}