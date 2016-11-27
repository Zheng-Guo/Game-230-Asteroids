#pragma once
#include <SFML\Graphics.hpp>
#include "GameConstants.h"

using namespace sf;

class GunShot:public CircleShape {
private:
	static Texture texture;
	float speed;
	Vector2f velocity;
	bool fired;
public:
	GunShot(float size=0,float s=0) :CircleShape(size),
	speed(s),
	fired(false){
		setOrigin(size, size);
		setTexture(&texture);
	}
	static void loadTexture();
	void setDirection(float r) { setRotation(r); velocity = Vector2f(-speed*cos(r*Degree_To_Radian), -speed*sin(r*Degree_To_Radian)); }
	void setFired(bool t) { fired = t; }
	bool getFired() { return fired; }
	void move() { CircleShape::move(velocity); }
	void shift(Vector2f v) { CircleShape::move(v); }
};

Texture GunShot::texture;

void GunShot::loadTexture() {
	texture.loadFromFile(Gun_Shot_Texutre);
}