#include "stdafx.h"
#include "TileMap.h"

void TileMap::clear()
{
	for (int x = 0; x < this->size.x; x++)
	{
		for (int y = 0; y < this->size.y; y++)
		{
			for (int z = 0; z < this->layers; z++)
			{
				for (int k = 0; k < this->map[x][y][z].size(); k++)
				{
					delete this->map[x][y][z][k];
					this->map[x][y][z][k] = NULL;
				}
				this->map[x][y][z].clear();
			}
			this->map[x][y].clear();
		}
		this->map[x].clear();
	}
	this->map.clear();
}

TileMap::TileMap(float gridSize, int width, int height, std::string texture_file)
{
	this->gridSizeF = gridSize;
	this->gridSizeI = static_cast<int>(this->gridSizeF);
	this->size.x = width;
	this->size.y = height;
	this->maxSizeWorldF.x = static_cast<float>(width) * gridSize;
	this->maxSizeWorldF.y = static_cast<float>(height) * gridSize;
	this->layers = 2;
	this->textureFile = texture_file;

	this->fromX = this->toX = this->fromY = this->toY = this->layer = 0;

	this->map.resize(this->size.x, std::vector< std::vector< std::vector<Tile*> > >());
	for (int x = 0; x < this->size.x; x++)
	{
		for (int y = 0; y < this->size.y; y++)
		{
			this->map[x].resize(this->size.y, std::vector< std::vector<Tile*> >());
			for (int z = 0; z < this->layers; z++)
			{
				this->map[x][y].resize(this->layers, std::vector<Tile*>());
			}
		}
	}
	
	if (!this->tileSet.loadFromFile(texture_file))
		std::cout << "FAILED TO LOAD TEXTURE SHEET\n";

	this->collisionBox.setSize(sf::Vector2f(gridSize, gridSize));
	this->collisionBox.setFillColor(sf::Color(255, 0, 0, 50));
}

TileMap::~TileMap()
{
	this->clear();
}

//Accessors
const sf::Texture* TileMap::getTileSheet() const
{
	return &this->tileSet;
}

const int TileMap::getLayerSize(const int x, const int y, const int z) const
{
	if (x >= 0 && x < this->map.size()) {
		if (y >= 0 && y < this->map[x].size()) {
			if (layer >= 0 && layer < this->map[x].size()) {
				return static_cast<int>(this->map[x][y][layer].size());
			}
		}
	}
	return -1;
}

const std::vector<sf::Vector2i> TileMap::getCollisionTiles()
{
	std::vector<sf::Vector2i> collisionTiles;
	for (int x = 0; x < map.size(); ++x) {
		for (int y = 0; y < map[x].size(); ++y) {
			for (int z = 0; z < map[x][y].size(); ++z) {
				for (int k = 0; k < map[x][y][z].size(); ++k) {
					if (map[x][y][z][k] && map[x][y][z][k]->getCollision()) {
						collisionTiles.push_back(sf::Vector2i(x, y));
					}
				}
			}
		}
	}
	return collisionTiles;
}

//Functions
void TileMap::saveToFile(const std::string file_name)
{
	/* Saving file in a format
	Size x y
	gridSize
	layers
	texture file
	//All tiles
	gridPos x y layer textureRect x y collision type
	*/

	std::ofstream out_file;
	out_file.open(file_name);
	if (out_file.is_open()) {
		out_file << this->size.x << " " << this->size.y << "\n"
			<< this->gridSizeI << "\n"
			<< this->layers << "\n"
			<< this->textureFile << "\n";

		//All tiles
		for (int x = 0; x < this->size.x; x++)
		{
			for (int y = 0; y < this->size.y; y++)
			{
				for (int z = 0; z < this->layers; z++)
				{
					if (!this->map[x][y][z].empty())
					{
						for (int k = 0; k < this->map[x][y][z].size(); k++)
						{
							out_file << x << " " << y << " " << z << " " << this->map[x][y][z][k]->getAsString() << " "; // LAST SPACE NO SAVING
						}
					}
				}
			}
		}
	}
	else
		std::cout << "COULDN'T SAVE TO FILE\n";
	out_file.close();
}

void TileMap::loadFromFile(const std::string file_name)
{
	std::ifstream in_file;
	in_file.open(file_name);
	if (in_file.is_open()) {
		sf::Vector2i size;
		int gridSize = 0;
		int layers = 0;
		std::string texture_file = "";
		int x = 0;
		int y = 0;
		int z = 0;
		int trX = 0;
		int trY = 0;
		bool collision = false;
		short type = 0;

		//Basics
		in_file >> size.x >> size.y >> gridSize >> layers >> texture_file;


		//All tiles
		this->gridSizeF = static_cast<float>(gridSize);
		this->gridSizeI = gridSize;
		this->size.x = size.x;
		this->size.y = size.y;
		this->layers = layers;
		this->textureFile = texture_file;

		this->clear();

		this->map.resize(this->size.x, std::vector< std::vector< std::vector< Tile* > > >());
		for (int x = 0; x < this->size.x; x++)
		{
			for (int y = 0; y < this->size.y; y++)
			{
				this->map[x].resize(this->size.y, std::vector< std::vector< Tile* > >());
				
				for (int z = 0; z < this->layers; z++)
				{
					this->map[x][y].resize(this->layers, std::vector<Tile*>());
				}
			}
		}

		if (!this->tileSet.loadFromFile(texture_file))
			std::cout << "FAILED TO LOAD TEXTURE SHEET\n";

		//Load all tiles
		while (in_file >> x >> y >> z >> trX >> trY >> collision >> type)
		{
			this->map[x][y][z].push_back(
				new Tile(
					x, y,
					this->gridSizeF,
					this->tileSet,
					sf::IntRect(trX, trY, this->gridSizeI, this->gridSizeI),
					collision, type
				)
			);

		}
	}
	else
		std::cout << "COULDN'T LOAD FROM FILE\n";
	in_file.close();
}

void TileMap::addTile(const int x, const int y, const int z, const sf::IntRect& texture_rect, const bool& collision, const short& type)
{
	if (x < this->size.x && x >= 0 && 
		y < this->size.y && y >= 0 &&
		z < this->layers && z >= 0) 
	{
		this->map[x][y][z].push_back(new Tile(x, y, this->gridSizeF, this->tileSet, texture_rect, collision, type));
		std::cout << "ADDED TILE\n";
	}
}

void TileMap::removeTile(const int x, const int y, const int z)
{
	if (x < this->size.x && x >= 0 &&
		y < this->size.y && y >= 0 &&
		z < this->layers && z >= 0)
	{
		if (!this->map[x][y][z].empty()) {
			delete this->map[x][y][z][this->map[x][y][z].size() - 1];
			this->map[x][y][z].pop_back();
			std::cout << "REMOVED TILE\n";
		}
	}
}

void TileMap::updateCollision(Entity* entity, const float& dt)
{
	//World bounds
	if (entity->getPosition().x < 0.f) {
		entity->setPosition(0.f, entity->getPosition().y);
		entity->stopVelocity("x");
	}
	else if (entity->getPosition().x + entity->getGlobalBounds().width > this->maxSizeWorldF.x) {
		entity->setPosition(this->maxSizeWorldF.x - entity->getGlobalBounds().width, entity->getPosition().y);
		entity->stopVelocity("x");
	}

	if (entity->getPosition().y < 0.f) {
		entity->setPosition(entity->getPosition().x, 0.f);
		entity->stopVelocity("y");
	}
	else if (entity->getPosition().y + entity->getGlobalBounds().height > this->maxSizeWorldF.y) {
		entity->setPosition(entity->getPosition().x, this->maxSizeWorldF.y - entity->getGlobalBounds().height);
		entity->stopVelocity("y");
	}

	//Tiles
	this->layer = 0;

	this->fromX = entity->getGridPos(this->gridSizeI).x - 1;
	if (this->fromX < 0)
		this->fromX = 0;
	else if (this->fromX > this->size.x)
		this->fromX = this->size.x;

	this->toX = entity->getGridPos(this->gridSizeI).x + 3;
	if (this->toX < 0)
		this->toX = 0;
	else if (this->toX > this->size.x)
		this->toX = this->size.x;

	this->fromY = entity->getGridPos(this->gridSizeI).y - 1;
	if (this->fromY < 0)
		this->fromY = 0;
	else if (this->fromY > this->size.y)
		this->fromY = this->size.y;

	this->toY = entity->getGridPos(this->gridSizeI).y + 3;
	if (this->toY < 0)
		this->toY = 0;
	else if (this->toY > this->size.y)
		this->toY = this->size.y;

	for (int x = this->fromX; x < this->toX; x++)
	{
		for (int y = this->fromY; y < this->toY; y++)
		{
			for (size_t k = 0; k < this->map[x][y][this->layer].size(); k++)
			{
				sf::FloatRect playerBounds = entity->getGlobalBounds();
				sf::FloatRect wallBounds = this->map[x][y][this->layer][k]->getGlobalBounds();
				sf::FloatRect nextPositionBounds = entity->getNextPosBounds(dt);

				if (this->map[x][y][this->layer][k]->getCollision() &&
					this->map[x][y][this->layer][k]->intersects(nextPositionBounds)
					)
				{
					//Bottom collision
					if (playerBounds.top < wallBounds.top
						&& playerBounds.top + playerBounds.height < wallBounds.top + wallBounds.height
						&& playerBounds.left < wallBounds.left + wallBounds.width
						&& playerBounds.left + playerBounds.width > wallBounds.left
						)
					{
						entity->stopVelocity("y");
					}

					//Top collision
					else if (playerBounds.top > wallBounds.top
						&& playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height
						&& playerBounds.left < wallBounds.left + wallBounds.width
						&& playerBounds.left + playerBounds.width > wallBounds.left
						)
					{
						entity->stopVelocity("y");
					}

					//Right collision
					if (playerBounds.left < wallBounds.left
						&& playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
						&& playerBounds.top < wallBounds.top + wallBounds.height
						&& playerBounds.top + playerBounds.height > wallBounds.top
						)
					{
						entity->stopVelocity("x");
					}

					//Left collision
					else if (playerBounds.left > wallBounds.left
						&& playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
						&& playerBounds.top < wallBounds.top + wallBounds.height
						&& playerBounds.top + playerBounds.height > wallBounds.top
						)
					{
						entity->stopVelocity("x");
					}
				}
			}
		}
	}
}

void TileMap::update()
{

}

void TileMap::render(sf::RenderTarget& target, const sf::Vector2i& gridPos)
{
	// Render all tiles
	for (int x = 0; x < this->size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			for (int k = 0; k < this->map[x][y][this->layer].size(); k++)
			{
				this->map[x][y][this->layer][k]->render(target);
			}
		}
	}
}