#pragma once
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SFML\Graphics.hpp>
#include "GameConstants.h"

using namespace sf;
using namespace std;

class Asteroid :public CircleShape {
private:
	static Texture textures[];
	AsteroidSize size;
	int mass;
	Vector2f velocity;
	float angularVelocity;
	bool collideWithAnotherAsteroid(shared_ptr<Asteroid> a);
	Vector2f velocityAfterCollision(shared_ptr<Asteroid> a);
public:
	Asteroid(float r, AsteroidSize s) :CircleShape(r),
		size(s) {
		srand(time(NULL));
		setOrigin(r, r);
		angularVelocity=rand() % (Asteroid_Maximum_Angular_Velocity - Asteroid_Minimum_Angular_Velocity) + Asteroid_Minimum_Angular_Velocity;
		angularVelocity *= (rand() % 2 == 0) ? -1 : 1;
		switch (s) {
		case AsteroidSize::Small:setTexture(&textures[0]); mass = Asteroid_Mass_Small; break;
		case AsteroidSize::Medium:setTexture(&textures[1]); mass = Asteroid_Mass_Medium; break;
		case AsteroidSize::Large:setTexture(&textures[2]); mass = Asteroid_Mass_Large; break;
		}
	}
	void setVelocity(Vector2f v) { velocity = v; }
	Vector2f getVelocity() { return velocity; }
	int getMass() { return mass; }
	void move() { CircleShape::move(velocity); rotate(angularVelocity); }
	void shiftPosition(Vector2f v) { CircleShape::move(v); }
	Vector2f newVelocity(set<shared_ptr<Asteroid>> collidibleAsteroids);
	vector<Asteroid> damage(int d);
	static void loadTextures();
};

Texture Asteroid::textures[3];

void Asteroid::loadTextures() {
	textures[0].loadFromFile(Asteroid_Small_Texture);
	textures[1].loadFromFile(Asteroid_Medium_Texture);
	textures[2].loadFromFile(Asteroid_Large_Texture);
}

bool Asteroid::collideWithAnotherAsteroid(shared_ptr<Asteroid> a) {
/*	if (a->getPosition().x<getPosition().x&&a->getVelocity().x <= velocity.x ||
		a->getPosition().x>getPosition().x&&a->getVelocity().x >= velocity.x ||
		a->getPosition().y<getPosition().y&&a->getVelocity().y <= velocity.y ||
		a->getPosition().y>getPosition().y&&a->getVelocity().y >= velocity.y)
		return false;
	else {*/
		Vector2f offset = a->getPosition() - getPosition();
		float distance = sqrt(offset.x*offset.x + offset.y*offset.y);
		if (distance > getRadius() + a->getRadius())
			return false;
		else
			return true;
//	}
}

Vector2f Asteroid::velocityAfterCollision(shared_ptr<Asteroid> a) {
	//float netMomentumX = mass*velocity.x + a->getMass()*a->getVelocity().x, netMomentumY = mass*velocity.y + a->getMass()*a->getVelocity().y;
	//float netKineticEnergyDoubleX=mass*velocity.x*velocity.x+ a->getMass()*a->getVelocity().x*a->getVelocity().x,netKineticEnergyDoubleY= mass*velocity.y*velocity.y + a->getMass()*a->getVelocity().y*a->getVelocity().y;
	float m1 = mass, m2 = a->getMass(), v1x = velocity.x, v1y = velocity.y, v2x = a->getVelocity().x, v2y = a->getVelocity().y;
	float secondOrderCoefficient = m1*(m1 + m2), firstOrderCoefficientX = -2 * m1*(m1*v1x + m2*v2x), firstOrderCoefficientY=-2 * m1*(m1*v1y + m2*v2y), constantX = m1*m1*v1x*v1x + 2 * m1*m2*v1x*v2x - m1*m2*v1x*v1x, constantY = m1*m1*v1y*v1y + 2 * m1*m2*v1y*v2y - m1*m2*v1y*v1y;
	float determinantX = firstOrderCoefficientX*firstOrderCoefficientX - 4 * secondOrderCoefficient*constantX, determinantY = firstOrderCoefficientY*firstOrderCoefficientY - 4 * secondOrderCoefficient*constantY;
	float newV1X1 = (-firstOrderCoefficientX + sqrt(determinantX)) / 2 / secondOrderCoefficient, newV1X2 = (-firstOrderCoefficientX - sqrt(determinantX)) / 2 / secondOrderCoefficient;
	float newV1Y1 = (-firstOrderCoefficientY + sqrt(determinantY)) / 2 / secondOrderCoefficient, newV1Y2 = (-firstOrderCoefficientY - sqrt(determinantY)) / 2 / secondOrderCoefficient;
	float solutionX, solutionY;
	if (abs(newV1X1 - v1x) < Epsilon)
		solutionX = newV1X2;
	else
		solutionX = newV1X1;
	if (abs(newV1Y1 - v1y) < Epsilon)
		solutionY = newV1Y2;
	else
		solutionY = newV1Y1;
	return Vector2f(solutionX, solutionY);
}

Vector2f Asteroid::newVelocity(set<shared_ptr<Asteroid>> collidibleAsteroids) {
	Vector2f resultantVelocity(0,0);
	bool collided = false;
	for (shared_ptr<Asteroid> a : collidibleAsteroids) {
		if (this!=a.get()&&collideWithAnotherAsteroid(a)) {
			resultantVelocity += velocityAfterCollision(a);
			collided = true;
		}
	}
	if (collided)
		return resultantVelocity;
	else
		return velocity;
}

vector<Asteroid> Asteroid::damage(int d) {
	vector<Asteroid> remanents;
	if (d == 0) {
		if (size == AsteroidSize::Large) {
			Asteroid a1(Asteroid_Medium_Radius, AsteroidSize::Medium), a2(Asteroid_Medium_Radius, AsteroidSize::Medium);
			float spawnAngle = rand() % 180;
			float spawnXOffset = cos(spawnAngle*Degree_To_Radian)*(Asteroid_Large_Radius - Asteroid_Medium_Radius), spawnYOffset = sin(spawnAngle*Degree_To_Radian)*(Asteroid_Large_Radius - Asteroid_Medium_Radius);
			a1.setPosition(getPosition().x+spawnXOffset,getPosition().y+spawnYOffset);
			a2.setPosition(getPosition().x - spawnXOffset, getPosition().y - spawnYOffset);
			float initialVelocityDirection1 = rand() % Asteroid_Spawn_Initial_Angle_Range - Asteroid_Spawn_Initial_Angle_Range + spawnAngle, initialVelocityDirection2 = rand() % Asteroid_Spawn_Initial_Angle_Range - Asteroid_Spawn_Initial_Angle_Range - spawnAngle;
			float initialSpeed1 = rand() % (Asteroid_Maximum_Speed - Asteroid_Minimum_Speed) + Asteroid_Minimum_Speed, initialSpeed2 = rand() % (Asteroid_Maximum_Speed - Asteroid_Minimum_Speed) + Asteroid_Minimum_Speed;
			a1.setVelocity(Vector2f(initialSpeed1*cos(initialVelocityDirection1*Degree_To_Radian), initialSpeed1*sin(initialVelocityDirection1*Degree_To_Radian)));
			a2.setVelocity(Vector2f(initialSpeed2*cos(initialVelocityDirection2*Degree_To_Radian), initialSpeed2*sin(initialVelocityDirection2*Degree_To_Radian)));
			remanents.push_back(a1);
			remanents.push_back(a2);
		}
		if (size == AsteroidSize::Medium) {
			Asteroid a1(Asteroid_Small_Radius, AsteroidSize::Small), a2(Asteroid_Small_Radius, AsteroidSize::Small);
			float spawnAngle = rand() % 180;
			float spawnXOffset = cos(spawnAngle*Degree_To_Radian)*(Asteroid_Medium_Radius - Asteroid_Small_Radius), spawnYOffset = sin(spawnAngle*Degree_To_Radian)*(Asteroid_Medium_Radius - Asteroid_Small_Radius);
			a1.setPosition(getPosition().x + spawnXOffset, getPosition().y + spawnYOffset);
			a2.setPosition(getPosition().x - spawnXOffset, getPosition().y - spawnYOffset);
			float initialVelocityDirection1 = rand() % Asteroid_Spawn_Initial_Angle_Range - Asteroid_Spawn_Initial_Angle_Range + spawnAngle, initialVelocityDirection2 = rand() % Asteroid_Spawn_Initial_Angle_Range - Asteroid_Spawn_Initial_Angle_Range - spawnAngle;
			float initialSpeed1 = rand() % (Asteroid_Maximum_Speed - Asteroid_Minimum_Speed) + Asteroid_Minimum_Speed, initialSpeed2 = rand() % (Asteroid_Maximum_Speed - Asteroid_Minimum_Speed) + Asteroid_Minimum_Speed;
			a1.setVelocity(Vector2f(initialSpeed1*cos(initialVelocityDirection1*Degree_To_Radian), initialSpeed1*sin(initialVelocityDirection1*Degree_To_Radian)));
			a2.setVelocity(Vector2f(initialSpeed2*cos(initialVelocityDirection2*Degree_To_Radian), initialSpeed2*sin(initialVelocityDirection2*Degree_To_Radian)));
			remanents.push_back(a1);
			remanents.push_back(a2);
		}
	}	
	return remanents;
}