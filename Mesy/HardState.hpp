#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game.hpp"
#include "Player.hpp"

namespace MESY {
    class HardState : public State {
    public:
        HardState(GameDataRef data);
        void HandleInput();
        void Update(float dt);
        void Draw(float dt);
        void Setup(int x);
        
    private:
        GameDataRef _data;
        sf::Clock _clock;
        sf::Event event;
        sf::Vector2i pos;
        sf::ConvexShape _triangle;
        sf::Sprite _restart, _background;
        int _grid_under[17][22], _grid_upper[17][22], _under_duplicate[17][22], _upper_duplicate[17][22];
        int x, y, x0, x1, x2, y0, y1, y2;;
        double gradient = 48 / 32;
        sf::Texture _under, _upper;
        Player* _player;
    };
}
