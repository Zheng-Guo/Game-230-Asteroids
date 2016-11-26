#pragma once
#include <SFML\Graphics.hpp>
#include "GameConstants.h"
#include "Spaceship.h"

using namespace sf;

class Player {
private:
	Spaceship spaceship;
	int lives;
	int score;
public:
	Player() :spaceship(Spaceship_Size, 90, Spaceship_Thrust,Spaceship_Full_Speed,Spaceship_Angular_Speed) {
		spaceship.setSpaceshipTexture(Spaceship_Texture);
		spaceship.setSpaceshipFlameTexture(Spaceship_Engine_Flame_Texture);
		spaceship.setSpaceshipFlameSize(Spaceship_Flame_Width, Spaceship_Flame_Height);
		spaceship.setSpaceshipFlamePosition(spaceship.getPosition().x,spaceship.getPosition().y);
	}
	Spaceship& getSpaceship() { return spaceship; }
	void setSpaceshipPosition(float x, float y) { spaceship.setPosition(x, y); }
	void moveForward() { if(!spaceship.getIsHit()) spaceship.moveForward(); }
	//void moveBackward() { spaceship.moveBackward(); }
	void turnLeft() { if (!spaceship.getIsHit()) spaceship.turnLeft(); }
	void turnRight() { if (!spaceship.getIsHit()) spaceship.turnRight(); }
	void setLives(int l) { lives = l; }
	int getLives() { return lives; }
	void loseLife() { lives--; spaceship.setIsHit(true); }
	void setScore(int s) { score = s; }
	int getScore() { return score; }
	void act();
	void explode() { spaceship.explode(); }
};

void Player::act() {
	spaceship.move();
}