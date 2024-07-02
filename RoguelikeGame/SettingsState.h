#pragma once
#include "State.h"
#include "Gui.h"

class SettingsState :
    public State
{
private:
    //Variables
    sf::Texture bgTexture;
    sf::RectangleShape background;
    sf::Font font;

    unsigned int number_of_buttons;
    std::map<std::string, gui::Button*> buttons;
    std::map<std::string, gui::DropDownMenu*> dropDownMenu;
    gui::Checkbox* checkbox;

    sf::Text optionsText;

    std::vector<sf::VideoMode> modes;

    //Initializers
    void initVariables();
    void initBackground();
    void initFonts();
    void initKeybinds();
    void initGui();
    void initText();

public:
    SettingsState(StateData* state_data);
    virtual ~SettingsState();

    //Functions
    void updateInput(const float& dt);
    void updateGui(const float& dt);
    void update(const float& dt);
    void renderGui(sf::RenderTarget& target);
    void render(sf::RenderTarget* target = nullptr);
};