#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <memory>
#include "GameConstants.h"
#include "Asteroid.h"
#include "Background.h"
#include "Player.h"

using namespace sf;
using namespace std;

class Level {
private:
	Background background;
	FloatRect spawnBound;
	vector<shared_ptr<Asteroid>> asteroids;
	vector<shared_ptr<Asteroid>> spawnedAsteroids;
	int asteroidNumberIncrement;
	int spawnedAsteroidNumber;
	int asteroidSpawningCounter;
	Player player;
	bool playerForward,playerBackward,playerLeft, playerRight;
	void initializeAsteroids();
	void spawnAsteroid();
	void spawnAsteroids();
public:
	Level():spawnBound(Spawn_Bound_Left,Spawn_Bound_Top,Spawn_Bound_Width,Spawn_Bound_Height),
	asteroidNumberIncrement(0),
	spawnedAsteroidNumber(0),
	asteroidSpawningCounter(0){
		player.setSpaceshipPosition(Window_Width / 2, Window_Height / 2);
		Asteroid::loadTextures();
		initializeAsteroids();
	}
	void setDisplayWindow(FloatRect w) { background.setDisplayWindow(w); }
	void processEvent(Event event);
	void processAction();
	void render(RenderWindow &window);
};

void Level::initializeAsteroids() {
	asteroids.clear();
	for (int i = 0; i < Initial_Asteroid_Number + asteroidNumberIncrement; i++) {
		shared_ptr<Asteroid> a=make_shared<Asteroid>(Asteroid_Large_Radius, AsteroidSize::Large);
		asteroids.push_back(a);
	}
}

void Level::spawnAsteroid() {
	int i = 0;
	while (i < spawnedAsteroids.size()) {
		if (!spawnBound.intersects(spawnedAsteroids[i]->getGlobalBounds())) {
			asteroids.push_back(spawnedAsteroids[i]);
			spawnedAsteroids.erase(spawnedAsteroids.begin() + i);
		}
		else
			++i;
	}
	if (spawnedAsteroids.size() < Minimum_Spawn_Asteroid_Number) {
		shared_ptr<Asteroid> nextAsteroid = asteroids[0];
		float x, y;
		bool suitableSpawnPoint;
		do {
			suitableSpawnPoint = true;
			int spawnPosition = rand() % 4;
			if (spawnPosition == 0) {
				x = Spawn_Bound_Left;
				y = rand() % Spawn_Bound_Height + Spawn_Bound_Top;
			}
			if (spawnPosition == 1) {
				x = rand()%Spawn_Bound_Width+Spawn_Bound_Left;
				y = Spawn_Bound_Top;
			}
			if (spawnPosition == 2) {
				x = Spawn_Bound_Left+Spawn_Bound_Width;
				y = rand() % Spawn_Bound_Height + Spawn_Bound_Top;
			}
			if (spawnPosition == 3) {
				x = rand() % Spawn_Bound_Width + Spawn_Bound_Left;
				y = Spawn_Bound_Top+Spawn_Bound_Height;
			}
			nextAsteroid->setPosition(x, y);
			for (shared_ptr<Asteroid> a : spawnedAsteroids) {
				if (a->getGlobalBounds().intersects(nextAsteroid->getGlobalBounds()))
					suitableSpawnPoint = false;
			}
		} while (!suitableSpawnPoint);
		spawnedAsteroids.push_back(nextAsteroid);
		asteroids.erase(asteroids.begin());
		float destinationX = rand() % Display_Center_Width + Display_Center_X_Position, destinationY = rand() % Display_Center_Width + Display_Center_Y_Position;
		float deltaX = destinationX - x, deltaY = destinationY - y;
		float angle;
		if (deltaX == 0) {
			if (deltaY > 0)
				angle = PI/2;
			else
				angle = -PI/2;
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
		float speed = rand() % (Asteroid_Maximum_Speed - Asteroid_Minimum_Speed) + Asteroid_Minimum_Speed;
		nextAsteroid->setVelocity(Vector2f(speed*cos(angle), speed*sin(angle)));
	}
}

void Level::spawnAsteroids() {
	if (asteroidSpawningCounter < Refresh_Frequency / 2)
		asteroidSpawningCounter++;
	else {
		asteroidSpawningCounter = 0;
		spawnAsteroid();
	}
}

void Level::processEvent(Event event) {
	playerForward=false,playerBackward=false,playerLeft = false, playerRight = false;
	if (Keyboard::isKeyPressed(Keyboard::Up))
		playerForward = true;
	if (Keyboard::isKeyPressed(Keyboard::Down))
		playerBackward = true;
	if (Keyboard::isKeyPressed(Keyboard::Left))
		playerLeft = true;
	if (Keyboard::isKeyPressed(Keyboard::Right))
		playerRight = true;
}

void Level::processAction() {
	if (playerForward)
		player.moveForward();
	//if (playerBackward)
	//	player.moveBackward();
	if (playerLeft)
		player.turnLeft();
	if (playerRight)
		player.turnRight();
	player.act();
	if (!background.isWithinInnerBound(player.getSpaceship())) {
		Vector2f shift=background.getShift(player);
		background.shiftPanels(shift*Background_Shift_Parallax_Coefficient);
		background.rotatePanels(player.getSpaceship().getPosition());
		for (shared_ptr<Asteroid> a : spawnedAsteroids)
			a->shiftPosition(shift);
	}
	for (shared_ptr<Asteroid> a : spawnedAsteroids)
		a->move();
	spawnAsteroids();
}

void Level::render(RenderWindow &window) {
	vector<shared_ptr<BackgroundPanel>> visiblePanels = background.getVisiblePanels();
	for (shared_ptr<BackgroundPanel> p : visiblePanels)
		window.draw(*p);
	for (shared_ptr<Asteroid> a : spawnedAsteroids)
		window.draw(*a);
	if (playerForward)
		window.draw(player.getSpaceship().getEngineFlame());
	window.draw(player.getSpaceship());
}