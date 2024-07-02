#pragma once

class MovementComponent
{
private:
	sf::Sprite& sprite;

	float maxVelocity;
	float acceleration;
	float deceleration;
	sf::Vector2f velocity;

	//Initializers

public:
	MovementComponent(sf::Sprite& sprite, float maxVelocity, float acceleration, float deceleration);
	virtual ~MovementComponent();

	//Accessors
	const sf::Vector2f& getVelocity() const;

	//Functions
	const std::string movingState() const;
	void stopVelocity(std::string velocity);

	void move(const float x, const float y, const float& dt);
	void update(const float& dt);
};