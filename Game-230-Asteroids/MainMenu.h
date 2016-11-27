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
	Font font;
	vector<Text> optionText;
	Text instruction1, instruction2;
	RectangleShape optionHighlight;
	int currentSelection;
	int startingCounter;
	bool startingGame;
	MenuOption option;
public:
	MainMenu() :background(Vector2f(Window_Width, Window_Height)),
	foreground(Vector2f(Main_Menu_Foreground_Width, Main_Menu_Foreground_Height)),
	optionHighlight(Vector2f(200,80)),
	currentSelection(0),
	startingCounter(0),
	startingGame(false){
		backgroundTexture.loadFromFile(Background_Texture_File);
		foregroundTexture.loadFromFile(Main_Menu_Foreground_Texture);
		background.setTexture(&backgroundTexture);
		foreground.setTexture(&foregroundTexture);
		foreground.setPosition(Main_Menu_Foreground_X, Main_Menu_Foreground_Y);
		optionHighlight.setFillColor(Color::Green);
		font.loadFromFile("Tinos-Regular.ttf");
		Text text;
		text.setFont(font);
		text.setString("Start Game");
		text.setCharacterSize(Menu_Option_Character_Size);
		text.setPosition(Menu_Option_X, Menu_Option_Initial_Y+optionText.size()*Menu_Option_Interval);
		optionText.push_back(text);
		text.setString("Quit");
		text.setPosition(Menu_Option_X, Menu_Option_Initial_Y + optionText.size()*Menu_Option_Interval);
		optionText.push_back(text);
		instruction1.setFont(font);
		instruction1.setString("Press Up/Down arrow key to navigate between options");
		instruction1.setCharacterSize(Stat_Character_Size);
		instruction1.setFillColor(Color::Yellow);
		instruction1.setPosition(Menu_Instruction_X, Menu_Instruction_Y);
		instruction2.setFont(font);
		instruction2.setString("Press Space to select option");
		instruction2.setCharacterSize(Stat_Character_Size);
		instruction2.setFillColor(Color::Yellow);
		instruction2.setPosition(Menu_Instruction_X, Menu_Instruction_Y+30);
	}
	int getSelectionOption() { return currentSelection; }
	Interface processEvent(Event event);
	Interface processAction();
	void render(RenderWindow &window);
	void resetMainMenu();
};

Interface MainMenu::processEvent(Event event) {
	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		if (currentSelection > 0)
			--currentSelection;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down)) {
		if (currentSelection < optionText.size()-1)
			++currentSelection;		
	}
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		if (currentSelection == MenuOption::StartGame) {
			startingGame = true;
		}
		if (currentSelection == MenuOption::Quit)
			return Interface::Exit;
	}
	return Interface::MenuInterface;
}

Interface MainMenu::processAction() {
	if (startingGame) {
		if (startingCounter >= Refresh_Frequency && startingCounter < Refresh_Frequency/2*3) {
			foreground.move(Menu_Phase1_Velocity);
		}
		else if (startingCounter >= Refresh_Frequency/2*3&&startingCounter < Refresh_Frequency *2) {
			foreground.move(Menu_Phase2_Velocity);
		}
		++startingCounter;
		if (startingCounter == Refresh_Frequency *2) {
			startingCounter = 0;
			return Interface::LevelInterface;
		}
	}
	return Interface::MenuInterface;
}

void MainMenu::render(RenderWindow &window) {
	window.draw(background);
	window.draw(foreground);
	if (!startingGame) {
		for (int i = 0; i < optionText.size(); ++i) {
			if (i == currentSelection)
				optionText[i].setFillColor(Color::Red);
			else
				optionText[i].setFillColor(Color::White);
			window.draw(optionText[i]);
		}
		window.draw(instruction1);
		window.draw(instruction2);
	}
}

void MainMenu::resetMainMenu() {
	currentSelection = 0;
	startingGame = false;
	foreground.setPosition(Main_Menu_Foreground_X, Main_Menu_Foreground_Y);
}