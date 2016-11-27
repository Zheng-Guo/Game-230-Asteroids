#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <memory>
#include "GameConstants.h"
#include "Player.h"

using namespace sf;
using namespace std;

class BackgroundPanel :public RectangleShape {
public:
	BackgroundPanel(float x = 0, float y = 0, float px = 0, float py = 0) :RectangleShape(Vector2f(x, y)){
	}
};

class Background {
private:
	shared_ptr<BackgroundPanel> backgroundPanels[3][3];
	shared_ptr<BackgroundPanel> currentBackgroundPanel;
	FloatRect displayWindow;
	FloatRect innerBound;
	RectangleShape displayCenter;
	Texture backgroundTexture;
public:
	Background():displayCenter(Vector2f(Display_Center_Width,Display_Center_Width)),
	innerBound(Background_Inner_Bound_Left,Background_Inner_Bound_Top,Background_Inner_Bound_Width,Background_Inner_Bound_Heigh){
		backgroundTexture.loadFromFile(Background_Texture_File);
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++) {
				backgroundPanels[i][j] = make_shared<BackgroundPanel>(Background_Panel_Width, Background_Panel_Height,Background_Center_Width,Background_Center_Height);
				backgroundPanels[i][j]->setPosition(Current_Background_Panel_Initial_X_Position+(j-1)*Background_Panel_Width,Current_Background_Panel_Initial_Y_Position+(i-1)*Background_Panel_Height);
				backgroundPanels[i][j]->setTexture(&backgroundTexture);
			}
		currentBackgroundPanel = backgroundPanels[1][1];		
	}
	void setDisplayWindow(FloatRect w) { displayWindow = w; }
	vector<shared_ptr<BackgroundPanel>> getVisiblePanels();
	bool isWithinInnerBound(Spaceship s) { return innerBound.contains(s.getPosition()); }
	Vector2f getShift(Player &player);
	void shiftPanels(Vector2f v);
	void rotatePanels(Vector2f v);
	void resetPanels();
};

vector<shared_ptr<BackgroundPanel>> Background::getVisiblePanels() {
	vector<shared_ptr<BackgroundPanel>> v;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (backgroundPanels[i][j]->getGlobalBounds().intersects(displayWindow))
				v.push_back(backgroundPanels[i][j]);
	return v;
}

Vector2f Background::getShift(Player &player) {
	float x=0, y=0;
	if (player.getSpaceship()->getPosition().x < innerBound.left) {
		x = innerBound.left - player.getSpaceship()->getPosition().x;
		player.setSpaceshipPosition(innerBound.left, player.getSpaceship()->getPosition().y);
	}
	if (player.getSpaceship()->getPosition().x > innerBound.left + innerBound.width) {
		x = innerBound.left + innerBound.width - player.getSpaceship()->getPosition().x;
		player.setSpaceshipPosition(innerBound.left + innerBound.width, player.getSpaceship()->getPosition().y);
	}
	if (player.getSpaceship()->getPosition().y < innerBound.top) {
		y = innerBound.top - player.getSpaceship()->getPosition().y;
		player.setSpaceshipPosition(player.getSpaceship()->getPosition().x,innerBound.top);
	}
	if (player.getSpaceship()->getPosition().y > innerBound.top + innerBound.height) {
		y = innerBound.top + innerBound.height - player.getSpaceship()->getPosition().y;
		player.setSpaceshipPosition(player.getSpaceship()->getPosition().x, innerBound.top + innerBound.height);
	}
	return Vector2f(x, y);
}

void Background::shiftPanels(Vector2f v) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			backgroundPanels[i][j]->move(v);
}

void Background::rotatePanels(Vector2f v) {
	int x=-1, y=-1;
	for (int i=0; i < 3; i++) {
		for (int j=0; j < 3; j++) {
			if (backgroundPanels[i][j]->getGlobalBounds().contains(v)) {
				x = i, y = j;
			}
		}
	}
	if (x != -1 && y != -1&&backgroundPanels[x][y]!=currentBackgroundPanel) {
		if (x == 0) {
			backgroundPanels[2][0]->setPosition(backgroundPanels[2][0]->getPosition().x, backgroundPanels[2][0]->getPosition().y-Background_Panel_Height*3);
			backgroundPanels[2][1]->setPosition(backgroundPanels[2][1]->getPosition().x, backgroundPanels[2][1]->getPosition().y - Background_Panel_Height * 3);
			backgroundPanels[2][2]->setPosition(backgroundPanels[2][2]->getPosition().x, backgroundPanels[2][2]->getPosition().y - Background_Panel_Height * 3);
			shared_ptr<BackgroundPanel> temp;
			temp = backgroundPanels[2][0];
			backgroundPanels[2][0] = backgroundPanels[1][0];
			backgroundPanels[1][0] = backgroundPanels[0][0];
			backgroundPanels[0][0] = temp;
			temp = backgroundPanels[2][1];
			backgroundPanels[2][1] = backgroundPanels[1][1];
			backgroundPanels[1][1] = backgroundPanels[0][1];
			backgroundPanels[0][1] = temp;
			temp = backgroundPanels[2][2];
			backgroundPanels[2][2] = backgroundPanels[1][2];
			backgroundPanels[1][2] = backgroundPanels[0][2];
			backgroundPanels[0][2] = temp;
		}
		if (x == 2) {
			backgroundPanels[0][0]->setPosition(backgroundPanels[0][0]->getPosition().x, backgroundPanels[0][0]->getPosition().y + Background_Panel_Height * 3);
			backgroundPanels[0][1]->setPosition(backgroundPanels[0][1]->getPosition().x, backgroundPanels[0][1]->getPosition().y + Background_Panel_Height * 3);
			backgroundPanels[0][2]->setPosition(backgroundPanels[0][2]->getPosition().x, backgroundPanels[0][2]->getPosition().y + Background_Panel_Height * 3);
			shared_ptr<BackgroundPanel> temp;
			temp = backgroundPanels[0][0];
			backgroundPanels[0][0] = backgroundPanels[1][0];
			backgroundPanels[1][0] = backgroundPanels[2][0];
			backgroundPanels[2][0] = temp;
			temp = backgroundPanels[0][1];
			backgroundPanels[0][1] = backgroundPanels[1][1];
			backgroundPanels[1][1] = backgroundPanels[2][1];
			backgroundPanels[2][1] = temp;
			temp = backgroundPanels[0][2];
			backgroundPanels[0][2] = backgroundPanels[1][2];
			backgroundPanels[1][2] = backgroundPanels[2][2];
			backgroundPanels[2][2] = temp;
		}
		if (y == 0) {
			backgroundPanels[0][2]->setPosition(backgroundPanels[0][2]->getPosition().x-Background_Panel_Width*3, backgroundPanels[0][2]->getPosition().y);
			backgroundPanels[1][2]->setPosition(backgroundPanels[1][2]->getPosition().x - Background_Panel_Width * 3, backgroundPanels[1][2]->getPosition().y);
			backgroundPanels[2][2]->setPosition(backgroundPanels[2][2]->getPosition().x - Background_Panel_Width * 3, backgroundPanels[2][2]->getPosition().y);
			shared_ptr<BackgroundPanel> temp;
			temp = backgroundPanels[0][2];
			backgroundPanels[0][2] = backgroundPanels[0][1];
			backgroundPanels[0][1] = backgroundPanels[0][0];
			backgroundPanels[0][0] = temp;
			temp = backgroundPanels[1][2];
			backgroundPanels[1][2] = backgroundPanels[1][1];
			backgroundPanels[1][1] = backgroundPanels[1][0];
			backgroundPanels[1][0] = temp;
			temp = backgroundPanels[2][2];
			backgroundPanels[2][2] = backgroundPanels[2][1];
			backgroundPanels[2][1] = backgroundPanels[2][0];
			backgroundPanels[2][0] = temp;
		}
		if (y == 2) {
			backgroundPanels[0][0]->setPosition(backgroundPanels[0][0]->getPosition().x + Background_Panel_Width * 3, backgroundPanels[0][0]->getPosition().y);
			backgroundPanels[1][0]->setPosition(backgroundPanels[1][0]->getPosition().x + Background_Panel_Width * 3, backgroundPanels[1][0]->getPosition().y);
			backgroundPanels[2][0]->setPosition(backgroundPanels[2][0]->getPosition().x + Background_Panel_Width * 3, backgroundPanels[2][0]->getPosition().y);
			shared_ptr<BackgroundPanel> temp;
			temp = backgroundPanels[0][0];
			backgroundPanels[0][0] = backgroundPanels[0][1];
			backgroundPanels[0][1] = backgroundPanels[0][2];
			backgroundPanels[0][2] = temp;
			temp = backgroundPanels[1][0];
			backgroundPanels[1][0] = backgroundPanels[1][1];
			backgroundPanels[1][1] = backgroundPanels[1][2];
			backgroundPanels[1][2] = temp;
			temp = backgroundPanels[2][0];
			backgroundPanels[2][0] = backgroundPanels[2][1];
			backgroundPanels[2][1] = backgroundPanels[2][2];
			backgroundPanels[2][2] = temp;
		}
		currentBackgroundPanel = backgroundPanels[x][y];
	}
}

void Background::resetPanels() {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++) {
			backgroundPanels[i][j]->setPosition(Current_Background_Panel_Initial_X_Position + (j - 1)*Background_Panel_Width, Current_Background_Panel_Initial_Y_Position + (i - 1)*Background_Panel_Height);
		}
	currentBackgroundPanel = backgroundPanels[1][1];
}