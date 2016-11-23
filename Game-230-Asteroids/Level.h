#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
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
	Background background;
	Player player;
	bool playerForward,playerBackward,playerLeft, playerRight;
public:
	Level(){
		player.setSpaceshipPosition(Window_Width / 2, Window_Height / 2);
	}
	void setDisplayWindow(FloatRect w) { background.setDisplayWindow(w); }
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
	}
}

void Level::render(RenderWindow &window) {
	vector<shared_ptr<BackgroundPanel>> visiblePanels = background.getVisiblePanels();
	for (shared_ptr<BackgroundPanel> p : visiblePanels)
		window.draw(*p);
	if (playerForward)
		window.draw(player.getSpaceship().getEngineFlame());
	window.draw(player.getSpaceship());
}