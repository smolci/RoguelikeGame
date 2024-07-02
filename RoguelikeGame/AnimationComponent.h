#pragma once

class AnimationComponent
{
private:
	class Animation
	{
	public:
		sf::Sprite& sprite;
		sf::Texture& textureSheet;
		float animationTimer;
		float timer;
		int width;
		int height;
		sf::IntRect startRect;
		sf::IntRect currRect;
		sf::IntRect endRect;

		//Constructor / Destructor
		Animation(sf::Sprite& sprite, sf::Texture& textureSheet,
			float animationTimer,
			int start_frame_x, int start_frame_y, int frames_x, int frames_y,
			int width, int height
		) : sprite(sprite), textureSheet(textureSheet), animationTimer(animationTimer), width(width), height(height)
		{
			this->timer = 0.f;
			this->startRect = sf::IntRect(start_frame_x * width, start_frame_y * height, width, height);
			this->currRect = this->startRect;
			this->endRect = sf::IntRect(frames_x * width, frames_y * height, width, height);
		
			this->sprite.setTexture(this->textureSheet, true);
			this->sprite.setTextureRect(this->startRect);
		}

		//Functions
		void play(const float& dt) 
		{
			//Update timer
			this->timer += 125.f * dt;
			if (this->timer >= this->animationTimer)
			{
				this->timer = 0.f; // Reset

				//Animate
				if (this->currRect != this->endRect)
				{
					this->currRect.left += this->width;
				}
				else //Reset the animation
				{
					this->currRect.left = this->startRect.left;
				}
				this->sprite.setTextureRect(this->currRect);
			}
		}

		void reset()
		{
			this->timer = 0.f;
			this->currRect = this->startRect;
		}
	};

	sf::Sprite& sprite;
	sf::Texture& textureSheet;
	std::map<std::string, Animation*> animations;

public:
	//Constructor / Desctructor
	AnimationComponent(sf::Sprite& sprite, sf::Texture& textureSheet);
	virtual ~AnimationComponent();

	//Functions
	void addAnimation(const std::string key,
		float animationTimer,
		int start_frame_x, int start_frame_y,
		int frames_x, int frames_y,
		int width, int height
	);
	
	void play(const std::string key, const float& dt);
};