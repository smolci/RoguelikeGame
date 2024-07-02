#include "stdafx.h"
#include "Entity.h"

void Entity::initVariables()
{
	this->movementComponent = nullptr;
	this->animationComponent = nullptr;
	this->hitboxComponent = nullptr;
}

Entity::Entity()
{
	this->initVariables();
}

Entity::~Entity()
{
	delete this->movementComponent;
	delete this->animationComponent;
	delete this->hitboxComponent;
}

//Components
void Entity::setTexture(sf::Texture& texture)
{
	this->sprite.setTexture(texture);
}

void Entity::createMovComp(const float maxVelocity, const float acceleration, const float deceleration)
{
	this->movementComponent = new MovementComponent(this->sprite, maxVelocity, acceleration, deceleration);
}

void Entity::createAnimComp(sf::Texture& textureSheet)
{
	this->animationComponent = new AnimationComponent(this->sprite, textureSheet);
}

void Entity::createHitboxComp(sf::Sprite& sprite,
	float offset_x, float offset_y,
	float width, float height
){
	this->hitboxComponent = new HitboxComponent(sprite, offset_x, offset_y, width, height);
}

const sf::Vector2f& Entity::getPosition()
{
	if (this->hitboxComponent)
		return this->hitboxComponent->getPosition();
	return this->sprite.getPosition();
}

const sf::Vector2f Entity::getCenter() const
{
	if (this->hitboxComponent)
		return this->hitboxComponent->getPosition() +
			sf::Vector2f
				(
					this->hitboxComponent->getGlobalBounds().width / 2.f,
					this->hitboxComponent->getGlobalBounds().height / 2.f
				);
	else {
		return this->sprite.getPosition() +
			sf::Vector2f
				(
					this->sprite.getGlobalBounds().width / 2.f,
					this->sprite.getGlobalBounds().height / 2.f
				);
	}
}

const sf::Vector2i Entity::getGridPos(const int gridSizeI) const
{
	if (this->hitboxComponent)
		return sf::Vector2i(
			static_cast<int>(this->hitboxComponent->getPosition().x) / gridSizeI,
			static_cast<int>(this->hitboxComponent->getPosition().y) / gridSizeI
		);
	return sf::Vector2i(
		static_cast<int>(this->sprite.getPosition().x) / gridSizeI,
		static_cast<int>(this->sprite.getPosition().y) / gridSizeI
	);
}

const sf::FloatRect Entity::getGlobalBounds() const
{
	if (this->hitboxComponent)
		return this->hitboxComponent->getGlobalBounds();
	return this->sprite.getGlobalBounds();
}

const sf::FloatRect Entity::getNextPosBounds(const float& dt) const
{
	if (this->hitboxComponent && this->movementComponent)
		return this->hitboxComponent->getNextPos(this->movementComponent->getVelocity() * dt);
	return sf::FloatRect(-1.f, -1.f, -1.f, -1.f);
}

//Functions
void Entity::setPosition(const float x, const float y)
{
	if (this->hitboxComponent)
		this->hitboxComponent->setPosition(x, y);
	else
		this->sprite.setPosition(x, y);
}

void Entity::move(const float dir_x, const float dir_y, const float& dt)
{

	if (this->movementComponent) {
		this->movementComponent->move(dir_x, dir_y, dt);
	}
	else {
		this->sprite.move(sf::Vector2f(dir_x, dir_y));
	}
}

void Entity::stopVelocity(std::string velocity)
{
	if (this->movementComponent) {
		this->movementComponent->stopVelocity(velocity);
	}
}

void Entity::update(const float& dt)
{
	
}

void Entity::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);

	/*
	if (this->hitboxComponent)
		this->hitboxComponent->render(target);
	*/
}