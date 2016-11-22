#pragma once
#include <cmath>
#include <iostream>
#include <fstream>
#include <SFML\Graphics.hpp>
#include "GameConstants.h"
#include "Matrix.h"

using namespace sf;
using namespace std;

class Spaceship:public CircleShape{
private:
	RectangleShape flame;
	Texture texture;
	Texture flameTexture;
	float direction;
	float angularSpeed;
	float thrust, fullSpeed;
	Vector2f velocity;
public:
	Spaceship(float size,float direction,float thrust,float fullSpeed,float angularSpeed):CircleShape(size),
	direction(direction),
	thrust(thrust),
	fullSpeed(fullSpeed),
	angularSpeed(angularSpeed){
		setOrigin(size, size);
		rotate(direction);
		flame.rotate(direction);
	}

	void setPosition(float x,float y);
	void setSpaceshipTexture(const char textureFile[]);
	void setSpaceshipFlameSize(float x, float y);
	void setSpaceshipFlamePosition(float x, float y);
	void setSpaceshipFlameTexture(const char textureFile[]);
	RectangleShape getEngineFlame() { return flame; }
	void setThrust(float f) { thrust = f; }
	float getThrust() { return thrust; }
	void setFullThrottle(float f) { fullSpeed = f; }
	float getFullThrottle() { return fullSpeed; }
	void setVelocity(Vector2f v) { velocity = v; }
	Vector2f getVelocity() { return velocity; }
	void moveForward();
	//void moveBackward();
	void turnLeft();
	void turnRight();
	void move();
};

void Spaceship::setPosition(float x, float y) {
	CircleShape::setPosition(x, y);
	flame.setPosition(x, y);
}

void Spaceship::setSpaceshipTexture(const char textureFile[]) {
	texture.loadFromFile(textureFile);
	setTexture(&texture);
}

void Spaceship::setSpaceshipFlameSize(float x, float y) {
	flame.setSize(Vector2f(x, y));
	flame.setOrigin(-getRadius(), y / 2);
}

void Spaceship::setSpaceshipFlamePosition(float x, float y) {
	flame.setPosition(x, y);
}

void Spaceship::setSpaceshipFlameTexture(const char textureFile[]) {
	flameTexture.loadFromFile(textureFile);
	flame.setTexture(&flameTexture);
}

void Spaceship::moveForward() {
	//cout <<"Original velocity: "<< velocity.x << " " << velocity.y << endl;
	Matrix rotationMatrix(cos(direction*Degree_To_Radian), sin(direction*Degree_To_Radian), -sin(direction*Degree_To_Radian), cos(direction*Degree_To_Radian));
	Vector2f velocityInLocalCoordinates = rotationMatrix*velocity;
	//cout <<"Velocity in local coordinate: "<< velocityInLocalCoordinates.x << " " << velocityInLocalCoordinates.y << endl;
	float xSpeed = velocityInLocalCoordinates.x + thrust > fullSpeed ? velocityInLocalCoordinates.x + thrust : fullSpeed;
	Vector2f newVelocityInLocalCoordinates(xSpeed, velocityInLocalCoordinates.y);
	Matrix reverseRotationMatrix(cos(-direction*Degree_To_Radian), sin(-direction*Degree_To_Radian), -sin(-direction*Degree_To_Radian), cos(-direction*Degree_To_Radian));
	velocity = reverseRotationMatrix*newVelocityInLocalCoordinates;
	cout <<"New velocity: "<< velocity.x << " " << velocity.y << endl;
}
/*
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
}*/

void Spaceship::turnLeft() {
	direction += angularSpeed;
	rotate(angularSpeed);
	flame.rotate(angularSpeed);
}

void Spaceship::turnRight() {
	direction -= angularSpeed;
	rotate(-angularSpeed);
	flame.rotate(-angularSpeed);
}

void Spaceship::move() {
	cout << "Velocity: "<<velocity.x<<" "<<velocity.y<< endl;
	CircleShape::move(velocity);
	flame.move(velocity);
}
