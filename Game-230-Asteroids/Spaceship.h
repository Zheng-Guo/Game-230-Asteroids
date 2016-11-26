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
	Texture explosionTexture;
	RectangleShape explosion;
	int explosionCounter;
	int explosionTextureX, explosionTextureY;
	int explosionSpeed;
	float direction;
	float angularSpeed;
	float thrust, fullSpeed;
	Vector2f velocity;
	bool isHit,spaceshipExploded;
public:
	Spaceship(float size,float direction,float thrust,float fullSpeed,float angularSpeed):CircleShape(size),
	direction(direction),
	thrust(thrust),
	fullSpeed(fullSpeed),
	angularSpeed(angularSpeed),
	explosion(Vector2f(Spaceship_Size*2,Spaceship_Size*2)),
	explosionCounter(0),
	explosionTextureX(1),
	explosionTextureY(0),
	explosionSpeed(Explosion_Speed),
	isHit(false),
	spaceshipExploded(false){
		setOrigin(size, size);
		rotate(direction);
		flame.rotate(direction);
		explosionTexture.loadFromFile(Spaceship_Explosion_Texture);
		explosion.setTexture(&explosionTexture);
		explosion.setTextureRect(IntRect(0, 0, 256, 256));
		explosion.setOrigin(Spaceship_Size, Spaceship_Size);
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
	void setIsHit(bool t) { isHit = t; }
	bool getIsHit() { return isHit; }
	void moveForward();
	//void moveBackward();
	void turnLeft();
	void turnRight();
	void move();
	void explode();
	RectangleShape getExplosion() { return explosion; }
};

void Spaceship::setPosition(float x, float y) {
	CircleShape::setPosition(x, y);
	flame.setPosition(x, y);
	explosion.setPosition(x, y);
}

void Spaceship::setSpaceshipTexture(const char textureFile[]) {
	texture.loadFromFile(textureFile);
	setTexture(&texture);
}

void Spaceship::setSpaceshipFlameSize(float x, float y) {
	flame.setSize(Vector2f(x, y));
	flame.setOrigin(-getRadius()*0.9, y / 2);
}

void Spaceship::setSpaceshipFlamePosition(float x, float y) {
	flame.setPosition(x, y);
}

void Spaceship::setSpaceshipFlameTexture(const char textureFile[]) {
	flameTexture.loadFromFile(textureFile);
	flame.setTexture(&flameTexture);
}

void Spaceship::moveForward() {
	Matrix rotationMatrix(cos(direction*Degree_To_Radian), sin(direction*Degree_To_Radian), -sin(direction*Degree_To_Radian), cos(direction*Degree_To_Radian));
	Vector2f velocityInLocalCoordinates = rotationMatrix*velocity;
	float xSpeed = velocityInLocalCoordinates.x + thrust > fullSpeed ? velocityInLocalCoordinates.x + thrust : fullSpeed;
	Vector2f newVelocityInLocalCoordinates(xSpeed, velocityInLocalCoordinates.y);
	Matrix reverseRotationMatrix(cos(-direction*Degree_To_Radian), sin(-direction*Degree_To_Radian), -sin(-direction*Degree_To_Radian), cos(-direction*Degree_To_Radian));
	velocity = reverseRotationMatrix*newVelocityInLocalCoordinates;
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
	if (direction > 180)
		direction -=360;
	else if (direction < -180)
		direction += 360;
	rotate(angularSpeed);
	flame.rotate(angularSpeed);
}

void Spaceship::turnRight() {
	direction -= angularSpeed;
	if (direction > 180)
		direction -= 360;
	else if (direction < -180)
		direction += 360;
	rotate(-angularSpeed);
	flame.rotate(-angularSpeed);
}

void Spaceship::move() {
	CircleShape::move(velocity);
	flame.move(velocity);
	explosion.move(velocity);
}

void Spaceship::explode() {
	if (!spaceshipExploded) {
		if (explosionCounter < explosionSpeed) {
			explosionCounter++;
		}
		else {
			explosionCounter = 0;
			explosion.setTextureRect(IntRect(explosionTextureX * 256, explosionTextureY * 256, 256, 256));
			++explosionTextureX;
			if (explosionTextureX >= Explosion_Texture_Column_Number) {
				++explosionTextureY;
				explosionTextureX = 0;
			}
			if (explosionTextureY >= Explosion_Texture_Row_Number) {
				explosionTextureY = 0;
				spaceshipExploded = true;
			}
		}
	}
}
