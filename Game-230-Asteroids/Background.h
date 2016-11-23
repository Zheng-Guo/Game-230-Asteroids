#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <memory>
#include "GameConstants.h"
#include "Player.h"

using namespace sf;
using namespace std;

class BackgroundPanel :public RectangleShape {
private:
	RectangleShape panelCenter;
public:
	BackgroundPanel(float x = 0, float y = 0, float px = 0, float py = 0) :RectangleShape(Vector2f(x, y)),
		panelCenter(Vector2f(px, py)) {
		panelCenter.setPosition(getPosition().x + x / 2 - px / 2, getPosition().y + y / 2 - py / 2);
	}
	bool enterCenter(float x, float y) { return panelCenter.getGlobalBounds().contains(x, y); }
	void setPosition(float x, float y);
};

void BackgroundPanel::setPosition(float x, float y) {
	RectangleShape::setPosition(x, y);
	panelCenter.setPosition(x+getSize().x/2 - panelCenter.getSize().x / 2, y+getSize().y/2 - panelCenter.getSize().y / 2);
}

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
	if (player.getSpaceship().getPosition().x < innerBound.left) {
		x = innerBound.left - player.getSpaceship().getPosition().x;
		player.setSpaceshipPosition(innerBound.left, player.getSpaceship().getPosition().y);
	}
	if (player.getSpaceship().getPosition().x > innerBound.left + innerBound.width) {
		x = innerBound.left + innerBound.width - player.getSpaceship().getPosition().x;
		player.setSpaceshipPosition(innerBound.left + innerBound.width, player.getSpaceship().getPosition().y);
	}
	if (player.getSpaceship().getPosition().y < innerBound.top) {
		y = innerBound.top - player.getSpaceship().getPosition().y;
		player.setSpaceshipPosition(player.getSpaceship().getPosition().x,innerBound.top);
	}
	if (player.getSpaceship().getPosition().y > innerBound.top + innerBound.height) {
		y = innerBound.top + innerBound.height - player.getSpaceship().getPosition().y;
		player.setSpaceshipPosition(player.getSpaceship().getPosition().x, innerBound.top + innerBound.height);
	}
	return Vector2f(x, y);
}

void Background::shiftPanels(Vector2f v) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			backgroundPanels[i][j]->move(v);
}