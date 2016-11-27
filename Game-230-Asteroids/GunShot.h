#pragma once
#include <set>
#include <memory>
#include <algorithm>
#include <SFML\Graphics.hpp>
#include "GameConstants.h"
#include "Asteroid.h"
#include "Player.h"

using namespace sf;

class GunShot:public CircleShape {
private:
	static Texture texture;
	float speed;
	Vector2f velocity;
	bool fired;
public:
	GunShot(float size=0,float s=0) :CircleShape(size),
	speed(s),
	fired(false){
		setOrigin(size, size);
		setTexture(&texture);
	}
	static void loadTexture();
	void setDirection(float r) { setRotation(r); velocity = Vector2f(-speed*cos(r*Degree_To_Radian), -speed*sin(r*Degree_To_Radian)); }
	void setFired(bool t) { fired = t; }
	bool getFired() { return fired; }
	void move() { CircleShape::move(velocity); }
	void shift(Vector2f v) { CircleShape::move(v); }
	bool withinVisibleArea(FloatRect r) { return r.intersects(getGlobalBounds()); }
	shared_ptr<Asteroid> target(set<shared_ptr<Asteroid>> allTargets);
};

Texture GunShot::texture;

void GunShot::loadTexture() {
	texture.loadFromFile(Gun_Shot_Texutre);
}

shared_ptr<Asteroid> GunShot::target(set<shared_ptr<Asteroid>> allTargets) {
	vector<shared_ptr<Asteroid>> targets;
	for (shared_ptr<Asteroid> a : allTargets) {
		Vector2f offset = a->getPosition() - getPosition();
		float distance= sqrt(offset.x*offset.x + offset.y*offset.y);
		if (distance < a->getRadius() + getRadius())
			targets.push_back(a);
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