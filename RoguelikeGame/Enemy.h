#pragma once

#include "Entity.h"

class Enemy :
    public Entity
{
private:
    //Variables
    bool isAlive;
    int health;
    sf::Vector2f scale;
    float hitCooldown;
    float currentHitCooldown;

    gui::HealthBar* healthBar;

    //Initializers
    void initVariables();
    void initHealthBar();
    void initComponents();
public:
    Enemy(float x, float y, sf::Texture& textureSheet);
    virtual ~Enemy();

    bool getAlive();
    float getHitCooldown();
    void setHitCooldown(float cooldown);

    //Functions
    void moveTowardsPlayer(const sf::Vector2f& playerPosition, float dt);
    void takeDamage(int damage);
    void kill();
    void updateHealthBar();
    virtual void update(const float& dt);
    virtual void render(sf::RenderTarget& target);
};