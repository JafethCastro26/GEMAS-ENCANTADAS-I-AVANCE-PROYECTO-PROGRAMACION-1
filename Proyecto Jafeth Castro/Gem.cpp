#include "Gem.h"
#include <iostream>
using namespace std;
using namespace sf;

Gem::Gem() {
	type = 0; 
	if (!texture.loadFromFile("img/ERROR_GEM.jpg")) { 
		cout << "ERROR AL CARGAR LA IMAGEN";
	}
	fallSpeed = 0.5f;
	currentPosition = Vector2f(0,0);
	targetPosition = Vector2f(0, 0);

}

Gem::~Gem() {};

bool Gem::isIce()
{
	return false;
}

bool Gem::isBomb()
{
	return false;
}

void Gem::setType(int pType) {
	type = pType;
}
Sprite Gem::getSprite() {
	return sprite;
}
void Gem::setSprite() {
	sprite.setTexture(texture);
}
Texture Gem::getTexture() {
	return texture;
}
void Gem::setTexture() {
	string path = "img/gem" + to_string(type) + ".png";
	if (!texture.loadFromFile(path)) {
		cout << "ERROR al cargar textura para tipo " << type << endl;
		texture.loadFromFile("img/ERROR_GEM.jpg"); 
	}
	sprite.setTexture(texture);

}
Vector2f Gem::getPosition(){
	return sprite.getPosition();
}

void Gem::setPosition(Vector2f& pPosition) {
	sprite.setPosition(pPosition);
}

FloatRect Gem::getGlobalBounds() {
	return sprite.getGlobalBounds();
}

void Gem::setTargetPosition(const Vector2f& pTargetPosition) {
	targetPosition = pTargetPosition;
}

void Gem::generateGem() {
	setType();
	setTexture();
	setSprite();
	
}

void Gem::drawGem(RenderWindow& window) {
	window.draw(sprite);
}


void Gem::doMoveEffect(float frameTime) {
	Vector2f direction;
	float distance, move;
	if (currentPosition != targetPosition) {
	    direction = targetPosition - currentPosition;
	    distance = sqrt(direction.x * direction.x + direction.y * direction.y);
	    move = fallSpeed * frameTime;
		if (distance <= move) {
			currentPosition = targetPosition;
		}
		else {
			currentPosition += (direction / distance) * move;
		}
		sprite.setPosition(currentPosition);
	}
}

bool Gem::isAtTarget() {
	return currentPosition == targetPosition;
}

void Gem::setGemPosition(Vector2f position) {
	currentPosition = position;
	targetPosition = position;
	sprite.setPosition(position);


}


