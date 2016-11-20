#pragma once
#include <SFML\Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <memory>
#include "GameConstants.h"
#include "Background.h"
#include "Player.h"

using namespace sf;
using namespace std;

class Level {
private:
	Player player;
	bool playerForward,playerBackward,playerLeft, playerRight;
public:
	Level(){
		player.setSpaceshipPosition(Window_Width / 2, Window_Height / 2);
	}
	void processEvent(Event event);
	void processAction();
	void render(RenderWindow &window);
};

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
	if (playerBackward)
		player.moveBackward();
	if (playerLeft)
		player.turnLeft();
	if (playerRight)
		player.turnRight();
	player.act();
}

void Level::render(RenderWindow &window) {
	window.draw(player.getSpaceship());
}