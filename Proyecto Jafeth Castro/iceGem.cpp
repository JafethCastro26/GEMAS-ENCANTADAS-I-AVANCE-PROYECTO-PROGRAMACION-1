#include "IceGem.h"
#include <iostream>
using namespace std;

iceGem::iceGem() {
    iceStrength = 2;
    fallSpeed = 500.0f;
    currentPosition = sf::Vector2f(0, 0);
    targetPosition = sf::Vector2f(0, 0);
}

iceGem::~iceGem() {}

bool iceGem::isIce()
{
    return true;
}

void iceGem::doMoveEffect(float frameTime)
{
    return;
}

void iceGem::setIceStrength(int pIceStrength)
{
    this->iceStrength = pIceStrength;
}

int iceGem::getType() {
    return type;
}

void iceGem::setType() {
    type = 5;
}

void iceGem::weakenIce() {
    if (iceStrength > 0) iceStrength--;
    if (iceStrength == 1) {
        texture.loadFromFile("img/broken1.png");
        sprite.setTexture(texture);
    }
}

bool iceGem::isBroken() {
    return iceStrength == 0;
}

int iceGem::getIceStrength() {
    return iceStrength;
}

