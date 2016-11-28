#pragma once
#include <SFML\Graphics.hpp>
#include "GameConstants.h"
#include "Player.h"

using namespace sf;

class PowerUp :public CircleShape {
private:
	static Texture textures[];
	PowerUpType powerUpType;
	bool isVisible;
	bool isExpired;
	int expiringCounter;
public:
	PowerUp(float r = 0,PowerUpType p=PowerUpType::LifePack) :CircleShape(r),
	isVisible(true),
	isExpired(false),
	expiringCounter(0),
	powerUpType(p){
		setOrigin(r, r);
		switch (powerUpType) {
		case PowerUpType::LifePack:setTexture(&textures[0]); break;
		case PowerUpType::MissilePack:setTexture(&textures[1]); break;
		}
	}
	void setVisible(bool t) { isVisible = t; }
	bool getVisible() { return isVisible; }
	void setExpired(bool t) { isExpired = t; }
	bool getExpired() { return isExpired; }
	void expiring();
	int interact(Player &p);
	void shift(Vector2f v) { CircleShape::move(v); }
	static void loadTextures();
};

Texture PowerUp::textures[2];

void PowerUp::loadTextures() {
	textures[0].loadFromFile(Power_Up_Life_Pack_Texture);
	textures[1].loadFromFile(Power_Up_Missile_Pack_Texture);
}

void PowerUp::expiring() {
	if (expiringCounter >= Refresh_Frequency * 3 && expiringCounter < Refresh_Frequency / 2 * 7)
		isVisible = false;
	else if (expiringCounter >= Refresh_Frequency/2*7 && expiringCounter < Refresh_Frequency*4)
		isVisible = true;
	else if (expiringCounter >= Refresh_Frequency*4 && expiringCounter < Refresh_Frequency/2*9)
		isVisible = false;
	else if (expiringCounter >= Refresh_Frequency / 2 * 9 && expiringCounter < Refresh_Frequency * 5)
		isVisible = true;
	else if (expiringCounter >= Power_Up_Duration) {
		isVisible = false;
		isExpired = true;
	}
	++expiringCounter;
}

int PowerUp::interact(Player &p) {
	Vector2f offset = getPosition() - p.getSpaceship()->getPosition();
	float distance=sqrt(offset.x*offset.x + offset.y*offset.y);
	if (distance < getRadius() + p.getSpaceship()->getRadius()) {
		p.addLife();
		isExpired = true;
		return 1;
	}
	return 0;
}