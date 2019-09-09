#include "Game.hpp"
#include "iostream"
#include "DEFINITIONS.hpp"
#include "SplashState.hpp"
#include "Player.hpp"
#include <cmath>

namespace MESY {
    
    Player::Player(GameDataRef data) : _data(data) {
        
        sf::Texture texture;
        sf::IntRect rectSourceSprite(300, 0, 123, 123);
        
        this->_data->assets.LoadTexture("Player", PLAYER_WALKING);
        this->_data->assets.LoadTexture("Explosion", EXPLOSION_FILEPATH);
        
        playerSprite.setTexture(this->_data->assets.GetTexture("Player"));
        playerSprite.setTextureRect(sf::IntRect(0, 0, 24, 32));
        explosionSprite.setTexture(this->_data->assets.GetTexture("Explosion"));
        explosionSprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
        
        isMoving = false;
        chosenStart = false;
        isExplosion = false;
        spriteShown = false;
        
        xPosition = 0;
        yPosition = 0;
        offsetX = 0;
        offsetY = 0;
        
        playerSprite.setPosition((float)((5) * TILE_WIDTH + 5), (float)((4) * TILE_WIDTH));
    }
    
    bool Player::PlayerMoving() {
        return isMoving;
    }
    
    bool Player::PlayerChosen() {
        return chosenStart;
    }
    
    void Player::setPos(int x) {
        chosenStart = true;
        xPosition = x;
        yPosition = 1;
        playerSprite.setPosition((float)((xPosition + 1) * TILE_WIDTH + 5), (float)(0) * TILE_WIDTH);
        offsetY = 3 * TILE_WIDTH;
        isMoving = true;
        originalPosition = playerSprite.getPosition();
        dtModifier = 12;
    }
    
    sf::Vector2i Player::GetPos()
    {
        return sf::Vector2i(xPosition, yPosition);
    }
    
    void Player::Explode(int newX, int newY) {
        isExplosion = true;
        explosionSprite.setPosition((float)(((newX + 1) * TILE_WIDTH) - (32 / 2)), (float)(((newY + 2) * TILE_WIDTH) - (32 / 2)));
    }
    
    void Player::MovePlayer(int newX, int newY, int spriteShown) {
        bool isValid = false;
        if (newY == -1 && yPosition > 1) {
            isValid = true;
        }
        if (newY == 1 && yPosition < BOARD_HEIGHT) {
            isValid = true;
        }
        if (newX == -1 && xPosition > 1) {
            isValid = true;
        }
        if (newX == 1 && xPosition < BOARD_WIDTH) {
            isValid = true;
        }
        if(isValid){
            yPosition += newY;
            xPosition += newX;
            this->spriteShown = spriteShown;
            isMoving = true;
            offsetX = newX * TILE_WIDTH;
            offsetY = newY * TILE_WIDTH;
            originalPosition = playerSprite.getPosition();
            dtModifier = 6;
        }
    }
    
    void Player::Draw() {
        if (!isDead) {
            this->_data->window.draw(playerSprite);
        }
        if (isExplosion) {
            this->_data->window.draw(explosionSprite);
        }
    }
    
    void Player::Move(float dt) {
        sf::Vector2f currentPos = playerSprite.getPosition();
        sf::Vector2f targetPos = originalPosition + sf::Vector2f((float)offsetX, (float)offsetY);
        float distance = (sqrt(abs(((targetPos.x - currentPos.x) * (targetPos.x - currentPos.x)) + ((targetPos.y - currentPos.y) * (targetPos.y - currentPos.y))))); // Pythagoras to find vector difference
        if (isMoving && distance >= 0.2f) {
            playerSprite.move((offsetX/2) * (dt / dtModifier), (offsetY/2) * (dt / dtModifier));
            playerSprite.setTextureRect(sf::IntRect((int)counterWalking * 24, spriteShown, 24, 32));
            counterWalking += 0.5;
            if (counterWalking > 7) {
                counterWalking = 0;
            }
        } else {
            isMoving = false;
        }
        
        if (isExplosion) {
            isDead = true;
            explosionSprite.setTextureRect(sf::IntRect((int)explosionTexture * 64, (int)explosionTextureVertical * 64, 64, 64));
            explosionTexture += 0.4f;
            if (explosionTexture > 4) {
                explosionTexture = 0;
                explosionTextureVertical += 1;
            }
            if (explosionTextureVertical > 4) {
                isExplosion = false;
                _data->machine.AddState(StateRef(new SplashState(this->_data, "gameOver")));
            }
        }
    }
}
