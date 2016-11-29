#pragma once
#include <memory>
#include <SFML\Graphics.hpp>
#include "GameConstants.h"
#include "Spaceship.h"

using namespace sf;
using namespace std;

class AIPlayer {
private:
	shared_ptr<Spaceship> spaceship;
	int score;
	bool isEngineOn;
	bool isHit;
	bool isDestroyed;
public:
	AIPlayer():isEngineOn(false),
	isHit(false),
	isDestroyed(false){
		spaceship = make_shared<Spaceship>(Spaceship_Size, 90, Spaceship_Thrust, Spaceship_Full_Speed, Spaceship_Angular_Speed);
		spaceship->setSpaceshipTexture(Enemy_Spaceship_Texture);
		spaceship->setSpaceshipFlameTexture(Enemy_Spaceship_Engine_Flame_Texture);
		spaceship->setSpaceshipFlameSize(Enemy_Spaceship_Flame_Width, Enemy_Spaceship_Flame_Height);
		spaceship->setSpaceshipFlamePosition(spaceship->getPosition().x, spaceship->getPosition().y);
	}
	shared_ptr<Spaceship> getSpaceship() { return spaceship; }
	void setSpaceshipPosition(float x, float y) { spaceship->setPosition(x, y); }
	void setSpaceshipRotation(float r) { spaceship->setSpaceshipRotation(r); }
	void moveForward() { if (!spaceship->getIsHit()) spaceship->moveForward(); }
	void turnLeft() { if (!spaceship->getIsHit()) spaceship->turnLeft(); }
	void turnRight() { if (!spaceship->getIsHit()) spaceship->turnRight(); }
	void setIsEngineOn(bool t) { isEngineOn = t; }
	bool getIsEngineOn() { return isEngineOn; }
	void setIsHit(bool t) { isHit = t; }
	bool getIsHit() { return isHit; }
	void setIsDestroyed(bool t) { isDestroyed = t; }
	bool getIsDestroyed() { return isDestroyed; }
	void explode() { spaceship->explode(); }
	bool isSpaceshipHit() { return spaceship->getIsHit(); }
	void reset() { spaceship->reset(); spaceship->setSpaceshipRotation(90); }
	void fireFun() { spaceship->fire(); }
	void resetSpaceshipEngineSound() { spaceship->resetEngineSound(); }
};