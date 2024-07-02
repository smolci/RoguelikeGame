#include "stdafx.h"
#include "Enemy.h"

//Initializers
void Enemy::initVariables()
{
	isAlive = true;
	this->health = 100;
	this->hitCooldown = 1.0f;
	this->currentHitCooldown = this->hitCooldown;
	this->scale.x = this->scale.y = 1.f;
}

void Enemy::initHealthBar()
{
	this->healthBar = new gui::HealthBar(15.f, 2.f, sf::Color::Red, sf::Color::Green, this->getPosition());
	this->healthBar->setPosition(sf::Vector2f(this->getPosition().x + 1.f, this->getPosition().y - 3.f));
	this->healthBar->setPercantage(this->health / 100.f);
}

void Enemy::initComponents()
{

}

//Constructor / Destructor
Enemy::Enemy(float x, float y, sf::Texture& textureSheet)
{
	this->initVariables();

	this->setTexture(textureSheet);
	this->setPosition(x, y);

	this->initHealthBar();

	this->createHitboxComp(this->sprite, 0.f, 0.f, 16.f, 16.f);
	this->createMovComp(100.f, 1250.f, 500.f); // TODO: Change mov speed?
	this->createAnimComp(textureSheet);

	this->animationComponent->addAnimation("IDLE", 10.f, 0, 0, 5, 0, 16, 16);
	this->animationComponent->addAnimation("RUN", 10.f, 0, 1, 5, 1, 16, 16);
}

Enemy::~Enemy()
{
	delete this->healthBar;
}

bool Enemy::getAlive()
{
	return isAlive;
}

float Enemy::getHitCooldown()
{
	return this->currentHitCooldown;
}

void Enemy::setHitCooldown(float cooldown)
{
	this->currentHitCooldown = cooldown;
}

void Enemy::moveTowardsPlayer(const sf::Vector2f& playerPosition, float dt) {
	sf::Vector2f direction = playerPosition - getPosition();

	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length != 0) {
		direction /= length;
	}

	move(direction.x, direction.y, dt);
}

void Enemy::takeDamage(int damage)
{
	health -= damage;
	if (health <= 0) {
		this->kill();
	}
}

void Enemy::kill()
{
	isAlive = false;
}

void Enemy::updateHealthBar()
{
	this->healthBar->setPosition(sf::Vector2f(this->getPosition().x + 1.f, this->getPosition().y - 3.f));
	this->healthBar->setPercantage(this->health / 100.f);
}

//Functions
void Enemy::update(const float& dt)
{
	if (this->currentHitCooldown > 0.0f) {
		this->currentHitCooldown -= dt;
		if (this->currentHitCooldown < 0.0f) {
			this->currentHitCooldown = 0.0f;
		}
	}

	this->updateHealthBar();
	this->movementComponent->update(dt);
	this->sprite.setScale(scale);
	this->sprite.setOrigin(0.f, 0.f);
	if (this->movementComponent->movingState() == "idle") {
		this->animationComponent->play("IDLE", dt);
	}
	else if (this->movementComponent->movingState() == "moving_right" || this->movementComponent->movingState() == "moving") {
		this->animationComponent->play("RUN", dt);
	}
	else if (this->movementComponent->movingState() == "moving_left") {
		this->sprite.setOrigin(16.f, 0.f);
		this->sprite.setScale(-this->scale.x, this->scale.y);
		this->animationComponent->play("RUN", dt);
	}
	this->hitboxComponent->update();
}

void Enemy::render(sf::RenderTarget& target)
{
	Entity::render(target);
	this->healthBar->render(target);
}