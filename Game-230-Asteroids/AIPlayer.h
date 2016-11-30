#pragma once
#include <memory>
#include <SFML\Graphics.hpp>
#include "GameConstants.h"
#include "Spaceship.h"
#include "Matrix.h"
#include "GunShot.h"
#include "Missile.h"


using namespace sf;
using namespace std;

class AIPlayer {
private:
	shared_ptr<Spaceship> spaceship;
	shared_ptr<Spaceship> playerSpaceship;
	int score;
	bool isEngineOn;
	bool isHit;
	bool isDestroyed;
public:
	AIPlayer():isEngineOn(false),
	isHit(false),
	isDestroyed(false),
	score(Enemy_Spaceship_Score){
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
	void reset() { spaceship->reset(); isHit = true; isEngineOn = false; }
	void fireFun() { spaceship->fire(); }
	void resetSpaceshipEngineSound() { spaceship->resetEngineSound(); }
	void setPlayerSpaceship(shared_ptr<Spaceship> s) { playerSpaceship = s; }
	void navigate();
	bool getHit(shared_ptr<CircleShape> c);
	int getScore() { return score; }
	void loseLife();
	bool isWithinFireRange();
};

void AIPlayer::navigate() {
	float direction = spaceship->getDirection();
	Matrix rotationMatrix(cos(direction*Degree_To_Radian), sin(direction*Degree_To_Radian), -sin(direction*Degree_To_Radian), cos(direction*Degree_To_Radian));
	Vector2f selfPositionInLocalCoordinates = rotationMatrix*spaceship->getPosition();
	Vector2f targetPositionInLocalCoordinates = rotationMatrix*playerSpaceship->getPosition();
	float deltaX = targetPositionInLocalCoordinates.x - selfPositionInLocalCoordinates.x, deltaY = targetPositionInLocalCoordinates.y - selfPositionInLocalCoordinates.y;
	float angle;
	if (deltaX == 0) {
		if (deltaY > 0)
			angle = PI / 2;
		else
			angle = -PI / 2;
	}
	else if (deltaY == 0) {
		if (deltaX > 0)
			angle = 0;
		else
			angle = PI;
	}
	else {
		angle = atan(deltaY / deltaX);
		if (deltaX < 0) {
			if (deltaY > 0)
				angle += PI;
			else
				angle -= PI;
		}
	}
	float turningAngle = (angle < 0) ? angle + PI : angle - PI;
	turningAngle /= Degree_To_Radian;
	spaceship->turn(turningAngle);
}

bool AIPlayer::getHit(shared_ptr<CircleShape> c) {
	Vector2f offset = spaceship->getPosition() - c->getPosition();
	float distance = sqrt(offset.x*offset.x + offset.y*offset.y);
	if (distance <  spaceship->getRadius() + c->getRadius())
		return true;
	return false;
}

void AIPlayer::loseLife() {
	spaceship->setIsHit(true);
	isHit = true;
	spaceship->setVelocity(Vector2f(0, 0));
	for (shared_ptr<GunShot> g : spaceship->getGunShots())
		g->setFired(false);
}

bool AIPlayer::isWithinFireRange() {
	float direction = spaceship->getDirection();
	Matrix rotationMatrix(cos(direction*Degree_To_Radian), sin(direction*Degree_To_Radian), -sin(direction*Degree_To_Radian), cos(direction*Degree_To_Radian));
	Vector2f selfPositionInLocalCoordinates = rotationMatrix*spaceship->getPosition();
	Vector2f targetPositionInLocalCoordinates = rotationMatrix*playerSpaceship->getPosition();
	float deltaX = targetPositionInLocalCoordinates.x - selfPositionInLocalCoordinates.x, deltaY = targetPositionInLocalCoordinates.y - selfPositionInLocalCoordinates.y;
	float angle;
	if (deltaX == 0) {
		if (deltaY > 0)
			angle = PI / 2;
		else
			angle = -PI / 2;
	}
	else if (deltaY == 0) {
		if (deltaX > 0)
			angle = 0;
		else
			angle = PI;
	}
	else {
		angle = atan(deltaY / deltaX);
		if (deltaX < 0) {
			if (deltaY > 0)
				angle += PI;
			else
				angle -= PI;
		}
	}
	float turningAngle = (angle < 0) ? angle + PI : angle - PI;
	turningAngle /= Degree_To_Radian;
	Vector2f offset = playerSpaceship->getPosition() - spaceship->getPosition();
	float distance = sqrt(offset.x*offset.x + offset.y*offset.y);
	float fireAngle = asin((playerSpaceship->getRadius() + Gun_Shot_Size) / distance);
	if (turningAngle > 0 && turningAngle < fireAngle || turningAngle<0 && turningAngle>fireAngle)
		return true;
	return false;
}