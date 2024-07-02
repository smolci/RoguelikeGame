#pragma once

#include "Entity.h"
#include "Sword.h"

class Player :
    public Entity
{
private:
    //Variables
    bool attacking;
    float attackCooldown, currentCooldown, 
        attackDuration, currentAttackDuration;
    int health;
    int damage;

    sf::Vector2f scale;

    gui::HealthBar* healthBar;

    Sword sword;

    //Initializers
    void initVariables();
    void initHealthBar();
    void initComponents();
public:
    Player(float x, float y, sf::Texture& textureSheet);
    virtual ~Player();

    bool getAttacking();
    int getDamage();
    bool isAlive();
    const sf::FloatRect getSwordGlobalBounds();

    //Functions
    void attack();
    void takeDamage(int damage);
    void heal(int hp);
    void upgradeWeapon();
    void updateHealthBar();
    virtual void update(const float& dt, sf::Vector2f& mouse_pos_view);
    virtual void render(sf::RenderTarget& target);
};