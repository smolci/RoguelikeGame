#include "stdafx.h"
#include "Player.h"

//Initializers
void Player::initVariables()
{
	this->attacking = false;
	this->attackCooldown = 0.5f;
	this->currentCooldown = this->attackCooldown;
	this->attackDuration = 1.f;
	this->currentAttackDuration = 0.f;
	this->health = 100;
	this->damage = 25;
	this->scale.x = this->scale.y = 1.f;
}

void Player::initHealthBar()
{
	this->healthBar = new gui::HealthBar(15.f, 2.f, sf::Color::Red, sf::Color::Green, this->getPosition());
}

void Player::initComponents()
{
	
}

//Constructor / Destructor
Player::Player(float x, float y, sf::Texture& textureSheet)
{
	this->initVariables();
	this->initHealthBar();
	
	this->setTexture(textureSheet);
	this->setPosition(x, y);

	this->createMovComp(150.f, 1500.f, 500.f);
	this->createAnimComp(textureSheet);
	this->createHitboxComp(this->sprite, 0.f, 0.f, 16.f, 16.f);

	this->animationComponent->addAnimation("IDLE", 10.f, 0, 0, 5, 0, 16, 16);
	this->animationComponent->addAnimation("RUN", 10.f, 0, 1, 5, 1, 16, 16);
}

Player::~Player()
{
	delete this->healthBar;
}

bool Player::getAttacking()
{
	return attacking;
}

int Player::getDamage()
{
	return this->damage;
}

bool Player::isAlive()
{
	return health > 0;
}

void Player::attack()
{
	if (!attacking && currentCooldown <= 0.0f) {
		attacking = true;
		currentCooldown = attackCooldown;
	}
}

void Player::takeDamage(int damage)
{
	health -= damage;
	if (health <= 0) {
		health = 0;
	}
}

void Player::heal(int hp)
{
	if (health <= 95)
		health += hp;
}

void Player::upgradeWeapon()
{
	this->sword.upgrade();
	this->damage += 5;
}

void Player::updateHealthBar()
{
	this->healthBar->setPosition(sf::Vector2f(this->getPosition().x + 1.f, this->getPosition().y - 3.f));
	this->healthBar->setPercantage(this->health / 100.f);
}

const sf::FloatRect Player::getSwordGlobalBounds()
{
	return this->sword.getGlobalBounds();
}

//Functions
void Player::update(const float& dt, sf::Vector2f& mouse_pos_view)
{
	if (currentCooldown > 0.0f) {
		currentCooldown -= dt;
		if (currentCooldown < 0.0f) {
			currentCooldown = 0.0f;
		}
	}

	if (attacking) {
		currentAttackDuration -= dt;
		if (currentAttackDuration <= 0.f) {
			attacking = false;
			currentAttackDuration = 0.f;
		}
	}

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
	this->updateHealthBar();

	if (this->currentCooldown == 0.f)
		this->sword.update(mouse_pos_view, this->getCenter(), 1);
	else
		this->sword.update(mouse_pos_view, this->getCenter(), 0);
	
}

void Player::render(sf::RenderTarget& target)
{
	Entity::render(target);
	this->healthBar->render(target);
	this->sword.render(target);
}