#include "stdafx.h"
#include "State.h"

State::State(StateData* state_data)
{
	this->stateData = state_data;
	this->window = state_data->window;
	this->supportedKeys = state_data->supportedKeys;
	this->states = state_data->states;
	this->end = false;
	this->paused = false;
	this->keyTime = 0.f;
	this->maxKeyTime = 3.f;
	this->gridSize = state_data->gridSize;
}

State::~State()
{

}

// Accessors
const bool& State::getEnd() const
{
	return this->end;
}

const bool State::getKeyTime()
{
	if (this->keyTime >= this->maxKeyTime) {
		this->keyTime = 0.f;
		return true;
	}
	return false;
}

// Functions
void State::endState()
{
	this->end = true;
}

void State::pauseState()
{
	this->paused = true;
}

void State::unpauseState()
{
	this->paused = false;
}

void State::textBoxTypedOn(sf::Event ev)
{

}

void State::updateMousePos(sf::View* view)
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);

	if(view)
		this->window->setView(*view);

	this->mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
	this->mousePosGrid = sf::Vector2i(
		static_cast<int>(this->mousePosView.x) / static_cast<int>(this->gridSize),
		static_cast<int>(this->mousePosView.y) / static_cast<int>(this->gridSize)
	);

	this->window->setView(this->window->getDefaultView());
}

void State::updateKeyTime(const float& dt)
{
	if (this->keyTime < this->maxKeyTime)
		this->keyTime += 10.f * dt;
}