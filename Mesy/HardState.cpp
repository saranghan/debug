
#include <sstream>
#include "DEFINITIONS.hpp"
#include "HardState.hpp"
#include "MainMenuState.hpp"

#include <iostream>

namespace MESY {
    HardState::HardState(GameDataRef data) : _data(data) {
        this->_data->assets.LoadTexture("Restart Button", RESTART_FILEPATH);
        this->_data->assets.LoadTexture("Background", BOARD_BACKGROUND_FILEPATH);
        this->_data->assets.LoadTexture("Tiles", TILES_FILEPATH);
        this->_data->assets.LoadTexture("Trump", DONALD_TRUMP);
        
        _restart.setTexture(this->_data->assets.GetTexture("Restart Button"));
        _background.setTexture(this->_data->assets.GetTexture("Background"));
        
        sf::Vector2f targetSize1(100.0f, 70.0f);
        sf::Vector2f targetSize((float)(1920.0f / 1.5), (float)(1280.0f / 1.5));
        
        _restart.setScale(targetSize1.x / _restart.getLocalBounds().width, targetSize1.y / _restart.getLocalBounds().height);
        _background.setScale(targetSize.x / _background.getLocalBounds().width, targetSize.y / _background.getLocalBounds().height);
        
        _restart.setPosition(SCREEN_WIDTH / 2 - (_restart.getGlobalBounds().width / 2), 13);
        _background.setPosition(-315, -50);
        
        _player = new Player(_data);
        
        x = 0; x0 = 0; x1 = 0; x2 = 0;
        y = 0; y0 = 0; y1 = 0; y2 = 0;
        
        _triangle.setPointCount(3);
        
        for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 20; j++) {
                //if 1 draw surface, if 0 draw under
                _grid_upper[i][j] = 1;
                //if 9, then its a bomb
                if (rand() % 1 == 0) {
                    _grid_under[i][j] = 9;
                }
            }
        }
        
        
        for (int i = 0; i < 17; i++) {
            for (int j = 0; j < 22; j++) {
                _under_duplicate[i][j] = _grid_under[i][j];
            }
        }
        
        for (int i = 0; i < 17; i++) {
            for (int j = 0; j < 22; j++) {
                _upper_duplicate[i][j] = _grid_upper[i][j];
            }
        }
    }
    
    void HardState::Setup(int x) {
        _player->setPos(x);
        
        _grid_upper[x][1] = 0;
        _grid_under[x][1] = 0; _grid_under[x - 1][1] = 0; _grid_under[x + 1][1] = 0; _grid_under[x][2] = 0;
        _grid_under[x - 1][2] = 0; _grid_under[x + 1][2] = 0; _grid_under[x - 2][1] = 0; _grid_under[x + 2][1] = 0;
        if (x % 2 != 0) {
            _grid_under[x + 2][2] = 0; _grid_under[x - 2][2] = 0;
        }
        /*
         This calculates how many mines are beside the current tile,
         giving the tile the proper number.
         */
        for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 20; j++) {
                int n = 0;
                if (_grid_under[i][j] == 9) continue;
                if (_grid_under[i + 1][j] == 9) n++;
                if (_grid_under[i][j + 1] == 9) n++;
                if (_grid_under[i - 1][j] == 9) n++;
                if (_grid_under[i][j - 1] == 9) n++;
                if (_grid_under[i + 1][j + 1] == 9) n++;
                if (_grid_under[i - 1][j - 1] == 9) n++;
                if (_grid_under[i - 1][j + 1] == 9) n++;
                if (_grid_under[i + 1][j - 1] == 9) n++;
                /*
                 Below are needed for the triangle minesweeper
                 Triangle minesweeper has more blocks connected
                 to each individual block so we need to change the number
                 presented in each triangle
                 */
                if (i % 2 != 0 && j % 2 != 0 || i % 2 == 0 && j % 2 == 0) {
                    if (_grid_under[i - 2][j] == 9) n++;
                    if (_grid_under[i + 2][j] == 9) n++;
                    if (_grid_under[i - 2][j + 1] == 9) n++;
                    if (_grid_under[i + 2][j + 1] == 9) n++;
                }
                else if (i % 2 == 0 && j % 2 != 0 || i % 2 != 0 && j % 2 == 0) {
                    if (_grid_under[i - 2][j] == 9) n++;
                    if (_grid_under[i + 2][j] == 9) n++;
                    if (_grid_under[i - 2][j - 1] == 9) n++;
                    if (_grid_under[i + 2][j - 1] == 9) n++;
                }
                _grid_under[i][j] = n;
            }
        }
    }
    
    
    void HardState::HandleInput() {
        pos = this->_data->input.GetMousePosition(this->_data->window);
        
        while (_data->window.pollEvent(event)) {
            if (sf::Event::Closed == event.type) {
                _data->window.close();
            }
            
            if (this->_data->input.IsSpriteClicked(this->_restart, sf::Mouse::Left, this->_data->window)) {
                this->_data->machine.AddState(StateRef(new MainMenuState(_data)), true);
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                
                x = (pos.x - TILE_WIDTH) / (TILE_WIDTH);
                y = (pos.y - 2 * TILE_WIDTH) / (TILE_WIDTH);
                /*
                 Had to use a function for the lines in triangles since .getGLobalBounds takes the outer most edges and is a square and doesn't take into account the shape of sprites/shapes
                 */
                if (y % 2 != 0) {
                    if (x % 2 != 0 && pos.y < TILE_WIDTH * (3.5 + y - 1)) {
                        if (pos.x > (x + 1.5) * TILE_WIDTH && ((long double)gradient * pos.y + ((x - (1.5 + y - 1)) * TILE_WIDTH)) < pos.x) {
                            x += 1;
                        }
                        else if (pos.x <= (x + 1.5) * TILE_WIDTH && ((long double)(-gradient) * pos.y + ((x + (4.5 + y - 1)) * TILE_WIDTH)) > pos.x) {
                            x -= 1;
                        }
                    }
                    else if (x % 2 == 0 && pos.y >= TILE_WIDTH * (3.5 + y - 1)) {
                        if (pos.x < (x + 1.5) * TILE_WIDTH && ((long double)gradient * pos.y + ((x - (2.5 + y - 1)) * TILE_WIDTH)) > pos.x) {
                            x -= 1;
                        }
                        else if (pos.x >= (x + 1.5) * TILE_WIDTH && ((long double)(-gradient) * pos.y + ((x + (5.5 + y - 1)) * TILE_WIDTH)) < pos.x) {
                            x += 1;
                        }
                    }
                }
                else {
                    if (x % 2 != 0 && pos.y > TILE_WIDTH * (4.5 + y - 2)) {
                        if (pos.x > (x + 1.5) * TILE_WIDTH && ((long double)(-gradient) * pos.y + ((x + (6.5 + y - 2)) * TILE_WIDTH)) < pos.x) {
                            x += 1;
                        }
                        else if (pos.x <= (x + 1.5) * TILE_WIDTH && ((long double)gradient * pos.y + ((x - (3.5 + y - 2)) * TILE_WIDTH)) > pos.x) {
                            x -= 1;
                        }
                    }
                    else if (x % 2 == 0 && pos.y <= TILE_WIDTH * (4.5 + y - 2)) {
                        if (pos.x < (x + 1.5) * TILE_WIDTH && ((long double)(-gradient) * pos.y + ((x + (5.5 + y - 2)) * TILE_WIDTH)) > pos.x) {
                            x -= 1;
                        }
                        else if (pos.x >= (x + 1.5) * TILE_WIDTH && ((long double)(gradient)* pos.y + ((x - (2.5 + y - 2)) * TILE_WIDTH)) < pos.x) {
                            x += 1;
                        }
                    }
                }
                if (x > 0 && x < 16 && y == 1 && !_player->PlayerChosen()) {
                    Setup(x);
                }
                if (_player->PlayerChosen()) {
                    if (event.key.code == sf::Mouse::Left) {
                        _grid_upper[x][y] = 0; //If tile is clicked, it being 0 will later mean it will display texture underneath.
                    }
                    
                    if (event.key.code == sf::Mouse::Right) { //Deals with the flag, that is why we have duplicates
                        if (_grid_under[x][y] != 11) {
                            _grid_under[x][y] = 11;
                        } else {
                            _grid_under[x][y] = _under_duplicate[x][y];
                            _grid_upper[x][y] = _upper_duplicate[x][y];
                        }
                        _grid_upper[x][y] = 0;
                    }
                }
            }
            int result = 0;
            if (event.type == sf::Event::KeyPressed && !_player->PlayerMoving() && _player->PlayerChosen()) {
                if (event.key.code == sf::Keyboard::W) {
                    _player->MovePlayer(0, -1, 32);
                }
                if (event.key.code == sf::Keyboard::S) {
                    _player->MovePlayer(0, 1, 0);
                }
                if (event.key.code == sf::Keyboard::A) {
                    _player->MovePlayer(-1, 0, 64);
                }
                if (event.key.code == sf::Keyboard::D) {
                    _player->MovePlayer(1, 0, 96);
                }
                sf::Vector2i currentPos = _player->GetPos();
                x = currentPos.x;
                y = currentPos.y;
                _grid_upper[x][y] = 0;
                if (_grid_under[x][y] == 9) { _player->Explode(x, y); }
            }
        }
    }
    void HardState::Update(float dt){
        _player->Move(dt*5);
    }
    void HardState::Draw(float dt){
        _data->window.draw(_background);
        
        for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 20; j++) {
                //If tile underneath is 9 then it is a mine, and upper grid == 0, hence will show all
                //Tiles underneath the surface texture, ending the game
                if (_grid_under[x][y] == 9 && _grid_upper[x][y] == 0) { _grid_upper[i][j] = 0; }
                
                //Will draw texture underneath
                if (_grid_upper[i][j] == 0){
                    /*
                     For triangle below(hidden surface texture with numbers and shit):
                     */
                    if (i % 2 != 0 && j % 2 != 0 || i % 2 == 0 && j % 2 == 0) {
                        x0 = 16, y0 = 0, x1 = -16, y1 = 32, x2 = 48, y2 = 32;
                    }
                    else if (i % 2 == 0 && j % 2 != 0 || i % 2 != 0 && j % 2 == 0) {
                        x0 = 16, y0 = 32, x1 = -16, y1 = 0, x2 = 48, y2 = 0;
                    }
                    _triangle.setTexture(&this->_data->assets.GetTexture("Tiles"), false);
                    _triangle.setTextureRect(sf::IntRect(_grid_under[i][j] * TILE_WIDTH, 0, TILE_WIDTH, TILE_WIDTH));
                } else { //Else will just draw surface texture
                    // For triangle below surface texture:
                    if (i % 2 != 0 && j % 2 != 0 || i % 2 == 0 && j % 2 == 0) {
                        x0 = 16, y0 = 0, x1 = -16, y1 = 32, x2 = 48, y2 = 32;
                    }
                    else if (i % 2 == 0 && j % 2 != 0 || i % 2 != 0 && j % 2 == 0) {
                        x0 = 16, y0 = 32, x1 = -16, y1 = 0, x2 = 48, y2 = 0;
                    }
                    _triangle.setTexture(&this->_data->assets.GetTexture("Trump"), false);
                    _triangle.setTextureRect(sf::IntRect((i + 1) * 30 * 4, j * 32 * 4, 60 * 4, 32 * 4));
                }
                _triangle.setPoint((std::size_t)0, sf::Vector2f((float)(x0), (float)(y0)));
                _triangle.setPoint((std::size_t)1, sf::Vector2f((float)(x1), (float)(y1)));
                _triangle.setPoint((std::size_t)2, sf::Vector2f((float)(x2), (float)(y2)));
                _triangle.setPosition((float)((i + 1) * TILE_WIDTH), (float)((j + 2) * TILE_WIDTH));
                _triangle.setOutlineColor(sf::Color(139, 69, 19, 200));
                _triangle.setOutlineThickness(2);
                _data->window.draw(_triangle);
            }
        }
        _data->window.draw(_restart);
        if (_player->PlayerChosen()) {
            _player->Draw();
        }
        _data->window.display();
    }
}
