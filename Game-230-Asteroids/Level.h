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
#include "PowerUp.h"
#include "AIPlayer.h"

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
	vector<PowerUp> powerUps;
	int asteroidNumberIncrement;
	int asteroidSpawningCounter;	
	Font font;
	Text lives;
	Text score;
	Player player;
	bool startingGame;
	int startingCounter;
	View view;
	bool playerForward,playerBackward,playerLeft, playerRight;
	bool fireGun;
	Text gameEndMessage, gameEndInstruction;
	bool levelClear, gameOver;
	int levelEndCounter;
	vector<RectangleShape> blackCurtains;
	vector<RectangleShape> displayedBlackCurtains;
	SoundBuffer levelClearSoundBuffer;
	Sound levelClearSound;
	Texture missileSymbleTexture;
	RectangleShape missileSymble;
	AIPlayer AI;
	int enemySpawnCounter;
	int enemySpawnTime;
	bool enemySpawned;
	void initializeAsteroids();
	void spawnAsteroid();
	void spawnAsteroids();
	void rebucket();
	set<shared_ptr<Asteroid>> getCollidibleAsteroids();
	set<shared_ptr<Asteroid>> getTargetAsteroids(shared_ptr<CircleShape> g);
	set<shared_ptr<Asteroid>> getVisibleTargets();
	bool spaceshipCollision();
public:
	Level() :spawnBound(Spawn_Bound_Left, Spawn_Bound_Top, Spawn_Bound_Width, Spawn_Bound_Height),
	asteroidNumberIncrement(0),
	asteroidSpawningCounter(0),
	startingGame(true),
	startingCounter(0),
	levelClear(false),
	gameOver(false),
	levelEndCounter(0),
	enemySpawnCounter(0),
	enemySpawned(false),
	view(FloatRect(Level_Initial_View_X,Level_Initial_View_Y,Level_Initial_View_Width,Level_Initial_View_Height)),
	missileSymble(Vector2f(Missile_Symble_Width,Missile_Symble_Height)){
		player.setSpaceshipPosition(Window_Width / 2, Window_Height / 2);
		player.setLives(3);
		player.setScore(0);
		Asteroid::loadTextures();
		Asteroid::loadSounds();
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
		gameEndMessage.setFont(font);
		gameEndMessage.setCharacterSize(Game_End_Message_Character_Size);
		gameEndMessage.setFillColor(Color::White);
		gameEndMessage.setPosition(Game_End_Message_X,Game_End_Message_Y);
		gameEndInstruction.setFont(font);
		gameEndInstruction.setCharacterSize(Game_End_Instruction_Character_Size);
		gameEndInstruction.setFillColor(Color::White);
		gameEndInstruction.setPosition(Game_End_Instruction_X, Game_End_Instruction_Y);
		for (int i = 0; i < Black_Curtain_Row_Number;++i) {
			for (int j = 0; j < Black_Curtain_Column_Number; ++j) {
				RectangleShape r(Vector2f(Black_Curtain_Width, Black_Curtain_Height));
				r.setFillColor(Color::Black);
				r.setPosition(i*Black_Curtain_Width, j*Black_Curtain_Height);
				blackCurtains.push_back(r);
			}
		}
		levelClearSoundBuffer.loadFromFile(Level_Clear_Sound);
		levelClearSound.setBuffer(levelClearSoundBuffer);
		PowerUp::loadTextures();
		missileSymbleTexture.loadFromFile(Missile_Texture);
		missileSymble.setTexture(&missileSymbleTexture);
		missileSymble.setPosition(Missile_Symble_X, Missile_Symble_Y);
		//AI.setSpaceshipPosition(300, 300);
		//AI.setIsEngineOn(true);
		AI.setIsHit(true);
		AI.setPlayerSpaceship(player.getSpaceship());
		view.setRotation(90);
		srand(time(NULL));
		enemySpawnTime = Enemy_Spaceship_Spawn_Time + rand() % Enemy_Spaceship_Spawn_Time_Margin;
	}
	void setDisplayWindow(FloatRect w) { background.setDisplayWindow(w); }
	Interface processEvent(Event event);
	Interface processAction();
	void render(RenderWindow &window);
	void resetLevel();
	int getScore() { return player.getScore(); }
	void nextLevel();
	void spawnEnemySpaceship();
};

void Level::initializeAsteroids() {
	asteroids.clear();
	spawnedAsteroids.clear();
	bucketAllocations.clear();
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
	int i = 0,spawnedLargeAsteroids=0;
	while (i < spawnedAsteroids.size()) {
		if (!spawnBound.intersects(spawnedAsteroids[i]->getGlobalBounds())) {
			asteroids.push_back(spawnedAsteroids[i]);
			spawnedAsteroids.erase(spawnedAsteroids.begin() + i);
		}
		else {
			if (spawnedAsteroids[i]->getSize() == AsteroidSize::Large)
				++spawnedLargeAsteroids;
			++i;
		}
	}
	if (spawnedLargeAsteroids < Minimum_Spawn_Asteroid_Number&&asteroids.size()>0) {
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
	if (!startingGame) {
		if (asteroidSpawningCounter < Refresh_Frequency / 2)
			asteroidSpawningCounter++;
		else {
			asteroidSpawningCounter = 0;
			spawnAsteroid();
		}
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

set<shared_ptr<Asteroid>> Level::getTargetAsteroids(shared_ptr<CircleShape> g) {
	set<shared_ptr<Asteroid>> targets;
	int i = (g->getPosition().x - Spawn_Bound_Left) / Bucket_Grid_Width, j = (g->getPosition().y - Spawn_Bound_Top) / Bucket_Grid_Height;
	targets.insert(bucketGrid[j][i].begin(), bucketGrid[j][i].end());
	bool overlapTop = false, overlapBottom = false, overlapLeft = false, overlapRight = false;
	if (i > 1 && g->getPosition().x - g->getRadius() < (i - 1)*Bucket_Grid_Width)
		overlapLeft = true;
	if (i < Bucket_Grid_Column_Number - 2 && g->getPosition().x + g->getRadius() > i*Bucket_Grid_Width)
		overlapRight = true;
	if (j > 1 && g->getPosition().y - g->getRadius() < (j - 1)*Bucket_Grid_Height)
		overlapTop = true;
	if (j < Bucket_Grid_Row_Number - 2 && g->getPosition().y + g->getRadius() > j*Bucket_Grid_Height)
		overlapBottom = true;
	if (overlapTop) {
		targets.insert(bucketGrid[j - 1][i].begin(), bucketGrid[j - 1][i].end());
	}
	if (overlapBottom) {
		targets.insert(bucketGrid[j + 1][i].begin(), bucketGrid[j + 1][i].end());
	}
	if (overlapLeft) {
		targets.insert(bucketGrid[j][i - 1].begin(), bucketGrid[j][i - 1].end());
	}
	if (overlapRight) {
		targets.insert(bucketGrid[j][i + 1].begin(), bucketGrid[j][i + 1].end());
	}
	if (overlapTop&&overlapLeft) {
		targets.insert(bucketGrid[j - 1][i - 1].begin(), bucketGrid[j - 1][i - 1].end());
	}
	if (overlapTop&&overlapRight) {
		targets.insert(bucketGrid[j - 1][i + 1].begin(), bucketGrid[j - 1][i + 1].end());
	}
	if (overlapBottom&&overlapLeft) {
		targets.insert(bucketGrid[j + 1][i - 1].begin(), bucketGrid[j + 1][i - 1].end());
	}
	if (overlapBottom&&overlapRight) {
		targets.insert(bucketGrid[j + 1][i + 1].begin(), bucketGrid[j + 1][i + 1].end());
	}
	return targets;
}

set<shared_ptr<Asteroid>> Level::getVisibleTargets() {
	set<shared_ptr<Asteroid>> targets;
	for (int i = 1; i < Bucket_Grid_Row_Number - 1; ++i)
		for (int j = 1; j < Bucket_Grid_Column_Number - 1; ++j)
			targets.insert(bucketGrid[i][j].begin(), bucketGrid[i][j].end());
	return targets;
}

bool Level::spaceshipCollision() {
	set<shared_ptr<Asteroid>> collidibleAsteroids = getCollidibleAsteroids();
	shared_ptr<Spaceship> spaceship = player.getSpaceship();
	for (shared_ptr<Asteroid> a : collidibleAsteroids) {
		if (!a->getIsHit()) {
			Vector2f offset = a->getPosition() - spaceship->getPosition();
			float distance = sqrt(offset.x*offset.x + offset.y*offset.y);
			if (distance < a->getRadius() + spaceship->getRadius())
				return true;
		}
	}
	Vector2f offset = AI.getSpaceship()->getPosition() - spaceship->getPosition();
	float distance = sqrt(offset.x*offset.x + offset.y*offset.y);
	if (distance <  AI.getSpaceship()->getRadius() + spaceship->getRadius())
		return true;
	for (shared_ptr<GunShot> g : AI.getSpaceship()->getGunShots()) {
		if (g->getFired()) {
			Vector2f offset = g->getPosition() - spaceship->getPosition();
			float distance = sqrt(offset.x*offset.x + offset.y*offset.y);
			if (distance < g->getRadius() + spaceship->getRadius())
				return true;
		}
	}
	return false;
}

Interface Level::processEvent(Event event) {
	playerForward=false,playerBackward=false,playerLeft = false, playerRight = false;
	fireGun = false;
	if (Keyboard::isKeyPressed(Keyboard::Up)&&!startingGame&&!levelClear)
		playerForward = true;
	if (Keyboard::isKeyPressed(Keyboard::Down) && !startingGame && !levelClear)
		playerBackward = true;
	if (Keyboard::isKeyPressed(Keyboard::Left) && !startingGame && !levelClear)
		playerLeft = true;
	if (Keyboard::isKeyPressed(Keyboard::Right) && !startingGame && !levelClear)
		playerRight = true;
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		return Interface::MenuInterface;
	}
	if (Keyboard::isKeyPressed(Keyboard::Space) && !startingGame && !player.isSpaceshipHit()&&!player.getIsInvincible() && !levelClear)
		fireGun = true;
	if (Keyboard::isKeyPressed(Keyboard::M) && !startingGame && !player.isSpaceshipHit() && !player.getIsInvincible() && !levelClear)
		player.launchMissile(getVisibleTargets());
	return Interface::LevelInterface;
}

Interface Level::processAction() {
	if (playerForward)
		player.moveForward();
	else
		player.resetSpaceshipEngineSound();
	//if (playerBackward)
	//	player.moveBackward();
	if (playerLeft)
		player.turnLeft();
	if (playerRight)
		player.turnRight();
	if (fireGun)
		player.fireFun();
	for (shared_ptr<Asteroid> a : spawnedAsteroids) {
		if (a->getIsHit())
			a->explode();
	}
	int i = 0;
	while (i < spawnedAsteroids.size()) {
		if (spawnedAsteroids[i]->getIsSplit()) {
			vector<shared_ptr<Asteroid>> childAsteroids = spawnedAsteroids[i]->damage();
			for (shared_ptr<Asteroid> a : childAsteroids)
				spawnedAsteroids.push_back(a);
		}
		if (spawnedAsteroids[i]->getIsDestroyed()) {
			PowerUp p;
			int r = rand() % Power_Up_Spawn_Probability;
			switch (r) {
			case Power_Up_Spawn_Life_Pack: p=PowerUp(Power_Up_Size, PowerUpType::LifePack); p.setPosition(spawnedAsteroids[i]->getPosition()); powerUps.push_back(p); break;
			case Power_Up_Spawn_Missile_Pack:p = PowerUp(Power_Up_Size, PowerUpType::MissilePack); p.setPosition(spawnedAsteroids[i]->getPosition()); powerUps.push_back(p); break;
			}
			spawnedAsteroids.erase(spawnedAsteroids.begin() + i);
		}
		else
			++i;
	}
	i = 0;
	while (i < powerUps.size()) {
		if (powerUps[i].getExpired())
			powerUps.erase(powerUps.begin() + i);
		else {
			int p=powerUps[i].interact(player);
			if (p == 1) {
				ostringstream ss;
				ss << "Life: " << player.getLives();
				lives.setString(ss.str());
			}
			powerUps[i].expiring();			
			++i;
		}
	}
	if (AI.isSpaceshipHit()) {
		AI.explode();
	}
	if (!startingGame&&!enemySpawned)
		spawnEnemySpaceship();
	if (enemySpawned&&!AI.getIsHit()&&!AI.isSpaceshipHit()) {
		AI.recalibrate();
		AI.navigate();
		AI.moveForward();
		AI.getSpaceship()->move();
		FloatRect fireRange(Enemy_Spaceship_Fire_Range_X, Enemy_Spaceship_Fire_Range_Y, Enemy_Spaceship_Fire_Range_Width, Enemy_Spaceship_Fire_Range_Height);
		if (!player.isSpaceshipHit()&&!gameOver&&fireRange.contains(AI.getSpaceship()->getPosition())&&AI.isWithinFireRange())
			AI.fireFun();
	}
	if (spawnedAsteroids.size() == 0 && asteroids.size() == 0&&!levelClear) {
		levelClearSound.play();
		levelClear = true;
		fireGun = false;
		gameEndMessage.setString("Level Clear");
		gameEndInstruction.setString("Proceed to next level");
	}	
	if (player.isGameOver() && !gameOver) {
		gameOver = true;
		gameEndMessage.setString("Game Over");
		gameEndInstruction.setString("Please restart the game");
	}
	player.getSpaceship()->recycleGunShots(spawnBound);
	player.getSpaceship()->recycleMissiles(spawnBound);
	AI.getSpaceship()->recycleGunShots(spawnBound);
	FloatRect visibleArea(0, 0, Window_Width, Window_Height);
	for (shared_ptr<GunShot> g : player.getSpaceship()->getGunShots()) {
		if (visibleArea.intersects(g->getGlobalBounds())) {
			shared_ptr<Asteroid> target = g->target(getTargetAsteroids(g));
			if (target != nullptr&&!target->getIsHit()&&!target->getIsDestroyed()) {
				g->setFired(false);
				player.addScore(g->hitTarget(target));
				ostringstream ss;
				ss << "Score: " << player.getScore();
				score.setString(ss.str());
			}			
			if (AI.getHit(g)) {
				g->setFired(false);
				AI.loseLife();
				player.addScore(AI.getScore());
				ostringstream ss;
				ss << "Score: " << player.getScore();
				score.setString(ss.str());
			}
		}
	}
	for (shared_ptr<Missile> m : player.getSpaceship()->getMissiles()) {
		if (visibleArea.intersects(m->getGlobalBounds())) {
			shared_ptr<Asteroid> target = m->target(getTargetAsteroids(m));
			if (target != nullptr && !target->getIsHit() && !target->getIsDestroyed()) {
				m->setFired(false);
				player.addScore(m->hitTarget(target));
				ostringstream ss;
				ss << "Score: " << player.getScore();
				score.setString(ss.str());
			}
		}
	}
	if (player.getSpaceship()->getMissileLaunched()) {
		player.getSpaceship()->launchMissiles();
	}
	for (shared_ptr<GunShot> g : player.getSpaceship()->getGunShots())
		if(g->getFired())
			g->move();
	for (shared_ptr<GunShot> g : AI.getSpaceship()->getGunShots())
		if (g->getFired())
			g->move();
	for (shared_ptr<Missile> m : player.getSpaceship()->getMissiles()) {
		if (m->getFired()) {
			m->navigate();
			m->moveForward();
			m->move();
		}
	}
	if (!player.isSpaceshipHit()&&!levelClear&&!gameOver) {
		player.act();
		if (!background.isWithinInnerBound(*player.getSpaceship())) {
			Vector2f shift = background.getShift(player);
			background.shiftPanels(shift*Background_Shift_Parallax_Coefficient);
			background.rotatePanels(player.getSpaceship()->getPosition());
			for (shared_ptr<Asteroid> a : spawnedAsteroids)
				a->shiftPosition(shift);
			for (shared_ptr<GunShot> g : player.getSpaceship()->getGunShots())
				g->shift(shift);
			for (shared_ptr<Missile> m : player.getSpaceship()->getMissiles())
				m->shift(shift);
			for (int i=0;i<powerUps.size();++i)
				powerUps[i].shift(shift);
			if (enemySpawned && !AI.isSpaceshipHit())
				AI.getSpaceship()->shift(shift);
			for (shared_ptr<GunShot> g : AI.getSpaceship()->getGunShots())
				g->shift(shift);
		}
	}
	rebucket();
	if (startingGame) {
		view.rotate(-30.0/Refresh_Frequency);
		view.setSize(Level_Initial_View_Width+(Window_Width - Level_Initial_View_Width) / Refresh_Frequency / 3 * startingCounter, Level_Initial_View_Height+(Window_Height - Level_Initial_View_Height) / Refresh_Frequency / 3 * startingCounter);
		if (startingCounter >= Refresh_Frequency * 3) {
			startingCounter = 0;
			view.setRotation(0);
			view.setSize(Window_Width, Window_Height);
			startingGame = false;
		}
		++startingCounter;
	}
	else if (player.isSpaceshipHit()) {
		player.explode();
	}
	else if (levelClear) {
		if (levelEndCounter < Refresh_Frequency * 3) {
			++levelEndCounter;
		}
		else {
			if (blackCurtains.size() > 0) {
				int i = rand() % blackCurtains.size();
				displayedBlackCurtains.push_back(blackCurtains[i]);
				blackCurtains.erase(blackCurtains.begin() + i);
			}
			else {
				nextLevel();
			}
		}
	}
	else if (gameOver) {
		if (levelEndCounter < Refresh_Frequency * 3) {
			++levelEndCounter;
		}
		else {
			if (blackCurtains.size() > 0) {
				int i = rand() % blackCurtains.size();
				displayedBlackCurtains.push_back(blackCurtains[i]);
				blackCurtains.erase(blackCurtains.begin() + i);
			}
			else {
				return Interface::GameoverInterface;
			}
		}
	}
	else if (player.isNextLifeUsed()) {
		player.prepareForBattle();
	}
	else if (!player.getIsInvincible()&&spaceshipCollision()) {
		player.loseLife();
		ostringstream ss;
		ss << "Life: " << player.getLives();
		lives.setString(ss.str());
	}
	map<shared_ptr<Asteroid>,Vector2f> newVelocities;
	map<shared_ptr<Asteroid>, float> stepBackFactors;
	for (int i = 0; i < spawnedAsteroids.size()&&i<bucketAllocations.size();i++) {
		if (!spawnedAsteroids[i]->getIsHit()&&bucketAllocations.find(spawnedAsteroids[i]) != bucketAllocations.end()) {
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
			float stepBackFactor = spawnedAsteroids[i]->stepBack(collidibleAsteroids);
			stepBackFactors[spawnedAsteroids[i]] = stepBackFactor;
		}
	}
	for (auto a : stepBackFactors) {
		if (!a.first->getIsHit())
			a.first->shiftPosition(-a.second*a.first->getVelocity());
	}
	for (auto a:newVelocities)
		if(!a.first->getIsHit())
			a.first->setVelocity(a.second);
	for (shared_ptr<Asteroid> a : spawnedAsteroids) {
		if(!a->getIsHit())
			a->move();
	}	
	spawnAsteroids();
	return Interface::LevelInterface;
}

void Level::render(RenderWindow &window) {
	if (startingGame) {
		window.setView(view);
	}
	vector<shared_ptr<BackgroundPanel>> visiblePanels = background.getVisiblePanels();
	for (shared_ptr<BackgroundPanel> p : visiblePanels)
		window.draw(*p);
	for (shared_ptr<Asteroid> a : spawnedAsteroids) {
		if(!a->getIsHit())
			window.draw(*a);
	}
	for (shared_ptr<Asteroid> a : spawnedAsteroids) {
		if (a->getIsHit()&&!a->getIsDestroyed())
			window.draw(a->getExplosion());
	}		
	for (PowerUp p : powerUps) {
		if(p.getVisible())
			window.draw(p);
	}
	for (shared_ptr<GunShot> g : player.getSpaceship()->getGunShots())
		if (g->getFired())
			window.draw(*g);
	for (shared_ptr<GunShot> g : AI.getSpaceship()->getGunShots())
		if (g->getFired())
			window.draw(*g);
	for (shared_ptr<Missile> m : player.getSpaceship()->getMissiles())
		if (m->getFired()) {
			window.draw(*m);
			window.draw(m->getFlame());
		}
	if (!player.isGameOver()&&!player.isSpaceshipHit()) {
		if (player.isSpaceshipVisible()) {
			if (playerForward)
				window.draw(player.getSpaceship()->getEngineFlame());
			window.draw(*player.getSpaceship());
		}		
	}
	if (player.isSpaceshipHit()) {
		window.draw(player.getSpaceship()->getExplosion());
	}
	if (!AI.getIsHit() && !AI.getIsDestroyed()) {
		if (AI.getIsEngineOn())
			window.draw(AI.getSpaceship()->getEngineFlame());
		window.draw(*AI.getSpaceship());
	}	
	if (AI.isSpaceshipHit()) {
		window.draw(AI.getSpaceship()->getExplosion());
	}
	if (!startingGame) {
		window.draw(lives);
		window.draw(score);
		if (player.getMissileEquipped())
			window.draw(missileSymble);
	}	
	if (levelClear || gameOver) {
		window.draw(gameEndMessage);
		window.draw(gameEndInstruction);
		for (RectangleShape r : displayedBlackCurtains)
			window.draw(r);
	}
}

void Level::resetLevel() {
	player.setSpaceshipPosition(Window_Width / 2, Window_Height / 2);
	player.setSpaceshipRotation(90);
	player.setLives(3);
	player.setScore(0);
	player.reset();
	background.resetPanels();
	initializeAsteroids();
	ostringstream ss;
	ss << "Life: " << player.getLives();
	lives.setString(ss.str());
	ss.str("");
	ss << "Score: " << player.getScore();
	score.setString(ss.str());
	view.setRotation(90);
	view.setSize(Level_Initial_View_Width, Level_Initial_View_Height);
	startingGame = true;
	startingCounter = 0;
	levelClear = false;
	gameOver = false;
	levelEndCounter = 0;
	blackCurtains.insert(blackCurtains.begin(), displayedBlackCurtains.begin(), displayedBlackCurtains.end());
	displayedBlackCurtains.clear();
	powerUps.clear();
	enemySpawnCounter = 0;
	enemySpawned = false;
	AI.reset();
}

void Level::nextLevel() {
	player.setSpaceshipPosition(Window_Width / 2, Window_Height / 2);
	player.setSpaceshipRotation(90);
	player.reset();
	asteroidNumberIncrement += Asteroid_Number_Increment;
	background.resetPanels();
	initializeAsteroids();
	ostringstream ss;
	ss << "Life: " << player.getLives();
	lives.setString(ss.str());
	ss.str("");
	ss << "Score: " << player.getScore();
	score.setString(ss.str());
	view.setRotation(90);
	view.setSize(Level_Initial_View_Width, Level_Initial_View_Height);
	startingGame = true;
	startingCounter = 0;
	levelClear = false;
	gameOver = false;
	levelEndCounter = 0;
	blackCurtains.insert(blackCurtains.begin(), displayedBlackCurtains.begin(), displayedBlackCurtains.end());
	displayedBlackCurtains.clear();
	powerUps.clear();
	enemySpawnCounter = 0;
	enemySpawned = false;
	AI.reset();
}

void Level::spawnEnemySpaceship() {
	if (enemySpawnCounter < enemySpawnTime)
		++enemySpawnCounter;
	else {
		enemySpawned = true;
		AI.setIsHit(false);
		AI.setIsEngineOn(true);
		float x, y;
		int i = rand() % 4;
		if (i == 0) {
			x = Enemy_Spaceship_Spawn_X;
			y = rand() % Enemy_Spaceship_Spawn_Height + Enemy_Spaceship_Spawn_Y;
		}
		else if (i == 1) {
			x = rand() % Enemy_Spaceship_Spawn_Width + Enemy_Spaceship_Spawn_X;
			y = Enemy_Spaceship_Spawn_Y;
		}
		else if (i == 2) {
			x = Enemy_Spaceship_Spawn_X+ Enemy_Spaceship_Spawn_Width;
			y = rand() % Enemy_Spaceship_Spawn_Height + Enemy_Spaceship_Spawn_Y;
		}
		else {
			x = rand() % Enemy_Spaceship_Spawn_Width + Enemy_Spaceship_Spawn_X;
			y = Enemy_Spaceship_Spawn_Y+ Enemy_Spaceship_Spawn_Height;
		}
		AI.setSpaceshipPosition(x, y);
		float deltaX = player.getSpaceship()->getPosition().x-x, deltaY = player.getSpaceship()->getPosition().y-y;
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
			if (deltaX > 0) {
				if (deltaY > 0)
					angle += PI;
				else
					angle -= PI;
			}
		}
		AI.setSpaceshipRotation(angle / Degree_To_Radian);
	}
}