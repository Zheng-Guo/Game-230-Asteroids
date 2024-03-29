#pragma once
#include <SFML\Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "GameConstants.h"
#include "MainMenu.h"
#include "Level.h"
#include "Help.h"
#include "GameOver.h"

using namespace sf;
using namespace std;

class Asteroids {
private:
	RenderWindow window;
	View initialView;
	MainMenu mainMenu;
	Level level;
	Help helpScreen;
	GameOver gameOver;
	Clock clock;
	Time time1, time2, time3;
	Interface currentInterface;
public:
	Asteroids() : window(VideoMode(Window_Width, Window_Height), "Asteroids", Style::Close | Style::Titlebar),
	currentInterface(Interface::MenuInterface){
		window.setPosition(Vector2i(400, 0));
		initialView = window.getDefaultView();
		//View v(FloatRect(450, 450, 100, 100));
		//window.setView(v);
		level.setDisplayWindow(FloatRect(0, 0, Window_Width, Window_Height));
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
			switch (currentInterface) {
			case Interface::MenuInterface:currentInterface = mainMenu.processEvent(event); 
				if (currentInterface == Interface::HelpInterface) helpScreen.resetMainMenu();
				if (currentInterface == Interface::Exit) window.close(); 
				break;
			case Interface::LevelInterface:currentInterface = level.processEvent(event); 
				if (currentInterface == Interface::MenuInterface) { mainMenu.resetMainMenu(); window.setView(initialView); } 
				break;
			case Interface::HelpInterface:currentInterface = helpScreen.processEvent(event);
				if (currentInterface == Interface::MenuInterface)  mainMenu.resetMainMenu(); 
				break;
			case Interface::GameoverInterface:currentInterface = gameOver.processEvent(event); 
				if (currentInterface == Interface::MenuInterface) mainMenu.resetMainMenu(); 
				break;
			}		
		}
		time2 = clock.getElapsedTime();
		time3 = time2 - time1;
		if (time3.asSeconds() >= Refresh_Interval) {
			time1 = time2;
			switch (currentInterface) {
			case Interface::MenuInterface: currentInterface=mainMenu.processAction(); 
				if (currentInterface == Interface::LevelInterface) level.resetLevel(); 
				break;
			case Interface::LevelInterface:currentInterface=level.processAction(); 
				if (currentInterface == Interface::GameoverInterface) gameOver.setScore(level.getScore()); 
				break;
			}
		}
		window.clear();
		switch (currentInterface) {
		case Interface::MenuInterface: mainMenu.render(window); break;
		case Interface::LevelInterface:level.render(window); break;
		case Interface::HelpInterface:helpScreen.render(window); break;
		case Interface::GameoverInterface:gameOver.render(window); break;
		}
		window.display();
	}
}