#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game.hpp"

namespace MESY {
    class SplashState : public State {
    public:
        SplashState(GameDataRef data, const char* mode);
        void HandleInput();
        void Update(float dt);
        void Draw(float dt);
        
    private:
        const char* _mode;
        GameDataRef _data;
        sf::Clock _clock;
        sf::Event event;
        sf::Sprite _background_intro1, _background_intro2, _background_intro3, _background_gameOver;
        sf::Sprite _background_tit1, _background_tit2, _background_tit3;
    };
}
