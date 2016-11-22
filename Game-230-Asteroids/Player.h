#pragma once
#include <SFML\Graphics.hpp>
#include "GameConstants.h"
#include "Spaceship.h"

using namespace sf;

class Player {
private:
	Spaceship spaceship;
public:
	Player() :spaceship(Spaceship_Size, 90, Spaceship_Thrust,Spaceship_Full_Speed,Spaceship_Angular_Speed) {
		spaceship.setSpaceshipTexture(Spaceship_Texture);
		spaceship.setSpaceshipFlameTexture(Spaceship_Engine_Flame_Texture);
		spaceship.setSpaceshipFlameSize(20, 15);
		spaceship.setSpaceshipFlamePosition(spaceship.getPosition().x,spaceship.getPosition().y);
	}
	Spaceship getSpaceship() { return spaceship; }
	void setSpaceshipPosition(float x, float y) { spaceship.setPosition(x, y); }
	void moveForward() { spaceship.moveForward(); }
	//void moveBackward() { spaceship.moveBackward(); }
	void turnLeft() { spaceship.turnLeft(); }
	void turnRight() { spaceship.turnRight(); }
	void act();
};

void Player::act() {
	spaceship.move();
}