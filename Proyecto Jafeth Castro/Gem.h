#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;
class Gem{

private:
	int type;
	Sprite sprite;
	Texture texture;
	Vector2f currentPosition;
	Vector2f targetPosition;
	float fallSpeed;
	
public:
	Gem();
	~Gem();
	int getType();
	void setType();
	void setType(int pType);
	Sprite getSprite();
	void setSprite();
	Texture getTexture();
	void setTexture();
	Vector2f getPosition();
	void setPosition(Vector2f& pPosition);
	FloatRect getGlobalBounds();
	void generateGem();
	void setTargetPosition(const Vector2f& pTargetPosition);
	void drawGem(RenderWindow& window);
	void doMoveEffect(float frameTime);
	bool isAtTarget();
	void setGemPosition(Vector2f position);
	
	

};

