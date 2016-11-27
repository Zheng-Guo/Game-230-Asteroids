#pragma once
#include <memory>
#include <SFML\Graphics.hpp>
#include "GameConstants.h"
#include "Spaceship.h"

using namespace sf;

class Player {
private:
	shared_ptr<Spaceship> spaceship;
	int lives;
	int score;
	int invincibilityCounter;
	bool isInvincible,isVisible;
public:
	Player():isInvincible(false),
	isVisible(true),
	invincibilityCounter(0){
		spaceship = make_shared<Spaceship>(Spaceship_Size, 90, Spaceship_Thrust, Spaceship_Full_Speed, Spaceship_Angular_Speed);
		spaceship->setSpaceshipTexture(Spaceship_Texture);
		spaceship->setSpaceshipFlameTexture(Spaceship_Engine_Flame_Texture);
		spaceship->setSpaceshipFlameSize(Spaceship_Flame_Width, Spaceship_Flame_Height);
		spaceship->setSpaceshipFlamePosition(spaceship->getPosition().x,spaceship->getPosition().y);
	}
	shared_ptr<Spaceship> getSpaceship() { return spaceship; }
	void setSpaceshipPosition(float x, float y) { spaceship->setPosition(x, y); }
	void setSpaceshipRotation(float r) { spaceship->setSpaceshipRotation(r); }
	void moveForward() { if(!spaceship->getIsHit()) spaceship->moveForward(); }
	//void moveBackward() { spaceship.moveBackward(); }
	void turnLeft() { if (!spaceship->getIsHit()) spaceship->turnLeft(); }
	void turnRight() { if (!spaceship->getIsHit()) spaceship->turnRight(); }
	void setLives(int l) { lives = l; }
	int getLives() { return lives; }
	void loseLife() { lives--; spaceship->setIsHit(true); spaceship->setVelocity(Vector2f(0,0)); isInvincible = true; }
	void setScore(int s) { score = s; }
	int getScore() { return score; }
	void act();
	void explode() { spaceship->explode(); }
	bool isSpaceshipHit() { return spaceship->getIsHit(); }
	void setIsInvincible(bool i) { isInvincible = i; }
	bool getIsInvincible() { return isInvincible; }
	void reset() { isInvincible = false; invincibilityCounter = 0; spaceship->reset(); spaceship->setSpaceshipRotation(90); }
	bool isSpaceshipVisible() { return isVisible; }
	bool isNextLifeUsed() { return lives>0&&!spaceship->getIsHit() && isInvincible; }
	bool isGameOver() { return lives <= 0; }
	void prepareForBattle();
};

void Player::act() {
	spaceship->move();
}

void Player::prepareForBattle() {
	if (invincibilityCounter < Spaceship_Invinciblility_Duration&&invincibilityCounter%Refresh_Frequency < Refresh_Frequency / 4)
		isVisible = true;
	else if (invincibilityCounter < Spaceship_Invinciblility_Duration&&invincibilityCounter%Refresh_Frequency < Refresh_Frequency/2)
		isVisible = false;
	else if (invincibilityCounter < Spaceship_Invinciblility_Duration&&invincibilityCounter%Refresh_Frequency < Refresh_Frequency / 4 * 3)
		isVisible = true;
	else if (invincibilityCounter < Spaceship_Invinciblility_Duration&&invincibilityCounter%Refresh_Frequency < Refresh_Frequency)
		isVisible = false;
	invincibilityCounter++;
	if(invincibilityCounter>=Spaceship_Invinciblility_Duration){
		isVisible = true;
		invincibilityCounter = 0;
		isInvincible = false;
	}
}