#pragma once
#include <vector>
#include <fstream>
#include <string>
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
	int startLine, endLine;
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
		ifstream ifs(Help_Text_File);
		string textLine;
		while (getline(ifs, textLine)) {
			Text text;
			text.setFillColor(Color::Red);
			text.setFont(font);
			text.setString(textLine);
			text.setCharacterSize(Help_Text_Character_Size);
			//text.setPosition(Help_Text_X,Help_Text_Initial_Y+Help_Text_Y_Interval*helpText.size());
			helpText.push_back(text);
		}
		startLine = 0;
		endLine = helpText.size()<= Help_Text_Line_Per_Page ?helpText.size() - 1:startLine+ Help_Text_Line_Per_Page-1;
	}
	Interface processEvent(Event event);
	void render(RenderWindow &window);
	void resetMainMenu();
	void pageUp();
	void pageDown();
};

Interface Help::processEvent(Event event) {
	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		pageUp();
	}
	if (Keyboard::isKeyPressed(Keyboard::Down)) {
		pageDown();
	}
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			return Interface::MenuInterface;
	}
	return Interface::HelpInterface;
}

void Help::render(RenderWindow &window) {
	window.draw(background);
	window.draw(foreground);
	for (int i = startLine; i <= endLine; i++) {
		helpText[i].setPosition(Help_Text_X, Help_Text_Initial_Y + Help_Text_Y_Interval*(i - startLine));
		window.draw(helpText[i]);
	}
	window.draw(instruction1);
	window.draw(instruction2);
}

void Help::resetMainMenu() {
	startLine = 0;
	endLine = helpText.size()<= Help_Text_Line_Per_Page ?helpText.size() - 1:startLine+ Help_Text_Line_Per_Page-1;
}

void Help::pageUp() {
	if (startLine > 0) {
		--startLine;
		--endLine;
	}
}

void Help::pageDown() {
	if (endLine < helpText.size() - 1) {
		++startLine;
		++endLine;
	}
}