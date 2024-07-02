#pragma once
#include "Weapon.h"

class MeleeWeapon;

class Sword :
    public Weapon
{
private:
    int weaponLevel;
public:
    Sword();
    virtual ~Sword();

    const sf::FloatRect getGlobalBounds();

    void upgrade();
    virtual void update(const sf::Vector2f& mouse_pos_view, const sf::Vector2f center, bool isOnColldown);
    virtual void render(sf::RenderTarget& target);
};