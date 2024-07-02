#pragma once

#include "Tile.h"
#include "Entity.h"

class Tile;
class Entity;

class TileMap
{
private:
	int layers;
	float gridSizeF;
	int gridSizeI;
	sf::Vector2f maxSizeWorldF;
	sf::Vector2i size;
	std::vector <std::vector <std::vector<std::vector< Tile* > > > > map;
	std::string textureFile;
	sf::Texture tileSet;
	sf::RectangleShape collisionBox;

	int fromX, toX, fromY, toY, layer;


	void clear();

public:
	TileMap(float gridSize, int width, int height, std::string texture_file);
	virtual ~TileMap();

	//Accessors
	const sf::Texture* getTileSheet() const;
	const int getLayerSize(const int x, const int y, const int z) const;
	const std::vector<sf::Vector2i> getCollisionTiles();

	//Functions
	void saveToFile(const std::string file_name);
	void loadFromFile(const std::string file_name);

	void addTile(const int x, const int y, const int z, const sf::IntRect& texture_rect, const bool& collision, const short& type);
	void removeTile(const int x, const int y, const int z);

	void updateCollision(Entity* entity, const float& dt);

	void update();
	void render(sf::RenderTarget& target, const sf::Vector2i& gridPosition);
};