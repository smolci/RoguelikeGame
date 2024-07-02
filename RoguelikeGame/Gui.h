#pragma once

enum button_states{BTN_IDLE = 0, BTN_HOVER, BTN_ACTIVE};

namespace gui 
{
	class Button
	{
	private:
		short unsigned buttonState;
		short unsigned id;

		sf::RectangleShape shape;
		sf::Font* font;
		sf::Text text;

		sf::Color textIdleColor;
		sf::Color textHoverColor;
		sf::Color textActiveColor;

		sf::Color idleColor;
		sf::Color hoverColor;
		sf::Color activeColor;

		sf::Color outlineIdleColor;
		sf::Color outlineHoverColor;
		sf::Color outlineActiveColor;


	public:
		Button(
			float x, float y, float width, float height, sf::Font* font, std::string text, unsigned int character_size,
			sf::Color text_idle_color, sf::Color text_hover_color, sf::Color text_active_color,
			sf::Color idle_color, sf::Color hover_color, sf::Color active_color,
			sf::Color outline_idle_color = sf::Color::Transparent,
			sf::Color outline_hover_color = sf::Color::Transparent,
			sf::Color outline_active_color = sf::Color::Transparent,
			short unsigned id = 0
		);
		~Button();

		//Accessors
		const bool isPressed() const;
		const std::string getText() const;
		const short unsigned& getId() const;

		//Modifiers
		void setText(const std::string text);
		void setId(const short unsigned id);
		void setPosition(const float x, const float y);

		//Functions
		void update(const sf::Vector2i& mousePosWindow);
		void render(sf::RenderTarget& target);
	};

	class Checkbox {
	private:
		sf::Texture checkmarkTexture;
		sf::Sprite checkmark;
		bool checked;

	public:
		Checkbox(float x, float y, float size, const std::string& texturePath, bool fullscreen);

		bool isClicked(const sf::Vector2f& mousePos);
		void toggle();
		bool isChecked() const;
		void render(sf::RenderTarget& target);
	};

	class DropDownMenu 
	{
	private:
		float keyTime, maxKeyTime, x, y, width, height;

		sf::Font& font;
		gui::Button* active;
		std::vector<gui::Button*> menu;
		bool showMenu;

	public:
		DropDownMenu(float x, float y, float width, float height, sf::Font& font, std::string list[], unsigned numOfEle, const unsigned default_index);
		~DropDownMenu();

		const unsigned short& getActiveElementId() const;

		const bool getKeyTime();
		void updateKeyTime(const float& dt);
		void update(const sf::Vector2i& mousePosWindow, const float& dt);
		void render(sf::RenderTarget& target);
	};

	class TextureSelector
	{
	private:
		float keyTime;
		const float maxKeyTime;
		float gridSize;
		bool active;
		bool hidden;
		Button* hideTexSel;
		sf::RectangleShape bounds;
		sf::Sprite sheet;
		sf::RectangleShape selector;
		sf::Vector2u mousePosGrid;
		sf::IntRect textureRect;

	public:
		TextureSelector(float x, float y, float width, float height, 
			float gridSize, const sf::Texture* texture_sheet, sf::Font& font, std::string text);
		~TextureSelector();

		//Accessors
		const bool& getActive() const;
		const sf::IntRect& getTextureRect() const;

		//Functions
		const bool getKeyTime();
		void updateKeyTime(const float& dt);
		void update(const sf::Vector2i& mousePosWindow, const float& dt);
		void render(sf::RenderTarget& target);
	};

	class HealthBar
	{
	private:
		sf::RectangleShape background;
		sf::RectangleShape bar;
		sf::Vector2f position;
		float maxWidth;
	public:
		HealthBar(float width, float height, const sf::Color& backgroundColor, const sf::Color& barColor, const sf::Vector2f& position);
		virtual ~HealthBar();

		void setPercantage(float percentage);
		void setPosition(const sf::Vector2f& pos);
		void render(sf::RenderTarget& target);
	};

	class TextBox {
	private:
		sf::Text textBox;
		std::ostringstream text;
		bool isSelected = false;
		bool hasLimit = false;
		int limit;

		void inputLogic(int charTyped);
		void deleteLastChar();
	public:
		const static int DELETE_KEY = 8;
		const static int ENTER_KEY = 13;
		const static int ESCAPE_KEY = 27;

		TextBox();
		TextBox(int size, sf::Color color, bool selected);
		void setFont(sf::Font& font);
		void setPosition(sf::Vector2f pos);
		void setLimit(bool value);
		void setLimit(bool value, int limit);
		void setSelected(bool sel);
		std::string getText();
		void clearText();
		void typedOn(sf::Event input);
		void render(sf::RenderTarget& target);
	};
}