#include "stdafx.h"
#include "Sword.h"

Sword::Sword()
{
	this->weaponLevel = 0;

	//Weapon
	if (!weaponTexture.loadFromFile("./assets/pack/heroes/knight/weapons.png"))
		std::cout << "Couldn't load sword texture\n";

	this->weaponSprite.setTexture(weaponTexture);
	this->weaponSprite.setTextureRect(sf::IntRect(this->weaponLevel * 182, 0, 182, 430));
	this->weaponSprite.setOrigin(91.f, weaponSprite.getGlobalBounds().height + 30.f);
}

Sword::~Sword()
{
}

const sf::FloatRect Sword::getGlobalBounds()
{
	return this->weaponSprite.getGlobalBounds();
}

void Sword::upgrade()
{
	++this->weaponLevel;
	if(this->weaponLevel <= 12)
		this->weaponSprite.setTextureRect(sf::IntRect(this->weaponLevel * 182, 0, 182, 430));
}

void Sword::update(const sf::Vector2f& mouse_pos_view, const sf::Vector2f center, bool isOnColldown)
{
	this->weaponSprite.setScale(0.06f, 0.06f);
	this->weaponSprite.setPosition(center);
	float dx = mouse_pos_view.x - weaponSprite.getPosition().x;
	float dy = mouse_pos_view.y - weaponSprite.getPosition().y;

	const float PI = 3.14159f;
	float rotation = atan2(dy, dx) * 180 / PI;

	if (!isOnColldown) {
		//Attacking
		float len = static_cast<float>(std::sqrt(pow(dx, 2) + pow(dy, 2)));
		sf::Vector2f vec(dx / len, dy / len);
		this->weaponSprite.setPosition(center.x + vec.x * 7.f, center.y + vec.y * 7.f);
	}
	else {
		//Not attacking
		this->weaponSprite.setRotation(rotation + 90.f);
	}
}

void Sword::render(sf::RenderTarget& target)
{
	target.draw(this->weaponSprite);
}