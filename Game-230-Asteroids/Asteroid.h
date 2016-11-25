#pragma once
#include <vector>
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
	Vector2f velocity;
	float angularVelocity;
public:
	Asteroid(float r, AsteroidSize s) :CircleShape(r),
		size(s) {
		srand(time(NULL));
		setOrigin(r, r);
		angularVelocity=rand() % (Asteroid_Maximum_Angular_Velocity - Asteroid_Minimum_Angular_Velocity) + Asteroid_Minimum_Angular_Velocity;
		switch (s) {
		case AsteroidSize::Small:setTexture(&textures[0]); break;
		case AsteroidSize::Medium:setTexture(&textures[1]); break;
		case AsteroidSize::Large:setTexture(&textures[2]); break;
		}
	}
	void setVelocity(Vector2f v) { velocity = v; }
	void move() { CircleShape::move(velocity); }
	vector<Asteroid> damage(int d);
	static void loadTextures();
};

Texture Asteroid::textures[3];

void Asteroid::loadTextures() {
	textures[0].loadFromFile(Asteroid_Small_Texture);
	textures[1].loadFromFile(Asteroid_Medium_Texture);
	textures[2].loadFromFile(Asteroid_Large_Texture);
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