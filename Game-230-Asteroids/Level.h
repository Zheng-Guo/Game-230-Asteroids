#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <memory>
#include <utility>
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
	vector<shared_ptr<Asteroid>> bucketGrid[Bucket_Grid_Row_Number][Bucket_Grid_Column_Number];
	map<shared_ptr<Asteroid>,vector<pair<int,int>>> bucketAllocations;
	FloatRect bucketGridBounds[Bucket_Grid_Row_Number][Bucket_Grid_Column_Number];
	int asteroidNumberIncrement;
	int asteroidSpawningCounter;	
	Font font;
	Text lives;
	Text score;
	Player player;
	bool playerForward,playerBackward,playerLeft, playerRight;
	void initializeAsteroids();
	void spawnAsteroid();
	void spawnAsteroids();
	void rebucket();
	set<shared_ptr<Asteroid>> getCollidibleAsteroids();
	bool spaceshipCollision();
public:
	Level():spawnBound(Spawn_Bound_Left,Spawn_Bound_Top,Spawn_Bound_Width,Spawn_Bound_Height),
	asteroidNumberIncrement(0),
	asteroidSpawningCounter(0){		
		player.setSpaceshipPosition(Window_Width / 2, Window_Height / 2);
		player.setLives(3);
		player.setScore(0);
		for (int i = 0; i < Bucket_Grid_Row_Number; i++) {
			for (int j = 0; j < Bucket_Grid_Column_Number; j++) {
				bucketGridBounds[i][j] = FloatRect(j*Bucket_Grid_Width, i*Bucket_Grid_Height, Bucket_Grid_Width, Bucket_Grid_Height);
			}
		}
		Asteroid::loadTextures();
		initializeAsteroids();
		font.loadFromFile("Tinos-Regular.ttf");
		ostringstream ss;
		ss << "Life: " << player.getLives();
		lives.setString(ss.str());
		lives.setFont(font);
		lives.setCharacterSize(Stat_Character_Size);
		lives.setFillColor(Color::Red);
		lives.setPosition(Life_X_Position, Life_Y_Position);
		ss.str("");
		ss << "Score: " << player.getScore();
		score.setString(ss.str());
		score.setFont(font);
		score.setCharacterSize(Stat_Character_Size);
		score.setFillColor(Color::Yellow);
		score.setPosition(Score_X_Position, Score_Y_Position);
	}
	void setDisplayWindow(FloatRect w) { background.setDisplayWindow(w); }
	void processEvent(Event event);
	void processAction();
	void render(RenderWindow &window);
};

void Level::initializeAsteroids() {
	asteroids.clear();
	spawnedAsteroids.clear();
	for (int i = 0; i < Bucket_Grid_Row_Number; i++) {
		for (int j = 0; j < Bucket_Grid_Column_Number; j++) {
			bucketGrid[i][j].clear();
		}
	}
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
				x = Spawn_Bound_Left+1;
				y = rand() % Spawn_Bound_Height + Spawn_Bound_Top;
			}
			if (spawnPosition == 1) {
				x = rand()%Spawn_Bound_Width+Spawn_Bound_Left;
				y = Spawn_Bound_Top+1;
			}
			if (spawnPosition == 2) {
				x = Spawn_Bound_Left+Spawn_Bound_Width-1;
				y = rand() % Spawn_Bound_Height + Spawn_Bound_Top;
			}
			if (spawnPosition == 3) {
				x = rand() % Spawn_Bound_Width + Spawn_Bound_Left-1;
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

void Level::rebucket() {
	for (int i = 0; i < Bucket_Grid_Row_Number; i++) {
		for (int j = 0; j < Bucket_Grid_Column_Number; j++) {
			bucketGrid[i][j].clear();
		}
	}
	bucketAllocations.clear();
	for (shared_ptr<Asteroid> a : spawnedAsteroids) {
		vector<pair<int, int>> bucketAllocation;
		if (a->getPosition().x > Spawn_Bound_Left&&a->getPosition().x<Spawn_Bound_Left + Spawn_Bound_Width&&a->getPosition().y>Spawn_Bound_Top&&a->getPosition().y < Spawn_Bound_Top + Spawn_Bound_Height) {
			int i = (a->getPosition().x-Spawn_Bound_Left) / Bucket_Grid_Width, j = (a->getPosition().y-Spawn_Bound_Top) / Bucket_Grid_Height;
			bucketGrid[j][i].push_back(a);
			bucketAllocation.push_back(pair<int, int>(j, i));
			bool overlapTop = false, overlapBottom = false, overlapLeft = false, overlapRight = false;
			if (i > 0 && a->getPosition().x - a->getRadius() < (i - 1)*Bucket_Grid_Width)
				overlapLeft = true;
			if (i < Bucket_Grid_Column_Number - 1 && a->getPosition().x + a->getRadius() > i*Bucket_Grid_Width)
				overlapRight = true;
			if (j > 0 && a->getPosition().y - a->getRadius() < (j - 1)*Bucket_Grid_Height)
				overlapTop = true;
			if (j < Bucket_Grid_Row_Number - 1 && a->getPosition().y + a->getRadius() > j*Bucket_Grid_Height)
				overlapBottom = true;
			if (overlapTop) {
				bucketGrid[j - 1][i].push_back(a);
				bucketAllocation.push_back(pair<int, int>(j - 1, i));
			}
			if (overlapBottom) {
				bucketGrid[j + 1][i].push_back(a);
				bucketAllocation.push_back(pair<int, int>(j + 1, i));
			}
			if (overlapLeft) {
				bucketGrid[j][i - 1].push_back(a);
				bucketAllocation.push_back(pair<int, int>(j, i - 1));
			}
			if (overlapRight) {
				bucketGrid[j][i + 1].push_back(a);
				bucketAllocation.push_back(pair<int, int>(j, i + 1));
			}
			if (overlapTop&&overlapLeft) {
				bucketGrid[j - 1][i - 1].push_back(a);
				bucketAllocation.push_back(pair<int, int>(j - 1, i - 1));
			}
			if (overlapTop&&overlapRight) {
				bucketGrid[j - 1][i + 1].push_back(a);
				bucketAllocation.push_back(pair<int, int>(j - 1, i + 1));
			}
			if (overlapBottom&&overlapLeft) {
				bucketGrid[j + 1][i - 1].push_back(a);
				bucketAllocation.push_back(pair<int, int>(j + 1, i - 1));
			}
			if (overlapBottom&&overlapRight) {
				bucketGrid[j + 1][i + 1].push_back(a);
				bucketAllocation.push_back(pair<int, int>(j + 1, i + 1));
			}
			bucketAllocations[a] = bucketAllocation;
		}
	}
}

set<shared_ptr<Asteroid>> Level::getCollidibleAsteroids() {
	set<shared_ptr<Asteroid>> collidibleAsteroids;
	Vector2f spaceshipPosition = player.getSpaceship()->getPosition();
	float spaceshipRadius = player.getSpaceship()->getRadius();
	int i = (spaceshipPosition.x - Spawn_Bound_Left) / Bucket_Grid_Width, j = (spaceshipPosition.y - Spawn_Bound_Top) / Bucket_Grid_Height;
	collidibleAsteroids.insert(bucketGrid[j][i].begin(), bucketGrid[j][i].end());
	bool overlapTop = false, overlapBottom = false, overlapLeft = false, overlapRight = false;
	if (i > 0 && spaceshipPosition.x - spaceshipRadius < (i - 1)*Bucket_Grid_Width)
		overlapLeft = true;
	if (i < Bucket_Grid_Column_Number - 1 && spaceshipPosition.x + spaceshipRadius > i*Bucket_Grid_Width)
		overlapRight = true;
	if (j > 0 && spaceshipPosition.y - spaceshipRadius < (j - 1)*Bucket_Grid_Height)
		overlapTop = true;
	if (j < Bucket_Grid_Row_Number - 1 && spaceshipPosition.y + spaceshipRadius > j*Bucket_Grid_Height)
		overlapBottom = true;
	if (overlapTop) {
		collidibleAsteroids.insert(bucketGrid[j - 1][i].begin(), bucketGrid[j - 1][i].end());
	}
	if (overlapBottom) {
		collidibleAsteroids.insert(bucketGrid[j + 1][i].begin(), bucketGrid[j + 1][i].end());
	}
	if (overlapLeft) {
		collidibleAsteroids.insert(bucketGrid[j][i-1].begin(), bucketGrid[j][i-1].end());
	}
	if (overlapRight) {
		collidibleAsteroids.insert(bucketGrid[j][i+1].begin(), bucketGrid[j][i+1].end());
	}
	if (overlapTop&&overlapLeft) {
		collidibleAsteroids.insert(bucketGrid[j - 1][i-1].begin(), bucketGrid[j - 1][i-1].end());
	}
	if (overlapTop&&overlapRight) {
		collidibleAsteroids.insert(bucketGrid[j - 1][i+1].begin(), bucketGrid[j - 1][i+1].end());
	}
	if (overlapBottom&&overlapLeft) {
		collidibleAsteroids.insert(bucketGrid[j + 1][i-1].begin(), bucketGrid[j + 1][i-1].end());
	}
	if (overlapBottom&&overlapRight) {
		collidibleAsteroids.insert(bucketGrid[j + 1][i+1].begin(), bucketGrid[j + 1][i+1].end());
	}
	return collidibleAsteroids;
}

bool Level::spaceshipCollision() {
	set<shared_ptr<Asteroid>> collidibleAsteroids = getCollidibleAsteroids();
	shared_ptr<Spaceship> spaceship = player.getSpaceship();
	for (shared_ptr<Asteroid> a : collidibleAsteroids) {
		Vector2f offset = a->getPosition() - spaceship->getPosition();
		float distance = sqrt(offset.x*offset.x + offset.y*offset.y);
		if (distance < a->getRadius() + spaceship->getRadius())
			return true;
	}
	return false;
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
	if (!player.isSpaceshipHit()) {
		player.act();
		if (!background.isWithinInnerBound(*player.getSpaceship())) {
			Vector2f shift = background.getShift(player);
			background.shiftPanels(shift*Background_Shift_Parallax_Coefficient);
			background.rotatePanels(player.getSpaceship()->getPosition());
			for (shared_ptr<Asteroid> a : spawnedAsteroids)
				a->shiftPosition(shift);
		}
	}
	rebucket();
	if (player.isSpaceshipHit()) {
		player.explode();
	}
	else if (spaceshipCollision()) {
		player.loseLife();
		ostringstream ss;
		ss << "Life: " << player.getLives();
		lives.setString(ss.str());
	}
	/*
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			cout << "(" << i << "," << j << "):";
			for (auto a : bucketGrid[i][j])
				cout << "[" << a->getPosition().x << "," << a->getPosition().y << "] ";
		}
		cout << endl;
	}
	for (auto a : bucketAllocations) {
		cout << "(" << a.first->getPosition().x << "," << a.first->getPosition().y << "): ";
		for (auto b : a.second)
			cout << "[" << b.first << "," << b.second << "]";
		cout << endl;
	}*/
	map<shared_ptr<Asteroid>,Vector2f> newVelocities;
	for (int i = 0; i < spawnedAsteroids.size()&&i<bucketAllocations.size();i++) {
		if (bucketAllocations.find(spawnedAsteroids[i]) != bucketAllocations.end()) {
			vector<pair<int, int>> bucketAllocation = bucketAllocations[spawnedAsteroids[i]];
			set<shared_ptr<Asteroid>> collidibleAsteroids;
			for (pair<int, int> p : bucketAllocation) {
				vector<shared_ptr<Asteroid>> bucket = bucketGrid[p.first][p.second];
				collidibleAsteroids.insert(bucket.begin(), bucket.end());
			}
			Vector2f newVelocity = spawnedAsteroids[i]->newVelocity(collidibleAsteroids);
			if (newVelocity != spawnedAsteroids[i]->getVelocity()) {
				newVelocities[spawnedAsteroids[i]] = newVelocity;
			}
			else {
				newVelocities[spawnedAsteroids[i]] = spawnedAsteroids[i]->getVelocity();
			}			
		}
	}
	for (auto a:newVelocities)
		a.first->setVelocity(a.second);
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
	if (!player.isSpaceshipHit()) {
		if (playerForward)
			window.draw(player.getSpaceship()->getEngineFlame());
		window.draw(*player.getSpaceship());
	}
	if (player.isSpaceshipHit()) {
		window.draw(player.getSpaceship()->getExplosion());
	}
	window.draw(lives);
	window.draw(score);
}