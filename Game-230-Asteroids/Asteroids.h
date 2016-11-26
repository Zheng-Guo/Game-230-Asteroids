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
	Vertex horizontalVertices[8][2], verticalVertices[8][2];
public:
	Asteroids() : window(VideoMode(Window_Width, Window_Height), "Asteroids", Style::Close | Style::Titlebar) {
		window.setPosition(Vector2i(400, 0));
		//View v(FloatRect(-400, -400, 1800, 1800));
		//window.setView(v);
		level.setDisplayWindow(FloatRect(0, 0, Window_Width, Window_Height));
		srand(time(NULL));
		for (int i = -1; i < 7; i++) {
			horizontalVertices[i][0].position = Vector2f(-200, i * 200);
			horizontalVertices[i][1].position = Vector2f(1200, i * 200);
			verticalVertices[i][0].position = Vector2f(i * 200, -200);
			verticalVertices[i][1].position = Vector2f(i * 200, 1200);
		}
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
		cout << time3.asSeconds() << endl;
		if (time3.asSeconds() >= Refresh_Interval) {
			time1 = time2;
			level.processAction();
		}
		window.clear();
		level.render(window);
		for (int i = 0; i < 8; i++) {
			window.draw(horizontalVertices[i], 2, Lines);
			window.draw(verticalVertices[i], 2, Lines);
		}
		window.display();
	}
}