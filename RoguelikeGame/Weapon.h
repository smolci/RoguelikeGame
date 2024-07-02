#pragma once

class Weapon
{
private:

protected:
    sf::Sprite weaponSprite;
    sf::Texture weaponTexture;

    int damageMin, damageMax;

public:
    Weapon();
    virtual ~Weapon();

    virtual void update(const sf::Vector2f& mouse_pos_view, const sf::Vector2f center, bool isOnCooldown) = 0;
    virtual void render(sf::RenderTarget& target) = 0;
};