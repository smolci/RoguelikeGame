#include "stdafx.h"
#include "EditorState.h"

// Initializers
void EditorState::initVariables()
{
	this->textureRect = sf::IntRect(0, 0, static_cast<int>(this->stateData->gridSize), static_cast<int>(this->stateData->gridSize));
	this->collision = false;
	this->type = TileTypes::DEFAULT;
	this->cameraSpeed = 100.f;
	this->layer = 0;
}

void EditorState::initView()
{
	this->view.setSize(sf::Vector2f(
		static_cast<float>(this->stateData->graphSettings->resolution.width),
		static_cast<float>(this->stateData->graphSettings->resolution.height)
	));
	this->view.setCenter(
		static_cast<float>(this->stateData->graphSettings->resolution.width) / 2.f,
		static_cast<float>(this->stateData->graphSettings->resolution.height) / 2.f
	);
}

void EditorState::initBackground()
{
	
}

void EditorState::initFonts()
{
	if (!this->font.loadFromFile("fonts/square.ttf")) {
		std::cout << "Couldnt load font\n";
	}
}

void EditorState::initText()
{
	this->cursorText.setFont(this->font);
	this->cursorText.setFillColor(sf::Color::White);
	this->cursorText.setCharacterSize(12);
}

void EditorState::initKeybinds()
{
	std::ifstream ifs("./config/editorstate_keybinds.ini");

	if (ifs.is_open()) {
		std::string key = "";
		std::string actualKey = "";
		while (ifs >> key >> actualKey) {
			this->keybinds[key] = this->supportedKeys->at(actualKey);
		}
	}

	ifs.close();
}

void EditorState::initPauseMenu()
{
	this->pMenu = new PauseMenu(*this->window, this->font);

	this->pMenu->addButton("LOAD", "load", 1);
	this->pMenu->addButton("SAVE", "save", 2);
	this->pMenu->addButton("EXIT", "exit", 3);
}

void EditorState::initButtons()
{
	
}

void EditorState::initTileMap()
{
	this->tileMap = new TileMap(this->stateData->gridSize, 50, 50, "assets/pack/fulltilemap.png");
}

void EditorState::initGui()
{
	this->sidebar.setSize(sf::Vector2f(100.f, static_cast<float>(this->stateData->graphSettings->resolution.height)));
	this->sidebar.setFillColor(sf::Color(50, 50, 50, 150));

	this->selectorRect.setSize(sf::Vector2f(this->stateData->gridSize, this->stateData->gridSize));
	this->selectorRect.setFillColor(sf::Color(255, 255, 255, 150));
	this->selectorRect.setOutlineThickness(1.f);
	this->selectorRect.setOutlineColor(sf::Color::White);
	this->selectorRect.setTexture(this->tileMap->getTileSheet());
	this->selectorRect.setTextureRect(this->textureRect);

	this->texSelector = new gui::TextureSelector(0.f, 0.f, 500.f, 500.f, this->stateData->gridSize , this->tileMap->getTileSheet(), this->font, "HIDE");
}

EditorState::EditorState(StateData* state_data)
	: State(state_data)
{
	this->initVariables();
	this->initView();
	this->initBackground();
	this->initFonts();
	this->initText();
	this->initKeybinds();
	this->initPauseMenu();
	this->initButtons();
	this->initTileMap();
	this->initGui();
}

EditorState::~EditorState()
{
	auto it = this->buttons.begin();
	for (it = this->buttons.begin(); it != this->buttons.end(); ++it) {
		delete it->second;
	}
	delete this->pMenu;
	delete this->tileMap;
	delete this->texSelector;
}

void EditorState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeyTime()) {
		if (!this->paused)
			this->pauseState();
		else
			this->unpauseState();
	}
}

void EditorState::updateEditorInput(const float& dt)
{
	//Move view
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_UP")))) {
		this->view.move(0.f, -this->cameraSpeed * dt);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_DOWN")))) {
		this->view.move(0.f, this->cameraSpeed * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_RIGHT")))) {
		this->view.move(this->cameraSpeed * dt, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_CAMERA_LEFT")))) {
		this->view.move(-this->cameraSpeed * dt, 0.f);
	}

	//Add a tile on left click of a mouse button
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getKeyTime()) {
		if (!this->sidebar.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow))) {
			if (!this->texSelector->getActive()) {
				this->tileMap->addTile(this->mousePosGrid.x, this->mousePosGrid.y, 0, this->textureRect, this->collision, this->type);
			}
			else {
				this->textureRect = this->texSelector->getTextureRect();
			}
		}
	}
	// Remove a tile on right click of a mouse button
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->getKeyTime()) {
		if (!this->texSelector->getActive()) 
			this->tileMap->removeTile(this->mousePosGrid.x, this->mousePosGrid.y, 0);
	}

	// Toggle collision
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("TOGGLE_COLLISION"))) && this->getKeyTime()) {
		if (this->collision)
			this->collision = false;
		else
			this->collision = true;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("TYPE_INCREASE"))) && this->getKeyTime()) {
			this->type++;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("TYPE_DECREASE"))) && this->getKeyTime()) {
		if (this->type > 0)
			this->type--;
	}
}

void EditorState::updateButtons()
{
	for (auto i : this->buttons) {
		i.second->update(this->mousePosWindow);
	}
}

void EditorState::updateGui(const float& dt)
{
	this->texSelector->update(this->mousePosWindow, dt);

	if (!this->texSelector->getActive()) {
		this->selectorRect.setTextureRect(this->textureRect);
		this->selectorRect.setPosition(this->mousePosGrid.x * this->stateData->gridSize, this->mousePosGrid.y * this->stateData->gridSize);
	}

	this->cursorText.setPosition(this->mousePosView.x + 100.f, this->mousePosView.y + 50.f);
	std::stringstream ss;
	ss << this->mousePosView.x << " " << this->mousePosView.y <<
		"\n" << this->mousePosGrid.x << " " << this->mousePosGrid.y <<
		"\n" << this->textureRect.left << " " << this->textureRect.top <<
		"\nCollision: " << this->collision <<
		"\nType" << this->type <<
		"\nTiles: " << this->tileMap->getLayerSize(this->mousePosGrid.x, this->mousePosGrid.y, this->layer);
	this->cursorText.setString(ss.str());
}

void EditorState::updatePMenuButtons()
{
	if (this->pMenu->isButtonPressed("LOAD"))
		this->tileMap->loadFromFile("config/map.slmp");

	if (this->pMenu->isButtonPressed("SAVE"))
		this->tileMap->saveToFile("config/map.slmp");

	if (this->pMenu->isButtonPressed("EXIT"))
		this->endState();
}

void EditorState::update(const float& dt)
{
	this->updateMousePos(&this->view);
	this->updateKeyTime(dt);
	this->updateInput(dt);

	if (!this->paused) { // Unpaused
		this->updateButtons();
		this->updateGui(dt);
		this->updateEditorInput(dt);
	}
	else { // Paused
		this->pMenu->update(this->mousePosWindow, *this->window);
		this->updatePMenuButtons();
	}

	this->updateButtons();
}

void EditorState::renderButtons(sf::RenderTarget& target)
{
	for (auto i : this->buttons) {
		i.second->render(target);
	}
}

void EditorState::renderGui(sf::RenderTarget& target)
{
	if (!this->texSelector->getActive()) {
		target.setView(this->view);
		target.draw(this->selectorRect);
	}

	target.setView(this->window->getDefaultView());
	this->texSelector->render(target);
	target.draw(this->sidebar);

	target.setView(this->view);
	target.draw(this->cursorText);
}

void EditorState::render(sf::RenderTarget* target)
{
	target->setView(this->view);
	this->tileMap->render(*target, this->mousePosGrid);

	target->setView(this->window->getDefaultView());
	this->renderButtons(*target);

	this->renderGui(*target);

	// Render pause menu
	if (this->paused) {
		target->setView(this->window->getDefaultView());
		this->pMenu->render(*target);
	}
}