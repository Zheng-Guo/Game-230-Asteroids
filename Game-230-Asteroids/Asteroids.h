#pragma once
#include <SFML\Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "GameConstants.h"
#include "Level.h"

using namespace sf;
using namespace std;

class Asteroids {
private:
	RenderWindow window;
	Level level;
	Clock clock;
	Time time1, time2, time3;
public:
	Asteroids() : window(VideoMode(Window_Width, Window_Height), "Asteroids", Style::Close | Style::Titlebar) {
		window.setPosition(Vector2i(400, 0));
		srand(time(NULL));
	}
	void startGame();
};

void Asteroids::startGame() {
	time1 = clock.getElapsedTime();
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
			level.processEvent(event);
		}
		time2 = clock.getElapsedTime();
		time3 = time2 - time1;
		if (time3.asSeconds() >= Refresh_Interval) {
			time1 = time2;
			level.processAction();
		}
		window.clear();
		level.render(window);
		window.display();
	}
}