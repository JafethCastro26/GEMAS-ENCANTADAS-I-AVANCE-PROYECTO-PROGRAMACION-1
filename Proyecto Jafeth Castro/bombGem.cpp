#include "bombGem.h"

bombGem::bombGem()
{
    type = 6;
    fallSpeed = 500.0f;
    currentPosition = sf::Vector2f(0, 0);
    targetPosition = sf::Vector2f(0, 0);
}


int bombGem::getType()
{
    return type;
}

void bombGem::setType()
{
    type = 6;
}

bool bombGem::isBomb()
{
    if (type == 6) {
        return true;
    }
    return false;
}
