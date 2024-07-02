#include "stdafx.h"
#include "EnemySpawner.h"

EnemySpawner::EnemySpawner(sf::Vector2i map_size, float grid_size, std::map<std::string, sf::Texture>& texture)
	: mapSize(map_size), gridSize(grid_size), texture(texture)
{
	this->spawnTimer = 0;
	this->spawnInterval = 5;
	this->maxEnemies = 1;
	this->currentLevel = 1;
	this->enemiesSpawned = 0;
	this->enemiesKilled = 0;
}

EnemySpawner::~EnemySpawner()
{
	for (int i = 0; i < this->enemies.size(); i++) {
		delete this->enemies[i];
	}
}

const std::vector<Enemy*>& EnemySpawner::getEnemies() const
{
	return this->enemies;
}

const int EnemySpawner::getMaxEnemies()
{
	return this->maxEnemies;
}

bool EnemySpawner::levelUp()
{
	return this->enemiesKilled >= this->maxEnemies;
}

void EnemySpawner::enemyKilled()
{
	this->enemiesKilled++;
}

void EnemySpawner::setEnemies(std::vector<Enemy*>& living_enemies)
{
	this->enemies = living_enemies;
}

void EnemySpawner::spawnEnemy(const std::vector<sf::Vector2i>& collision_tiles)
{
	if (this->enemies.size() < this->maxEnemies) {
		int x, y;
		do {
			x = std::rand() % this->mapSize.x;
			y = std::rand() % this->mapSize.y;
		} while (std::find(collision_tiles.begin(), collision_tiles.end(), sf::Vector2i(x, y)) != collision_tiles.end());

		float worldX = static_cast<float>(x) * this->gridSize;
		float worldY = static_cast<float>(y) * this->gridSize;

		auto it = std::next(std::begin(texture), std::rand() % texture.size());
		sf::Texture& selectedTexture = it->second;

		this->enemies.push_back(new Enemy(worldX, worldY, selectedTexture));
		this->enemiesSpawned++;
	}
}

void EnemySpawner::moveEnemiesTowardsPlayer(sf::Vector2f player_pos, const float& dt) {
	for (auto& enemy : this->enemies) {
		enemy->moveTowardsPlayer(player_pos, dt);
	}
}

void EnemySpawner::increaseLevel()
{
	this->maxEnemies += 3;
	this->enemiesSpawned = 0;
	this->enemiesKilled = 0;
	this->spawnInterval /= 2;
	this->currentLevel++;
}

void EnemySpawner::update(const float& dt, const std::vector<sf::Vector2i>& collision_tiles)
{
	for (auto& enemy : this->enemies) {
		enemy->update(dt);
	}

	this->spawnTimer += dt;
	if (this->spawnTimer >= this->spawnInterval && this->enemiesSpawned < this->maxEnemies) {
		this->spawnEnemy(collision_tiles);
		this->spawnTimer = 0;
	}
}