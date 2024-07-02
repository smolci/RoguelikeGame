#pragma once

enum TileTypes {DEFAULT = 0, DAMAGING};

class Tile
{
private:

protected:
    sf::RectangleShape tile;
    bool collision;
    short type;

public:
    Tile();
    Tile(int grid_x, int grid_y, float gridSizeF,
        const sf::Texture& texture, const sf::IntRect& texture_rect,
        bool collision = false, short type = TileTypes::DEFAULT
    );
    virtual ~Tile();

    const sf::Vector2f& getPosition() const;
    const sf::FloatRect getGlobalBounds() const;
    const bool intersects(const sf::FloatRect bounds) const;
    const bool& getCollision() const;
    const std::string getAsString() const;

    void update();
    void render(sf::RenderTarget& target);
};