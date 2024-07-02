#include "stdafx.h"
#include "GameState.h"

void GameState::initRender()
{
	this->renderTexture.create(
		this->stateData->graphSettings->resolution.width,
		this->stateData->graphSettings->resolution.height
	);
	this->renderSprite.setTexture(this->renderTexture.getTexture());
	this->renderSprite.setTextureRect(sf::IntRect(0, 0, this->stateData->graphSettings->resolution.width, this->stateData->graphSettings->resolution.height));
}

void GameState::initBackground()
{
	if (!this->backgroundTexture.loadFromFile("assets/background/gameBackground.png"))
		std::cout << "Couldn't load background texture!\n";

	this->background.setTexture(this->backgroundTexture);
	this->background.setScale(static_cast<float>(this->stateData->graphSettings->resolution.width) / backgroundTexture.getSize().x, static_cast<float>(this->stateData->graphSettings->resolution.height) / backgroundTexture.getSize().y);
}

void GameState::initView()
{
	this->view.setSize(sf::Vector2f(this->stateData->graphSettings->resolution.width / 4.f, this->stateData->graphSettings->resolution.height / 4.f));
	this->view.setCenter(this->stateData->graphSettings->resolution.width / 2.f, this->stateData->graphSettings->resolution.height / 2.f);
}

void GameState::initKeybinds()
{
	std::ifstream ifs("./config/gamestate_keybinds.ini");

	if (ifs.is_open()) {
		std::string key = "";
		std::string actualKey = "";
		while (ifs >> key >> actualKey) {
			this->keybinds[key] = this->supportedKeys->at(actualKey);
		}
	}

	ifs.close();
}

void GameState::initFonts()
{
	if (!this->font.loadFromFile("./fonts/square.ttf")) {
		std::cout << "Couldnt load font\n";
	}
}

void GameState::initText()
{
	this->allTimeBestText.setFont(font);
	this->allTimeBestText.setCharacterSize(75);
	this->allTimeBestText.setScale(0.1f, 0.1f);
	this->allTimeBestText.setFillColor(sf::Color::White);
	this->allTimeBestText.setPosition(10.f, 10.f);

	this->currentLevelText.setFont(font);
	this->currentLevelText.setCharacterSize(75);
	this->currentLevelText.setScale(0.1f, 0.1f);
	this->currentLevelText.setFillColor(sf::Color::White);
	this->currentLevelText.setPosition(10.f, 30.f);

	this->scoreText.setFont(font);
	this->scoreText.setCharacterSize(75);
	this->scoreText.setScale(0.1f, 0.1f);
	this->scoreText.setFillColor(sf::Color::White);
	this->scoreText.setPosition(10.f, 50.f);
}

void GameState::initTextures()
{
	if (!this->texture["PLAYER_SHEET"].loadFromFile("./assets/pack/heroes/knight/knight_spritesheet.png"))
		std::cout << "Couldnt load player texture!\n";
	if (!this->texture["ENEMY_SHEET_1"].loadFromFile("./assets/pack/enemies/goblin/goblin_spritesheet.png"))
		std::cout << "Couldnt load goblin  texture!\n";
	if (!this->texture["ENEMY_SHEET_2"].loadFromFile("./assets/pack/enemies/flying_creature/fly_creature_spritesheet.png"))
		std::cout << "Couldnt load flying creature  texture!\n";
	if (!this->texture["ENEMY_SHEET_3"].loadFromFile("./assets/pack/enemies/slime/slime_spritesheet.png"))
		std::cout << "Couldnt load slime texture!\n";
}

void GameState::initPauseMenu()
{
	this->pMenu = new PauseMenu(*this->window, this->font);

	this->pMenu->addButton("RESUME", "resume", 1);
	this->pMenu->addButton("SETTINGS", "settings", 2);
	this->pMenu->addButton("EXIT", "exit", 3);
}

void GameState::initEndScreenMenu()
{
	this->endMenu = new EndScreenMenu(*this->window, this->font);

	this->endMenu->addButton("PLAY_AGAIN", "play again", 3);
	this->endMenu->addButton("EXIT", "exit", 4);
}

void GameState::initWinScreen()
{
	this->winScreen = new WinScreen(*this->window, this->font);

	this->winScreen->addButton("CONTINUE", "continue", 1);
	this->winScreen->addButton("EXIT", "exit", 2);
}

void GameState::initPlayers()
{
	this->player = new Player(20.f, 20.f, this->texture["PLAYER_SHEET"]);
}

void GameState::initEnemies()
{
	this->spawner = new EnemySpawner(sf::Vector2i(50, 50), 16.f, this->texture);
}

void GameState::initTileMap()
{
	this->tileMap = new TileMap(this->stateData->gridSize, 50, 50, "assets/pack/fulltilemap.png");
	this->tileMap->loadFromFile("config/map.slmp");
}

//Constructor / Destructor
GameState::GameState(StateData* state_data)
	: State(state_data)
{
	this->currentLevel = 1;
	this->score = 0;
	this->winMenuActive = false;
	this->continuing = false;
	this->endMenuActive = false;
	this->scoreOutputted = false;

	this->initRender();
	this->initBackground();
	this->initView();
	this->initKeybinds();
	this->initFonts();
	this->initText();
	this->initTextures();
	this->initPauseMenu();
	this->initEndScreenMenu();
	this->initWinScreen();
	this->initPlayers();
	this->initEnemies();
	this->initTileMap();

	//Input highscore
	std::ifstream inFile("config/highscore.txt");
	inFile >> this->bestName >> this->highScore;
	inFile.close();
}

GameState::~GameState()
{
	delete this->player;
	delete this->pMenu;
	delete this->endMenu;
	delete this->winScreen;
	delete this->tileMap;
	delete this->spawner;
}

void GameState::textBoxTypedOn(sf::Event ev)
{
	if (!this->player->isAlive()) {
		this->endMenu->textBoxTypedOn(ev);
	}
}

//Functions
void GameState::updateView(const float& dt)
{
	this->view.setSize(sf::Vector2f(this->stateData->graphSettings->resolution.width / 4.f, this->stateData->graphSettings->resolution.height / 4.f));
	this->view.setCenter(std::floor(this->player->getPosition().x), std::floorf(this->player->getPosition().y));
}

void GameState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeyTime() && !this->endMenuActive) {
		if (!this->paused)
			this->pauseState();
		else
			this->unpauseState();
	}
}

void GameState::updatePlayerInput(const float& dt)
{
	//Update player input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_LEFT"))))
		this->player->move(-1.f, 0.f, dt);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_RIGHT"))))
		this->player->move(1.f, 0.f, dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_UP"))))
		this->player->move(0.f, -1.f, dt);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN"))))
		this->player->move(0.f, 1.f, dt);

	//Attack
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		this->player->attack();
	}
}

void GameState::updateText()
{
	sf::Vector2f viewPosition = this->view.getCenter() - this->view.getSize() / 2.f;
	
	this->allTimeBestText.setPosition(10.f + viewPosition.x, 10.f + viewPosition.y);
	this->allTimeBestText.setString("highest score " + this->bestName + " : " + std::to_string(this->highScore));

	this->currentLevelText.setPosition(10.f + viewPosition.x, 30.f + viewPosition.y);
	this->currentLevelText.setString("current level: " + std::to_string(this->currentLevel));
	
	this->scoreText.setPosition(10.f + viewPosition.x, 50.f + viewPosition.y);
	this->scoreText.setString("kills: " + std::to_string(this->score));

}

void GameState::updatePMenuButtons()
{
	if (this->pMenu->isButtonPressed("EXIT")) {
		this->endState();
	}
	if (this->pMenu->isButtonPressed("RESUME")) {
		this->paused = false;
	}
	if (this->pMenu->isButtonPressed("SETTINGS")) {
		this->states->push(new SettingsState(this->stateData));
	}
}

void GameState::updateEndMenuButtons()
{
	if (this->endMenu->isButtonPressed("EXIT")) {
		this->endState();
	}
	if (this->endMenu->isButtonPressed("PLAY_AGAIN")) {
		this->endState();
		this->states->push(new GameState(this->stateData));
	}
}

void GameState::updateWinScreenButtons()
{
	if (this->winScreen->isButtonPressed("EXIT")) {
		this->endState();
	}
	if (this->winScreen->isButtonPressed("CONTINUE")) {
		this->continuing = true;
	}
}

void GameState::updateTilemap(const float& dt)
{
	this->tileMap->update();
	this->tileMap->updateCollision(this->player, dt);
	for (auto& enemy : this->spawner->getEnemies()) {
		this->tileMap->updateCollision(enemy, dt);
	}
}

void GameState::updateSpawner(const float& dt)
{
	this->spawner->update(dt, this->tileMap->getCollisionTiles());
	this->spawner->moveEnemiesTowardsPlayer(this->player->getPosition(), dt);

	std::vector<Enemy*> livingEnemies;
	for (auto enemy : this->spawner->getEnemies()) {
		if (this->player->getSwordGlobalBounds().intersects(enemy->getGlobalBounds()) && this->player->getAttacking()) {
			enemy->takeDamage(this->player->getDamage());
		}
		if (enemy->getAlive()) {
			livingEnemies.push_back(enemy);
		}
		else {
			this->score++;
			this->spawner->enemyKilled();
			this->player->heal(5);
			if (this->spawner->levelUp()) {
				++this->currentLevel;
				this->player->upgradeWeapon();
				this->spawner->increaseLevel();
			}
			delete enemy;
		}
	}
	this->spawner->setEnemies(livingEnemies);
}

void GameState::updateEnemyCollision(const float& dt)
{
	const std::vector<Enemy*>& enemies = this->spawner->getEnemies();

	for (int i = 0; i < enemies.size(); ++i) {
		for (int j = i + 1; j < enemies.size(); ++j) {
			if (enemies[i]->getGlobalBounds().intersects(enemies[j]->getGlobalBounds())) {
				if (enemies[i]->getPosition().x < enemies[j]->getPosition().x) {
					enemies[i]->move(-1.f, 0.f, dt);
					enemies[j]->move(1.f, 0.f, dt);
				}
				else {
					enemies[i]->move(1.f, 0.f, dt);
					enemies[j]->move(-1.f, 0.f, dt);
				}
				if (enemies[i]->getPosition().y < enemies[j]->getPosition().y) {
					enemies[i]->move(0.f, -1.f, dt);
					enemies[j]->move(0.f, 1.f, dt);
				}
				else {
					enemies[i]->move(0.f, 1.f, dt);
					enemies[j]->move(0.f, -1.f, dt);
				}
			}
		}
	}
}

void GameState::update(const float& dt)
{
	this->updateMousePos(&this->view);
	this->updateKeyTime(dt);
	this->updateInput(dt);

	// Unpaused update
	if (!this->paused && this->window->hasFocus()) {
		this->updateView(dt);

		sf::Vector2f viewSize = this->view.getSize();
		this->background.setPosition(this->view.getCenter() - viewSize / 2.f);
		this->background.setScale(viewSize.x / this->background.getLocalBounds().width, viewSize.y / this->background.getLocalBounds().height);

		this->updatePlayerInput(dt);

		if (this->player->isAlive()) {
			if (this->currentLevel >= 13 && !this->continuing) {
				this->winScreen->update(this->mousePosWindow, *this->window);
				this->updateWinScreenButtons();
			}
			else {
				this->updateSpawner(dt);
				this->updateEnemyCollision(dt);
				this->updateTilemap(dt);
				this->player->update(dt, mousePosView);


				for (auto& enemy : this->spawner->getEnemies()) {
					if (this->player->getGlobalBounds().intersects(enemy->getGlobalBounds()) && enemy->getHitCooldown() == 0.0f) {
						enemy->setHitCooldown(1.0f);
						this->player->takeDamage(10);
					}
				}

				this->updateText();
			}
			
		}
		else {
			this->endMenuActive = true;
			this->endMenu->update(this->mousePosWindow, *this->window);
			this->updateEndMenuButtons();
			if (this->endMenu->alreadyGotName() && !this->scoreOutputted) {
				this->scoreOutputted = true;

				//Output score
				std::ofstream outfile("config/scores.txt", std::ios_base::app);
				outfile << this->endMenu->getName() << " : " << this->score << " kills" << std::endl;
				outfile.close();

				if (this->score > this->highScore) {
					this->highScore = this->score;
					this->bestName = this->endMenu->getName();

					std::ofstream outFile("config/highscore.txt");
					outFile << this->bestName << " " << this->highScore << "\n";
					outFile.close();
				}
			}
		}
	}
	// Paused update
	else {
		this->pMenu->update(this->mousePosWindow, *this->window);
		this->updatePMenuButtons();
	}
}

void GameState::render(sf::RenderTarget* target)
{
	this->renderTexture.clear();

	this->renderTexture.setView(this->view);

	this->renderTexture.draw(this->background);
	
	this->tileMap->render(this->renderTexture, this->player->getGridPos(static_cast<int>(this->stateData->gridSize)));
	this->player->render(this->renderTexture);

	for (auto enemy : this->spawner->getEnemies()) {
		enemy->render(this->renderTexture);
	}

	this->renderTexture.draw(this->allTimeBestText);
	this->renderTexture.draw(this->currentLevelText);
	this->renderTexture.draw(this->scoreText);

	// Winning screen
	if (this->currentLevel >= 13 && !this->continuing) {
		this->renderTexture.setView(this->renderTexture.getDefaultView());
		this->winScreen->render(this->renderTexture);
	}

	// Render end menu screen
	if (!this->player->isAlive()) {
		this->renderTexture.setView(this->renderTexture.getDefaultView());
		this->endMenu->render(this->renderTexture);
	}

	// Render pause menu
	if (this->paused) {
		this->renderTexture.setView(this->renderTexture.getDefaultView());
		this->pMenu->render(this->renderTexture);
	}

	// FINAL RENDER
	this->renderTexture.display();
	this->renderSprite.setTexture(this->renderTexture.getTexture());
	target->draw(this->renderSprite);
}