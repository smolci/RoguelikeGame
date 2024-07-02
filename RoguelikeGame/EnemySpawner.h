#pragma once

#include "Enemy.h"

class EnemySpawner
{
private:
	sf::Vector2i mapSize;
	float gridSize; // Tile size
	float spawnTimer, spawnInterval;
	int maxEnemies, currentLevel, enemiesSpawned, enemiesKilled;
	std::map<std::string, sf::Texture>& texture;

	std::vector<Enemy*> enemies;

public:
	EnemySpawner(sf::Vector2i map_size, float grid_size, std::map<std::string, sf::Texture>& texture);
	virtual ~EnemySpawner();

	const std::vector<Enemy*>& getEnemies() const;
	const int getMaxEnemies();
	bool levelUp();
	void enemyKilled();
	void setEnemies(std::vector<Enemy*>& living_enemies);
	void moveEnemiesTowardsPlayer(sf::Vector2f player_pos, const float& dt);
	void increaseLevel();
	void spawnEnemy(const std::vector<sf::Vector2i>& collision_tiles);
	void update(const float& dt, const std::vector<sf::Vector2i>& collision_tiles);
};