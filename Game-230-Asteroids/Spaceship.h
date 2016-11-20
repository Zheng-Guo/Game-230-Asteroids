#pragma once
#include <cmath>
#include <SFML\Graphics.hpp>
#include "GameConstants.h"

using namespace sf;
using namespace std;

class Spaceship:public CircleShape{
private:
	RectangleShape flame;
	Texture texture;
	Texture flameTexture;
	float direction;
	float angularSpeed;
	float thrust, speed,fullSpeed;
	Vector2f currentVelocity;
public:
	Spaceship(float size,float direction,float thrust,float fullSpeed,float angularSpeed):CircleShape(size),
	direction(direction),
	thrust(thrust),
	speed(0),
	fullSpeed(fullSpeed),
	angularSpeed(angularSpeed){
		setOrigin(size, size);
		rotate(direction);
	}

	void setSpaceshipTexture(const char textureFile[]);
	void setSpaceshipFlameSize(float x, float y);
	void setSpaceshipFlamePosition(float x, float y);
	void setSpaceshipFlameTexture(const char textureFile[]);
	void setThrust(float f) { thrust = f; }
	float getThrust() { return thrust; }
	void setFullThrottle(float f) { fullSpeed = f; }
	float getFullThrottle() { return fullSpeed; }
	void setCurrentVelocity(Vector2f v) { currentVelocity = v; }
	Vector2f getCurrentVelocity() { return currentVelocity; }
	void moveForward();
	void moveBackward();
	void turnLeft();
	void turnRight();
	void move();
};

void Spaceship::setSpaceshipTexture(const char textureFile[]) {
	texture.loadFromFile(textureFile);
	setTexture(&texture);
}

void Spaceship::setSpaceshipFlameSize(float x, float y) {
	flame.setSize(Vector2f(x, y));
}

void Spaceship::setSpaceshipFlamePosition(float x, float y) {
	flame.setPosition(x, y);
}

void Spaceship::setSpaceshipFlameTexture(const char textureFile[]) {
	flameTexture.loadFromFile(textureFile);
	flame.setTexture(&texture);
}

void Spaceship::moveForward() {
	float xAcceleration = -thrust*cos(direction*Degree_To_Radian);
	float yAcceleration = -thrust*sin(direction*Degree_To_Radian);
	float newXSpeed = currentVelocity.x + xAcceleration;
	float newYSpeed = currentVelocity.y + yAcceleration;
	if (newXSpeed > 0 && newXSpeed > fullSpeed)
		newXSpeed = fullSpeed;
	if (newXSpeed < 0 && newXSpeed < -fullSpeed)
		newXSpeed = -fullSpeed;
	if (newYSpeed > 0 && newYSpeed > fullSpeed)
		newYSpeed = fullSpeed;
	if (newYSpeed < 0 && newYSpeed < -fullSpeed)
		newYSpeed = -fullSpeed;
	currentVelocity.x = newXSpeed;
	currentVelocity.y = newYSpeed;
}

void Spaceship::moveBackward() {
	float xAcceleration = thrust*cos(direction*Degree_To_Radian);
	float yAcceleration = thrust*sin(direction*Degree_To_Radian);
	float newXSpeed = currentVelocity.x + xAcceleration;
	float newYSpeed = currentVelocity.y + yAcceleration;
	if (newXSpeed > 0 && newXSpeed > fullSpeed)
		newXSpeed = fullSpeed;
	if (newXSpeed < 0 && newXSpeed < -fullSpeed)
		newXSpeed = -fullSpeed;
	if (newYSpeed > 0 && newYSpeed > fullSpeed)
		newYSpeed = fullSpeed;
	if (newYSpeed < 0 && newYSpeed < -fullSpeed)
		newYSpeed = -fullSpeed;
	currentVelocity.x = newXSpeed;
	currentVelocity.y = newYSpeed;
}

void Spaceship::turnLeft() {
	direction += angularSpeed;
	rotate(angularSpeed);
}

void Spaceship::turnRight() {
	direction -= angularSpeed;
	rotate(-angularSpeed);
}

void Spaceship::move() {
	CircleShape::move(currentVelocity);
}
