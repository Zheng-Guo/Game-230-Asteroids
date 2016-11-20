#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <memory>
#include "GameConstants.h"

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
	RectangleShape displayCenter;
	Texture backgroundTexture;
public:
	Background():displayCenter(Vector2f(Display_Center_Width,Display_Center_Width)){
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
	void shiftPanels(float x, float y);
};


vector<shared_ptr<BackgroundPanel>> Background::getVisiblePanels() {
	vector<shared_ptr<BackgroundPanel>> v;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (backgroundPanels[i][j]->getGlobalBounds().intersects(displayWindow))
				v.push_back(backgroundPanels[i][j]);
	return v;
}

void Background::shiftPanels(float x, float y) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			backgroundPanels[i][j]->move(x, y);
}