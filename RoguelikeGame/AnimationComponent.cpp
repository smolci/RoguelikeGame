#include "stdafx.h"
#include "AnimationComponent.h"

//Constructor / Desctructor
AnimationComponent::AnimationComponent(sf::Sprite& sprite, sf::Texture& textureSheet)
	:sprite(sprite), textureSheet(textureSheet)
{

}

AnimationComponent::~AnimationComponent()
{
	for (auto& i : this->animations) {
		delete i.second;
	}
}

//Functions
void AnimationComponent::addAnimation(
	const std::string key,
	float animationTimer,
	int start_frame_x, int start_frame_y,
	int frames_x, int frames_y,
	int width, int height
){
	this->animations[key] = new Animation(
		this->sprite, this->textureSheet, 
		animationTimer,
		start_frame_x, start_frame_y,
		frames_x, frames_y,
		width, height
	);
}

void AnimationComponent::play(const std::string key, const float& dt)
{
	this->animations[key]->play(dt);
}