#pragma once
#include <memory>
#include <SFML\Graphics.hpp>
#include "GameConstants.h"
#include "Asteroid.h"
#include "Matrix.h"

using namespace sf;
using namespace std;

class Missile :public CircleShape {
private:
	static Texture texture;
	static Texture flameTexture;
	RectangleShape flame;
	float thrust, fullSpeed;
	float maximumAngularSpeed;
	DamageType damageType;
	Vector2f velocity;
	bool fired;
	float direction;
	shared_ptr<Asteroid> predefinedTarget;
	int navigationCounter;
public:
	Missile(float r = 0, float t = 0,float s=0,float a=0) :CircleShape(r),
	flame(Vector2f(Missile_Flame_Width,Missile_Flame_Height)),
	thrust(t),
	fullSpeed(s),
	maximumAngularSpeed(a),
	damageType(DamageType::Annihilate),
	fired(false),
	navigationCounter(0){
		setOrigin(r, r);
		setTexture(&texture);
		flame.setTexture(&flameTexture);
		flame.setOrigin(-r,Missile_Flame_Height/2);
	}
	static void loadTexture();
	void setDirection(float r);
	void setPosition(Vector2f v) { CircleShape::setPosition(v); flame.setPosition(v); }
	void setFired(bool t) { fired = t; }
	bool getFired() { return fired; }
	void setPredefinedTarget(shared_ptr<Asteroid> a) { predefinedTarget = a; }
	void navigate();
	void moveForward();
	void move() { CircleShape::move(velocity); flame.move(velocity); }
	void shift(Vector2f v) { CircleShape::move(v); flame.move(v); }
	bool withinVisibleArea(FloatRect r) { return r.intersects(getGlobalBounds()); }
	int hitTarget(shared_ptr<Asteroid>a);
	DamageType getDamageType() { return damageType; }
	shared_ptr<Asteroid> target(set<shared_ptr<Asteroid>> allTargets);
	RectangleShape getFlame() { return flame; }
	void rotate(float r);
};

Texture Missile::texture;
Texture Missile::flameTexture;

void Missile::loadTexture() {
	texture.loadFromFile(Missile_Texture);
	flameTexture.loadFromFile(Missile_Flame_Texture);
}

void Missile::setDirection(float r) {
	direction = r;
	setRotation(r);
	flame.setRotation(r);
}

void Missile::rotate(float r) {
	direction += r;
	CircleShape::rotate(r);
	flame.rotate(r);
}

shared_ptr<Asteroid> Missile::target(set<shared_ptr<Asteroid>> allTargets) {
	vector<shared_ptr<Asteroid>> targets;
	for (shared_ptr<Asteroid> a : allTargets) {
		if (!a->getIsHit()) {
			Vector2f offset = a->getPosition() - getPosition();
			float distance = sqrt(offset.x*offset.x + offset.y*offset.y);
			if (distance < a->getRadius() + getRadius())
				targets.push_back(a);
		}
	}
	if (targets.size() == 0)
		return nullptr;
	else if (targets.size() == 1)
		return targets[0];
	else {
		Vector2f position = getPosition();
		auto predicate = [position](shared_ptr<Asteroid> a, shared_ptr<Asteroid> b) {
			Vector2f offset1 = a->getPosition() - position, offset2 = b->getPosition() - position;
			float distance1 = sqrt(offset1.x*offset1.x + offset1.y*offset1.y), distance2 = sqrt(offset2.x*offset2.x + offset2.y*offset2.y);
			return distance1 < distance2;
		};
		sort(targets.begin(), targets.end(), predicate);
		return targets[0];
	}
}

int Missile::hitTarget(shared_ptr<Asteroid> a) {
	a->setIsHit(true);
	return a->getScore(damageType);
}

void Missile::navigate() {
	if (navigationCounter < Missile_Navigation_Preparation_Duration)
		++navigationCounter;
	else {
		FloatRect visibleBound(0, 0, Window_Width, Window_Height);
		if (predefinedTarget != nullptr&&!predefinedTarget->getIsHit()&&!predefinedTarget->getIsDestroyed()&&visibleBound.intersects(predefinedTarget->getGlobalBounds())) {
			Matrix rotationMatrix(cos(direction*Degree_To_Radian), sin(direction*Degree_To_Radian), -sin(direction*Degree_To_Radian), cos(direction*Degree_To_Radian));
			Vector2f selfPositionInLocalCoordinates = rotationMatrix*getPosition();
			Vector2f targetPositionInLocalCoordinates = rotationMatrix*predefinedTarget->getPosition();
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
			if (turningAngle < 0) {
				if (turningAngle < maximumAngularSpeed)
					rotate(maximumAngularSpeed);
				else
					rotate(turningAngle);
			}
			else {
				if (turningAngle > -maximumAngularSpeed)
					rotate(-maximumAngularSpeed);
				else
					rotate(turningAngle);
			}
		}
	}
}

void Missile::moveForward() {
	Matrix rotationMatrix(cos(direction*Degree_To_Radian), sin(direction*Degree_To_Radian), -sin(direction*Degree_To_Radian), cos(direction*Degree_To_Radian));
	Vector2f velocityInLocalCoordinates = rotationMatrix*velocity;
	float xSpeed = velocityInLocalCoordinates.x + thrust > fullSpeed ? velocityInLocalCoordinates.x + thrust : fullSpeed;
	Vector2f newVelocityInLocalCoordinates(xSpeed, velocityInLocalCoordinates.y);
	Matrix reverseRotationMatrix(cos(-direction*Degree_To_Radian), sin(-direction*Degree_To_Radian), -sin(-direction*Degree_To_Radian), cos(-direction*Degree_To_Radian));
	velocity = reverseRotationMatrix*newVelocityInLocalCoordinates;
	if (navigationCounter == Missile_Navigation_Preparation_Duration) {
		velocity = Vector2f(0, 0);
		navigationCounter++;
	}
}