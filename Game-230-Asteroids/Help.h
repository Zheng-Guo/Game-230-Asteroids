#pragma once
#include <vector>
#include <SFML\Graphics.hpp>
#include "GameConstants.h"

using namespace sf;
using namespace std;

class Help {
private:
	RectangleShape background;
	RectangleShape foreground;
	Texture backgroundTexture, foregroundTexture;
	Font font;
	vector<Text> helpText;
	Text instruction1, instruction2;
	MenuOption option;
public:
	Help() :background(Vector2f(Window_Width, Window_Height)),
	foreground(Vector2f(Help_Foreground_Width, Help_Foreground_Height)) {
		backgroundTexture.loadFromFile(Background_Texture_File);
		foregroundTexture.loadFromFile(Help_Foreground_Texture);
		background.setTexture(&backgroundTexture);
		foreground.setTexture(&foregroundTexture);
		foreground.setPosition(Help_Foreground_X, Help_Foreground_Y);
		font.loadFromFile("Tinos-Regular.ttf");
		instruction1.setFont(font);
		instruction1.setString("Press Up/Down arrow key to scroll up/down");
		instruction1.setCharacterSize(Stat_Character_Size);
		instruction1.setFillColor(Color::Yellow);
		instruction1.setPosition(Menu_Instruction_X, Menu_Instruction_Y);
		instruction2.setFont(font);
		instruction2.setString("Press Esc to return to Main Menu");
		instruction2.setCharacterSize(Stat_Character_Size);
		instruction2.setFillColor(Color::Yellow);
		instruction2.setPosition(Menu_Instruction_X, Menu_Instruction_Y + 30);
	}
	Interface processEvent(Event event);
	void render(RenderWindow &window);
	void resetMainMenu();
};

Interface Help::processEvent(Event event) {
	if (Keyboard::isKeyPressed(Keyboard::Up)) {

	}
	if (Keyboard::isKeyPressed(Keyboard::Down)) {

	}
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			return Interface::MenuInterface;
	}
	return Interface::HelpInterface;
}

void Help::render(RenderWindow &window) {
	window.draw(background);
	window.draw(foreground);
	window.draw(instruction1);
	window.draw(instruction2);
}

void Help::resetMainMenu() {

}