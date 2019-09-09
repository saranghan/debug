#include <sstream>
#include <time.h>
#include "SplashState.hpp"
#include "DEFINITIONS.hpp"
#include "GameStates.hpp"
#include "MainMenuState.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

namespace MESY {
    GameStates::GameStates(GameDataRef data, int x) : _data(data) {
        this->_data->assets.LoadTexture("Restart Button", RESTART_FILEPATH);
        this->_data->assets.LoadTexture("Surface", DESERT_FILEPATH);
        this->_data->assets.LoadTexture("Tiles", TILES_FILEPATH);
        this->_data->assets.LoadTexture("Background", BOARD_BACKGROUND_FILEPATH);
        this->_data->assets.LoadTexture("Trump", DONALD_TRUMP);
        
        _restart.setTexture(this->_data->assets.GetTexture("Restart Button"));
        _background.setTexture(this->_data->assets.GetTexture("Background"));
        
        sf::Vector2f targetSize1(75.0f, 52.5f);
        _restart.setScale(targetSize1.x / _restart.getLocalBounds().width, targetSize1.y / _restart.getLocalBounds().height);
        _restart.setPosition(10, 10);
        //_restart.setPosition(SCREEN_WIDTH / 2 - (_restart.getGlobalBounds().width / 2), 13);
        
        sf::Vector2f targetSize((float)(1920.0f / 1.5), (float)(1280.0f / 1.5));
        _background.setScale(targetSize.x / _background.getLocalBounds().width, targetSize.y / _background.getLocalBounds().height);
        _background.setPosition(-315, -50);
        
        _player = new Player(_data);
        
        GameState = x;
        
        
        if (GameState == 1) {
            _Shape.setPointCount(4);
            
            for (int i = 1; i <= 15; i++) {
                for (int j = 1; j <= 20; j++) {
                    _grid_upper[i][j] = (rand() % 9) + 12;
                    if (rand() % 3 == 0) {
                        _grid_under[i][j] = 9;
                    }
                    else {
                        _grid_under[i][j] = 0;
                    }
                }
            }
        }else if (GameState == 2) {
            x = 0; x0 = 0; x1 = 0; x2 = 0;
            y = 0; y0 = 0; y1 = 0; y2 = 0;
            
            _Shape.setPointCount(3);
            
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
    
    void GameStates::Setup(int x) {
        //sets players position to the tile the player clicked, and makes hasChosen = true which needs to be true so player can be drawn.
        _player->setPos(x);
        
        //sets which tiles to be clear of mines in the start, now its the player's choice, if upper = 0 it will show tile, if under = 0, it will have no bomb
        _grid_upper[x][1] = 0;
        _grid_under[x][1] = 0; _grid_under[x-1][1] = 0; _grid_under[x+1][1] = 0; _grid_under[x][2] = 0; _grid_under[x-1][2] = 0; _grid_under[x+1][2] = 0;
        
        if (GameState == 2) {
            _grid_under[x - 2][1] = 0; _grid_under[x + 2][1] = 0;
            if (x % 2 != 0) {
                _grid_under[x + 2][2] = 0; _grid_under[x - 2][2] = 0;
            }
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
                if (GameState == 2) {
                    if ((i % 2 != 0 && j % 2 != 0) || (i % 2 == 0 && j % 2 == 0)) {
                        if (_grid_under[i - 2][j] == 9) n++;
                        if (_grid_under[i + 2][j] == 9) n++;
                        if (_grid_under[i - 2][j + 1] == 9) n++;
                        if (_grid_under[i + 2][j + 1] == 9) n++;
                    }
                    else if ((i % 2 == 0 && j % 2 != 0) || (i % 2 != 0 && j % 2 == 0)) {
                        if (_grid_under[i - 2][j] == 9) n++;
                        if (_grid_under[i + 2][j] == 9) n++;
                        if (_grid_under[i - 2][j - 1] == 9) n++;
                        if (_grid_under[i + 2][j - 1] == 9) n++;
                    }
                }
                _grid_under[i][j] = n;
            }
        }
    }
    
    void GameStates::HandleInput() {
        
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
                if (GameState == 2) {
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
                }
                
                if (x > 0 && x < 16 && y == 1 && !_player->PlayerChosen()) {
                    Setup(x);
                }
                if (_player->PlayerChosen()) {
                    if (event.key.code == sf::Mouse::Left) {
                        _grid_upper[x][y] = 0; //If tile is clicked, it being 0 will later mean it will display texture underneath.
                    }
                    
                    if (event.key.code == sf::Mouse::Right) { //Deals with the flag, that is why we have duplicates
                        if (_grid_under[x][y] != 11 && _grid_upper[x][y] != 0) {
                            _grid_under[x][y] = 11;
                            _grid_upper[x][y] = 0;
                            std::cout << _under_duplicate[x][y];
                        }
                        else if (_grid_under[x][y] == 11) {
                            _grid_under[x][y] = _under_duplicate[x][y];
                            _grid_upper[x][y] = _upper_duplicate[x][y];
                        }
                    }
                }
            }
            int result = 0;
            if (event.type == sf::Event::KeyPressed && !_player->PlayerMoving() && _player->PlayerChosen()) {
                if (event.key.code == sf::Keyboard::W && _grid_under[x][y] != 11 && _grid_under[x][y-1] != 11) {
                    _player->MovePlayer(0, -1, 32);
                }
                if (event.key.code == sf::Keyboard::S && _grid_under[x][y] != 11 && _grid_under[x][y+1] != 11) {
                    _player->MovePlayer(0, 1, 0);
                }
                if (event.key.code == sf::Keyboard::A && _grid_under[x][y] != 11 && _grid_under[x-1][y] != 11) {
                    _player->MovePlayer(-1, 0, 64);
                }
                if (event.key.code == sf::Keyboard::D && _grid_under[x][y] != 11 && _grid_under[x+1][y] != 11) {
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
    
    void GameStates::Update(float dt) {
        _player->Move(dt*6);
    }
    
    void GameStates::Draw(float dt) {
        _data->window.draw(_background);
        
        for (int i = 1; i <= 15; i++) {
            for (int j = 1; j <= 20; j++) {
                //If tile underneath is 9 then it is a mine, and upper grid == 0, hence will show all
                //Tiles underneath the surface texture, ending the game
                if (_grid_under[x][y] == 9 && _grid_upper[x][y] == 0) { _grid_upper[i][j] = 0; }// Restart Game
                
                if (GameState == 1) {
                    //Will draw texture underneath
                    if (_grid_upper[i][j] == 0) {
                        _Shape.setTexture(&this->_data->assets.GetTexture("Tiles"), false);
                        _Shape.setTextureRect(sf::IntRect(_grid_under[i][j] * TILE_WIDTH, 0, TILE_WIDTH, TILE_WIDTH));
                    }
                    else { //Else will just draw surface texture
                        _Shape.setTexture(&this->_data->assets.GetTexture("Surface"), false);
                        _Shape.setTextureRect(sf::IntRect(i * TILE_WIDTH, j * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH));
                    }
                    _Shape.setPoint((std::size_t)0, sf::Vector2f((float)0, (float)0));
                    _Shape.setPoint((std::size_t)1, sf::Vector2f((float)32, (float)0));
                    _Shape.setPoint((std::size_t)2, sf::Vector2f((float)32, (float)32));
                    _Shape.setPoint((std::size_t)3, sf::Vector2f((float)0, (float)32));
                }else if (GameState == 2) {
                    //If tile underneath is 9 then it is a mine, and upper grid == 0, hence will show all
                    //Tiles underneath the surface texture, ending the game
                    
                    //Will draw texture underneath
                    if (_grid_upper[i][j] == 0) {
                        /*
                         For triangle below(hidden surface texture with numbers and shit):
                         */
                        if ((i % 2 != 0 && j % 2 != 0) || (i % 2 == 0 && j % 2 == 0)) {
                            x0 = 16, y0 = 0, x1 = -16, y1 = 32, x2 = 48, y2 = 32;
                        }
                        else if ((i % 2 == 0 && j % 2 != 0 )|| (i % 2 != 0 && j % 2 == 0)) {
                            x0 = 16, y0 = 32, x1 = -16, y1 = 0, x2 = 48, y2 = 0;
                        }
                        _Shape.setTexture(&this->_data->assets.GetTexture("Tiles"), false);
                        _Shape.setTextureRect(sf::IntRect(_grid_under[i][j] * TILE_WIDTH, 0, TILE_WIDTH, TILE_WIDTH));
                    }
                    else { //Else will just draw surface texture
                        // For triangle below surface texture:
                        if (i % 2 != 0 && j % 2 != 0 || (i % 2 == 0 && j % 2 == 0)) {
                            x0 = 16, y0 = 0, x1 = -16, y1 = 32, x2 = 48, y2 = 32;
                        }
                        else if ((i % 2 == 0 && j % 2 != 0) || (i % 2 != 0 && j % 2 == 0)) {
                            x0 = 16, y0 = 32, x1 = -16, y1 = 0, x2 = 48, y2 = 0;
                        }
                        _Shape.setTexture(&this->_data->assets.GetTexture("Trump"), false);
                        _Shape.setTextureRect(sf::IntRect((i + 1) * 30 * 4, j * 32 * 4, 60 * 4, 32 * 4));
                    }
                    _Shape.setPoint((std::size_t)0, sf::Vector2f((float)(x0), (float)(y0)));
                    _Shape.setPoint((std::size_t)1, sf::Vector2f((float)(x1), (float)(y1)));
                    _Shape.setPoint((std::size_t)2, sf::Vector2f((float)(x2), (float)(y2)));
                }
                _Shape.setPosition((float)((i + 1) * TILE_WIDTH), (float)((j + 2) * TILE_WIDTH));
                _Shape.setOutlineColor(sf::Color(139, 69, 19, 200));
                _Shape.setOutlineThickness(2);
                _data->window.draw(_Shape);
            }
        }
        _data->window.draw(_restart);
        if (_player->PlayerChosen()) {
            _player->Draw();
        }
        _data->window.display();
    }
}
