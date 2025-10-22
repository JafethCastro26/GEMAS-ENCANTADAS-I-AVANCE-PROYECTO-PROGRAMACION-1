#include "normalGem.h"
#include <iostream>
using namespace std;

normalGem::normalGem() {
    type = 0;
    setTexture();
    fallSpeed = 500.0f;
    currentPosition = sf::Vector2f(0, 0);
    targetPosition = sf::Vector2f(0, 0);
}

normalGem::~normalGem() {}

int normalGem::getType() {
    return type;
}
void normalGem::setType() {
    type = rand() % 5;
}