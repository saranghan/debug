#include <sstream>
#include "SplashState.hpp"
#include "DEFINITIONS.hpp"
#include "MainMenuState.hpp"
#include "GameStates.hpp"
#include "HardState.hpp"
#include <iostream>

namespace MESY {
    MainMenuState::MainMenuState(GameDataRef data) : _data(data) {
        this->_data->assets.LoadTexture("Easy Button", EASY_MAINMENU_FILEPATH);
        this->_data->assets.LoadTexture("Hard Button", HARD_MAINMENU_FILEPATH);
        this->_data->assets.LoadTexture("Background", MENU_IMAGE_FILEPATH);
        
        _easy.setTexture(this->_data->assets.GetTexture("Easy Button"));
        _hard.setTexture(this->_data->assets.GetTexture("Hard Button"));
        _background.setTexture(this->_data->assets.GetTexture("Background"));
        
        sf::Vector2f targetSize((float)(1920.0f / 1.5), (float)(1280.0f / 1.5));
        sf::Vector2f targetSize1((float)(960.0f / 7), (float)(480.0f / 7));
        sf::Vector2f targetSize3((float)(550.0f / 2), (float)(413.0f / 2));
        
        _easy.setScale(targetSize1.x / _easy.getLocalBounds().width, targetSize1.y / _easy.getLocalBounds().height);
        _hard.setScale(targetSize3.x / _hard.getLocalBounds().width, targetSize3.y / _hard.getLocalBounds().height);
        
        _easy.setPosition(SCREEN_WIDTH / 2 - (_easy.getGlobalBounds().width / 2), 450);
        _hard.setPosition((SCREEN_WIDTH / 2) - (_hard.getGlobalBounds().width / 2) + 10, 500);
        
        _background.setScale(targetSize.x / _background.getLocalBounds().width, targetSize.y / _background.getLocalBounds().height);
        _background.setPosition(-315, -50);
    }
    
    void MainMenuState::HandleInput() {
        sf::Event event;
        
        while(_data->window.pollEvent(event)) {
            if(sf::Event::Closed == event.type) {
                _data->window.close();
            }
            
            if(this->_data->input.IsSpriteClicked(this->_easy, sf::Mouse::Left, this->_data->window)) {
                this->_data->machine.AddState(StateRef(new GameStates(_data, 1)),true);
            }
            
            if(this->_data->input.IsSpriteClicked(this->_hard, sf::Mouse::Left, this->_data->window)) {
                this->_data->machine.AddState(StateRef(new GameStates(_data, 2)),true);
            }
        }
    }
    
    void MainMenuState::Update(float dt) {
        
    }
    
    void MainMenuState::Draw(float dt) {
        _data->window.clear();
        _data->window.draw(_background);
        _data->window.draw(_easy);
        _data->window.draw(_hard);
        _data->window.display();
    }
}
