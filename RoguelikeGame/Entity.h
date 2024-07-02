#pragma once

#include "HitboxComponent.h"
#include "MovementComponent.h"
#include "AnimationComponent.h"
#include "Gui.h"

class MovementComponent;
class AnimationComponent;
class HitboxComponent;

class Entity
{
private:
	void initVariables();

protected:
	sf::Sprite sprite;

	MovementComponent* movementComponent;
	AnimationComponent* animationComponent;
	HitboxComponent* hitboxComponent;

public:
	Entity();
	virtual ~Entity();

	//Components
	void setTexture(sf::Texture& texture);
	void createMovComp(const float maxVelocity, const float acceleration, const float deceleration);
	void createAnimComp(sf::Texture& textureSheet);
	void createHitboxComp(sf::Sprite& sprite,
		float offset_x, float offset_y,
		float width, float height
	);

	//Accessors
	virtual const sf::Vector2f& getPosition();
	virtual const sf::Vector2f getCenter() const;
	virtual const sf::Vector2i getGridPos(const int gridSizeI) const;
	virtual const sf::FloatRect getGlobalBounds() const;
	virtual const sf::FloatRect getNextPosBounds(const float& dt) const;

	//Functions
	virtual void setPosition(const float x, const float y);
	virtual void move(const float x, const float y, const float& dt);
	virtual void stopVelocity(std::string velocity);

	virtual void update(const float& dt);
	virtual void render(sf::RenderTarget& target);
};