#pragma once
#include <vector>
#include <SFML\Graphics.hpp>
#include "GameConstants.h"

using namespace sf;
using namespace std;

class MainMenu {
private:
	RectangleShape background;
	RectangleShape foreground;
	Texture backgroundTexture, foregroundTexture;
	vector<Text> optionText;
	RectangleShape optionHighlight;
	int currentSelection;
	bool atMainMenu;
public:
	MainMenu() :background(Vector2f(Window_Width, Window_Height)),
	foreground(Vector2f(Main_Menu_Foreground_Width, Main_Menu_Foreground_Height)),
	atMainMenu(true) {
		backgroundTexture.loadFromFile(Background_Texture_File);
		foregroundTexture.loadFromFile(Main_Menu_Foreground_Texture);
		background.setTexture(&backgroundTexture);
		foreground.setTexture(&foregroundTexture);
		foreground.setPosition(Main_Menu_Foreground_X, Main_Menu_Foreground_Y);
	}
	void setAtMainMenu(bool b) { atMainMenu = b; }
	bool getAtMainMenu() { return atMainMenu; }
	void processEvent(Event event);
	void processAction();
	void render(RenderWindow &window);
};

void MainMenu::render(RenderWindow &window) {
	window.draw(background);
	window.draw(foreground);
}