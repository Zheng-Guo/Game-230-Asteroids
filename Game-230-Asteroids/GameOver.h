#pragma once
#include <sstream>
#include <SFML\Graphics.hpp>
#include "GameConstants.h"

using namespace sf;
using namespace std;

class GameOver {
private:
	RectangleShape background;
	RectangleShape foreground;
	Texture backgroundTexture, foregroundTexture;
	Font font;
	Text score;
	Text instruction;
	int playerScore;
public:
	GameOver() :background(Vector2f(Window_Width, Window_Height)),
		foreground(Vector2f(Window_Width, Window_Height)){
		backgroundTexture.loadFromFile(Background_Texture_File);
		foregroundTexture.loadFromFile(Game_Over_Foreground_Texture);
		background.setTexture(&backgroundTexture);
		foreground.setTexture(&foregroundTexture);
		font.loadFromFile("Tinos-Regular.ttf");
		ostringstream ss;
		ss << "Final Score: " << playerScore;
		score.setString(ss.str());
		score.setFont(font);
		score.setCharacterSize(Game_Over_Score_Character_Size);
		score.setFillColor(Color::Yellow);
		score.setPosition(Game_Over_Score_X, Game_Over_Score_Y);
		instruction.setFont(font);
		instruction.setString("Press Esc to return to main menu");
		instruction.setCharacterSize(Stat_Character_Size);
		instruction.setFillColor(Color::Yellow);
		instruction.setPosition(Menu_Instruction_X, Menu_Instruction_Y);
	}
	void setScore(int s) { playerScore = s; }
	Interface processEvent(Event event);
	void render(RenderWindow &window);
};

Interface GameOver::processEvent(Event event) {
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		return Interface::MenuInterface;
	}
	return Interface::GameoverInterface;
}

void GameOver::render(RenderWindow &window) {
	window.draw(background);
	window.draw(foreground);
	window.draw(score);
	window.draw(instruction);
}