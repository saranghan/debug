#include <sstream>
#include "SplashState.hpp"
#include "DEFINITIONS.hpp"
#include "MainMenuState.hpp"
#include <iostream>

namespace MESY {
    SplashState::SplashState(GameDataRef data, const char* mode) : _data(data) {
        _mode = mode;
        if (strcmp(mode, "intro") == 0) {
            _data->assets.LoadTexture("Splash Intro1", SPLASH_INTRO1);
            _data->assets.LoadTexture("Splash Intro2", SPLASH_INTRO2);
            
            _data->assets.LoadTexture("Splash Title1", SPLASH_TITLE1);
            _data->assets.LoadTexture("Splash Title2", SPLASH_TITLE2);
            _data->assets.LoadTexture("Splash Title3", SPLASH_TITLE3);
            
            _background_intro1.setTexture(this->_data->assets.GetTexture("Splash Intro1"));
            _background_intro2.setTexture(this->_data->assets.GetTexture("Splash Intro2"));
            
            _background_tit1.setTexture(this->_data->assets.GetTexture("Splash Title1"));
            _background_tit2.setTexture(this->_data->assets.GetTexture("Splash Title2"));
            _background_tit3.setTexture(this->_data->assets.GetTexture("Splash Title3"));
            
            sf::Vector2f targetSize(620.0f, 800.0f);
            _background_intro1.setScale(targetSize.x / _background_intro1.getLocalBounds().width, targetSize.y / _background_intro1.getLocalBounds().height);
            _background_intro2.setScale(targetSize.x / _background_intro2.getLocalBounds().width, targetSize.y / _background_intro2.getLocalBounds().height);
            
            _background_intro1.setPosition(-10, 0);
            _background_intro2.setPosition(-10, 0);
            
            sf::Vector2f targetSize1((float)(1920.0f / 1.5), (float)(1280.0f / 1.5));
            _background_tit1.setScale(targetSize1.x / _background_tit1.getLocalBounds().width, targetSize1.y / _background_tit1.getLocalBounds().height);
            _background_tit2.setScale(targetSize1.x / _background_tit2.getLocalBounds().width, targetSize1.y / _background_tit2.getLocalBounds().height);
            _background_tit3.setScale(targetSize1.x / _background_tit3.getLocalBounds().width, targetSize1.y / _background_tit3.getLocalBounds().height);
            
            _background_tit1.setPosition(-315, -50);
            _background_tit2.setPosition(-315, -50);
            _background_tit3.setPosition(-315, -50);
        }
        else if (strcmp(mode, "gameOver") == 0) {
            _data->assets.LoadTexture("Game Over", GAME_OVER_SCREEN);
            _background_gameOver.setTexture(this->_data->assets.GetTexture("Game Over"));
            
            sf::Vector2f targetSize((float)(1920.0f / 1.5), (float)(1280.0f / 1.5));
            _background_gameOver.setScale(targetSize.x / _background_gameOver.getLocalBounds().width, targetSize.y / _background_gameOver.getLocalBounds().height);
            _background_gameOver.setPosition(-315, -50);
        }
        
    }
    
    void SplashState::HandleInput() {
        while(_data->window.pollEvent(event)) {
            if(sf::Event::Closed == event.type) {
                _data->window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    this->_data->machine.AddState(StateRef(new MainMenuState(_data)), true);
                }
            }
        }
    }
    
    void SplashState::Update(float dt) {
        if (this->_clock.getElapsedTime().asSeconds() > SPLASH_INTRO_SHOW_TIME) {
            this->_data->machine.AddState(StateRef(new MainMenuState(_data)), true);
        }
    }
    
    void SplashState::Draw(float dt) {
        if (strcmp(_mode, "intro") == 0) {
            _data->window.clear();
            if (this->_clock.getElapsedTime().asSeconds() > 8) {
                _data->window.draw(_background_tit3);
            }
            else if (this->_clock.getElapsedTime().asSeconds() > 7) {
                _data->window.draw(_background_tit2);
            }
            else if (this->_clock.getElapsedTime().asSeconds() > 6) {
                _data->window.draw(_background_tit1);
            }
            else if (this->_clock.getElapsedTime().asSeconds() > 4) {
                _data->window.draw(_background_intro2);
            }
            else {
                _data->window.draw(_background_intro1);
            }
        }
        else if (strcmp(_mode, "gameOver") == 0) {
            _data->window.draw(_background_gameOver);
        }
        _data->window.display();
    }
}
