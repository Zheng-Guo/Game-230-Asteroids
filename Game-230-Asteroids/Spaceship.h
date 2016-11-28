#pragma once
#include <cmath>
#include <iostream>
#include <fstream>
#include <SFML\Graphics.hpp>
#include "GameConstants.h"
#include "Matrix.h"
#include "GunShot.h"

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
	bool isHit;
	bool fireGun;
	vector<shared_ptr<GunShot>> gunShots;
	int firingCounter;
public:
	Spaceship(float size,float direction,float thrust,float fullSpeed,float angularSpeed):CircleShape(size),
	direction(direction),
	thrust(thrust),
	fullSpeed(fullSpeed),
	angularSpeed(angularSpeed),
	explosion(Vector2f(Spaceship_Size*4,Spaceship_Size*4)),
	explosionCounter(0),
	explosionTextureX(1),
	explosionTextureY(0),
	explosionSpeed(Explosion_Speed),
	isHit(false),
	fireGun(false),
	firingCounter(0){
		setOrigin(size, size);
		rotate(direction);
		flame.rotate(direction);
		explosionTexture.loadFromFile(Spaceship_Explosion_Texture);
		explosion.setTexture(&explosionTexture);
		explosion.setTextureRect(IntRect(0, 0, Spaceship_Explosion_Frame_Width, Spaceship_Explosion_Frame_Height));
		explosion.setOrigin(explosion.getSize().x/2, explosion.getSize().y / 2);
		explosion.setPosition(getPosition());
		GunShot::loadTexture();
	//	for (int i = 0; i < Gun_Shot_Number; ++i) {
	//		shared_ptr<GunShot> gunshot = make_shared<GunShot>(Gun_Shot_Size,Gun_Shot_Speed);
	//		gunShots.push_back(gunshot);
	//	}
	}

	void setPosition(float x,float y);
	void setSpaceshipTexture(const char textureFile[]);
	void setSpaceshipFlameSize(float x, float y);
	void setSpaceshipFlamePosition(float x, float y);
	void setSpaceshipFlameTexture(const char textureFile[]);
	void setSpaceshipRotation(float r) { direction = r; setRotation(r), flame.setRotation(r); }
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
	vector<shared_ptr<GunShot>> getGunShots() { return gunShots; }
	void fire();
	void recycleGunShots(FloatRect r);
	void reset();
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
	if (isHit) {
		if (explosionCounter < explosionSpeed) {
			explosionCounter++;
		}
		else {
			explosionCounter = 0;
			explosion.setTextureRect(IntRect(explosionTextureX * Spaceship_Explosion_Frame_Width, explosionTextureY * Spaceship_Explosion_Frame_Height, Spaceship_Explosion_Frame_Width, Spaceship_Explosion_Frame_Height));
			++explosionTextureX;
			if (explosionTextureX >= Explosion_Texture_Column_Number) {
				++explosionTextureY;
				explosionTextureX = 0;
			}
			if (explosionTextureY >= Explosion_Texture_Row_Number) {
				explosionTextureY = 0;
				isHit = false;
			}
		}
	}
}

void Spaceship::fire() {
	if (firingCounter < Gun_Shot_Interval) {
		++firingCounter;
	}
	else {
		firingCounter = 0;
		//shared_ptr<GunShot> g = gunShots[0];
		shared_ptr<GunShot> g = make_shared<GunShot>(Gun_Shot_Size, Gun_Shot_Speed);
		g->setFired(true);
		g->setPosition(getPosition());
		g->setDirection(direction);
		//gunShots.erase(gunShots.begin());
		gunShots.push_back(g);
	}
}

void Spaceship::recycleGunShots(FloatRect r) {
//	for (shared_ptr<GunShot> g : gunShots) {
//		if (g->getFired() && !r.intersects(g->getGlobalBounds()))
//			g->setFired(false);
//	}
	int i = 0;
	while (i < gunShots.size()) {
		if (!gunShots[i]->getFired()||!r.intersects(gunShots[i]->getGlobalBounds())) {
			gunShots.erase(gunShots.begin() + i);
		}
		else
			++i;
	}
}

void Spaceship::reset() {
	velocity = Vector2f(0, 0);
	explosion.setTextureRect(IntRect(0, 0, 256, 256));
	explosionCounter = 0;
	explosionTextureX = 1;
	explosionTextureY = 0;
	isHit = false;
}