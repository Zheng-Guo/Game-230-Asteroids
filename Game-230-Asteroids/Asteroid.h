#pragma once
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "GameConstants.h"

using namespace sf;
using namespace std;

class Asteroid :public CircleShape {
private:
	static Texture textures[];
	AsteroidSize size;
	int mass;
	int score;
	Vector2f velocity;
	float angularVelocity;
	set<shared_ptr<Asteroid>> previouslyCollidedAsteroids;
	static Texture explosionTexture;
	RectangleShape explosion;
	int explosionCounter;
	int explosionTextureX, explosionTextureY;
	int explosionSpeed;
	DamageType damageTaken;
	bool isHit;
	bool isDestroyed;
	bool isSplit;
	bool collideWithAnotherAsteroid(shared_ptr<Asteroid> a);
	Vector2f velocityAfterCollision(shared_ptr<Asteroid> a);
	Vector2f previousPosition;
	static SoundBuffer soundBuffer;
	Sound sound;
public:
	Asteroid(float r, AsteroidSize s) :CircleShape(r),
		size(s),
		explosion(Vector2f(r * 3, r * 3)),
		explosionCounter(0),
		explosionTextureX(1),
		explosionTextureY(0),
		explosionSpeed(Explosion_Speed),
		isHit(false),
		isDestroyed(false),
		isSplit(false){
		srand(time(NULL));
		setOrigin(r, r);
		angularVelocity=rand() % (Asteroid_Maximum_Angular_Velocity - Asteroid_Minimum_Angular_Velocity) + Asteroid_Minimum_Angular_Velocity;
		angularVelocity *= (rand() % 2 == 0) ? -1 : 1;
		switch (s) {
		case AsteroidSize::Small:setTexture(&textures[0]); mass = Asteroid_Mass_Small; score = Asteroid_Full_Score / 16; break;
		case AsteroidSize::Medium:setTexture(&textures[1]); mass = Asteroid_Mass_Medium; score = Asteroid_Full_Score / 4; break;
		case AsteroidSize::Large:setTexture(&textures[2]); mass = Asteroid_Mass_Large; score = Asteroid_Full_Score; break;
		}
		explosion.setTexture(&explosionTexture);
		explosion.setTextureRect(IntRect(0, 0, Asteroid_Explosion_Frame_Width, Asteroid_Explosion_Frame_Height));
		explosion.setOrigin(explosion.getSize().x / 2, explosion.getSize().y / 2);
		explosion.setPosition(getPosition());
		sound.setBuffer(soundBuffer);
		sound.setVolume(25);
	}
	void setPosition(float x, float y) { CircleShape::setPosition(x, y); explosion.setPosition(x, y); }
	AsteroidSize getSize() { return size; }
	void setVelocity(Vector2f v) { velocity = v; }
	Vector2f getVelocity() { return velocity; }
	void setIsHit(bool t) { isHit = t; }
	bool getIsHit() { return isHit; }
	void setIsDestroyed(bool t) { isDestroyed = t; }
	bool getIsDestroyed() { return isDestroyed; }
	bool getIsSplit() { return isSplit; }
	int getMass() { return mass; }
	int getScore(DamageType d);
	void move() { CircleShape::move(velocity); rotate(angularVelocity); explosion.move(velocity); }
	void shiftPosition(Vector2f v) { CircleShape::move(v); explosion.move(v); }
	Vector2f newVelocity(set<shared_ptr<Asteroid>> collidibleAsteroids);
	float stepBack(set<shared_ptr<Asteroid>> collidibleAsteroids);
	vector<shared_ptr<Asteroid>>damage();
	RectangleShape getExplosion() { return explosion; }
	void explode();
	static void loadTextures();
	static void loadSounds();
};

Texture Asteroid::textures[3];
Texture Asteroid::explosionTexture;

void Asteroid::loadTextures() {
	textures[0].loadFromFile(Asteroid_Small_Texture);
	textures[1].loadFromFile(Asteroid_Medium_Texture);
	textures[2].loadFromFile(Asteroid_Large_Texture);
	explosionTexture.loadFromFile(Asteroid_Explosion_Texture);
}

SoundBuffer Asteroid::soundBuffer;

void Asteroid::loadSounds() {
	soundBuffer.loadFromFile(Explosion_Sound);
}

bool Asteroid::collideWithAnotherAsteroid(shared_ptr<Asteroid> a) {
	if (!a->getIsHit()) {
		Vector2f offset = a->getPosition() - getPosition();
		float distance = sqrt(offset.x*offset.x + offset.y*offset.y);
		if (distance > getRadius() + a->getRadius())
			return false;
		else
			return true;
	}
	else
		return false;
}

Vector2f Asteroid::velocityAfterCollision(shared_ptr<Asteroid> a) {
	float m1 = mass, m2 = a->getMass(), v1x = velocity.x, v1y = velocity.y, v2x = a->getVelocity().x, v2y = a->getVelocity().y;
	float secondOrderCoefficient = m1*(m1 + m2), firstOrderCoefficientX = -2 * m1*(m1*v1x + m2*v2x), firstOrderCoefficientY=-2 * m1*(m1*v1y + m2*v2y), constantX = m1*m1*v1x*v1x + 2 * m1*m2*v1x*v2x - m1*m2*v1x*v1x, constantY = m1*m1*v1y*v1y + 2 * m1*m2*v1y*v2y - m1*m2*v1y*v1y;
	float determinantX = firstOrderCoefficientX*firstOrderCoefficientX - 4 * secondOrderCoefficient*constantX, determinantY = firstOrderCoefficientY*firstOrderCoefficientY - 4 * secondOrderCoefficient*constantY;
	if (determinantX <= 0)
		determinantX = 0;
	if (determinantY <= 0)
		determinantY = 0;
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
	set<shared_ptr<Asteroid>> asteroidsToBeCollided;
	bool collided = false;
	for (shared_ptr<Asteroid> a : collidibleAsteroids) {
		if (this!=a.get()&&previouslyCollidedAsteroids.find(a)==previouslyCollidedAsteroids.end()&&collideWithAnotherAsteroid(a)) {
			asteroidsToBeCollided.insert(a);
			resultantVelocity += velocityAfterCollision(a);
			collided = true;
		}
	}
	previouslyCollidedAsteroids = asteroidsToBeCollided;
	if (collided)
		return resultantVelocity;
	else
		return velocity;
}

float Asteroid::stepBack(set<shared_ptr<Asteroid>> collidibleAsteroids) {
	float finalStepBackFactor = 0;
	for (shared_ptr<Asteroid> a : collidibleAsteroids) {
		if (this != a.get() && collideWithAnotherAsteroid(a)) {
			Vector2f offset = a->getPosition() - getPosition();
			float distance = sqrt(offset.x*offset.x + offset.y*offset.y);
			float stepBackDistance = a->getRadius() + getRadius() - distance;
			float deltaX = a->getPosition().x - getPosition().x, deltaY = a->getPosition().y - getPosition().y;
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
			float stepBackFactor;
			if (angle == PI / 2 || angle == -PI / 2) {
				stepBackFactor = stepBackDistance / (a->getVelocity().y + velocity.y);
			}
			else {
				stepBackFactor= stepBackDistance*cos(angle)/ (a->getVelocity().x + velocity.x);
			}
			if (stepBackFactor > finalStepBackFactor)
				finalStepBackFactor = stepBackFactor;
		}
	}
	return finalStepBackFactor;
}

int Asteroid::getScore(DamageType d) {
	damageTaken = d;
	if (d == DamageType::Split) {
		if (size == AsteroidSize::Large|| size == AsteroidSize::Medium)
			return score / 2;
		else return score;
	}
	else
		return score;
}

vector<shared_ptr<Asteroid>> Asteroid::damage() {
	vector<shared_ptr<Asteroid>> remanents;
	if (damageTaken == DamageType::Split) {
		if (size == AsteroidSize::Large) {
			shared_ptr<Asteroid> a1=make_shared<Asteroid>(Asteroid_Medium_Radius, AsteroidSize::Medium), a2 = make_shared<Asteroid>(Asteroid_Medium_Radius, AsteroidSize::Medium);
			float spawnAngle = rand() % 180;
			float spawnXOffset = cos(spawnAngle*Degree_To_Radian)*(Asteroid_Large_Radius - Asteroid_Medium_Radius+30), spawnYOffset = sin(spawnAngle*Degree_To_Radian)*(Asteroid_Large_Radius - Asteroid_Medium_Radius+30);
			a1->setPosition(getPosition().x+spawnXOffset,getPosition().y+spawnYOffset);
			a2->setPosition(getPosition().x - spawnXOffset, getPosition().y - spawnYOffset);
			float initialVelocityDirection1 = rand() % Asteroid_Spawn_Initial_Angle_Range - Asteroid_Spawn_Initial_Angle_Range + spawnAngle, initialVelocityDirection2 = rand() % Asteroid_Spawn_Initial_Angle_Range - Asteroid_Spawn_Initial_Angle_Range - spawnAngle;
			float initialSpeed1 = rand() % (Asteroid_Maximum_Speed - Asteroid_Minimum_Speed) + Asteroid_Minimum_Speed, initialSpeed2 = rand() % (Asteroid_Maximum_Speed - Asteroid_Minimum_Speed) + Asteroid_Minimum_Speed;
			a1->setVelocity(Vector2f(initialSpeed1*cos(initialVelocityDirection1*Degree_To_Radian), initialSpeed1*sin(initialVelocityDirection1*Degree_To_Radian)));
			a2->setVelocity(Vector2f(initialSpeed2*cos(initialVelocityDirection2*Degree_To_Radian), initialSpeed2*sin(initialVelocityDirection2*Degree_To_Radian)));
			remanents.push_back(a1);
			remanents.push_back(a2);
		}
		if (size == AsteroidSize::Medium) {
			shared_ptr<Asteroid> a1 = make_shared<Asteroid>(Asteroid_Small_Radius, AsteroidSize::Small), a2 = make_shared<Asteroid>(Asteroid_Small_Radius, AsteroidSize::Small);
			float spawnAngle = rand() % 180;
			float spawnXOffset = cos(spawnAngle*Degree_To_Radian)*(Asteroid_Medium_Radius - Asteroid_Small_Radius+30), spawnYOffset = sin(spawnAngle*Degree_To_Radian)*(Asteroid_Medium_Radius - Asteroid_Small_Radius+30);
			a1->setPosition(getPosition().x + spawnXOffset, getPosition().y + spawnYOffset);
			a2->setPosition(getPosition().x - spawnXOffset, getPosition().y - spawnYOffset);
			float initialVelocityDirection1 = rand() % Asteroid_Spawn_Initial_Angle_Range - Asteroid_Spawn_Initial_Angle_Range + spawnAngle, initialVelocityDirection2 = rand() % Asteroid_Spawn_Initial_Angle_Range - Asteroid_Spawn_Initial_Angle_Range - spawnAngle;
			float initialSpeed1 = rand() % (Asteroid_Maximum_Speed - Asteroid_Minimum_Speed) + Asteroid_Minimum_Speed, initialSpeed2 = rand() % (Asteroid_Maximum_Speed - Asteroid_Minimum_Speed) + Asteroid_Minimum_Speed;
			a1->setVelocity(Vector2f(initialSpeed1*cos(initialVelocityDirection1*Degree_To_Radian), initialSpeed1*sin(initialVelocityDirection1*Degree_To_Radian)));
			a2->setVelocity(Vector2f(initialSpeed2*cos(initialVelocityDirection2*Degree_To_Radian), initialSpeed2*sin(initialVelocityDirection2*Degree_To_Radian)));
			remanents.push_back(a1);
			remanents.push_back(a2);
		}
		isSplit = false;
	}	
	return remanents;
}

void Asteroid::explode() {
	if (explosionTextureX == 1 && explosionTextureY == 0)
		sound.play();
		if (explosionCounter < explosionSpeed) {
			explosionCounter++;
		}
		else {
			explosionCounter = 0;
			explosion.setTextureRect(IntRect(explosionTextureX * Asteroid_Explosion_Frame_Width, explosionTextureY * Asteroid_Explosion_Frame_Height, Asteroid_Explosion_Frame_Width, Asteroid_Explosion_Frame_Height));
			++explosionTextureX;
			if (explosionTextureX >= Asteroid_Explosion_Texture_Column_Number) {
				++explosionTextureY;
				explosionTextureX = 0;
			}
			if (explosionTextureY >= Asteroid_Explosion_Texture_Row_Number) {
				explosionTextureY = 0;
				isHit = false;
				isDestroyed = true;
			}
			if (explosionTextureX == Asteroid_Split_Frame_X&&explosionTextureY == Asteroid_Split_Frame_Y)
				isSplit = true;
		}
}