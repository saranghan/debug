#include "InputManager.hpp"

namespace MESY {
    bool InputManager::IsSpriteClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow &window) {
        if (sf::Mouse::isButtonPressed(button)){
            if (object.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                return true;
            }
        }
        return false;
    }
    
    sf::Vector2i InputManager::GetMousePosition(sf::RenderWindow &window) {
        return sf::Mouse::getPosition(window);
    }
}
