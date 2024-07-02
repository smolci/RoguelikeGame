#include "stdafx.h"
#include "MovementComponent.h"

//Constructor / Destructor
MovementComponent::MovementComponent(sf::Sprite& sprite, 
	float maxVelocity, float acceleration, float deceleration)
	: sprite(sprite), maxVelocity(maxVelocity), 
	acceleration(acceleration), deceleration(deceleration)
{
	this->maxVelocity = maxVelocity;
}

MovementComponent::~MovementComponent()
{

}

const sf::Vector2f& MovementComponent::getVelocity() const
{
	return this->velocity;
}

//Functions
const std::string MovementComponent::movingState() const
{
	// Moving
	if (this->velocity.x != 0.f || this->velocity.y != 0.f) {
		if (this->velocity.x < 0.f)
			return "moving_left";
		else if (this->velocity.x > 0.f)
			return "moving_right";
		else
			return "moving";
	}
	//Not moving
	else
		return "idle";
}

void MovementComponent::stopVelocity(std::string velocity)
{
	if (velocity == "xy") {
		this->velocity.x = 0.f;
		this->velocity.y = 0.f;
	}
	else if (velocity == "x") {
		this->velocity.x = 0.f;
	}
	else if (velocity == "y") {
		this->velocity.y = 0.f;
	}
}

void MovementComponent::move(const float dir_x, const float dir_y, const float& dt)
{
	this->velocity.x += this->acceleration * dir_x * dt;
	this->velocity.y += this->acceleration * dir_y * dt;
}

void MovementComponent::update(const float& dt)
{
	if (this->velocity.x > 0.f) { // Moving right
		// Max velocity check
		if (this->velocity.x > this->maxVelocity) {
			this->velocity.x = this->maxVelocity;
		}

		// Deceleration
		this->velocity.x -= this->deceleration * dt;
		if (this->velocity.x < 0.f)
			this->velocity.x = 0.f;
	}
	else if (this->velocity.x < 0.f){ // Moving left
		// Max velocity check
		if (this->velocity.x < -this->maxVelocity) {
			this->velocity.x = -this->maxVelocity;
		}

		// Deceleration
		this->velocity.x += this->deceleration * dt;
		if (this->velocity.x > 0.f)
			this->velocity.x = 0.f;
	}

	if (this->velocity.y > 0.f) { // Moving down
		// Max velocity check
		if (this->velocity.y > this->maxVelocity) {
			this->velocity.y = this->maxVelocity;
		}

		// Deceleration
		this->velocity.y -= this->deceleration * dt;
		if (this->velocity.y < 0.f)
			this->velocity.y = 0.f;
	}
	else if (this->velocity.y < 0.f) { // Moving up
		// Max velocity check
		if (this->velocity.y < -this->maxVelocity) {
			this->velocity.y = -this->maxVelocity;
		}

		// Deceleration
		this->velocity.y += this->deceleration * dt;
		if (this->velocity.y > 0.f) {
			this->velocity.y = 0.f;
		}
	}

	
	//Final movement
	this->sprite.move(this->velocity * dt);
}