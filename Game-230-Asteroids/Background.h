#pragma once
#include <SFML\Graphics.hpp>
#include <memory>
#include "GameConstants.h"

using namespace sf;
using namespace std;

class Background {
private:
	shared_ptr<RectangleShape> backgroundPanels[3][3];
	shared_ptr<RectangleShape> currentBackgroundPanel;
	RectangleShape panelCenter;
	RectangleShape displayCenter;
	Texture backgroundTexture;
public:
	Background():panelCenter(Vector2f(Background_Center_Width,Background_Center_Height)),
	displayCenter(Vector2f(Display_Center_Width,Display_Center_Width)){
		backgroundTexture.loadFromFile(Background_Texture_File);
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++) {
				backgroundPanels[i][j] = make_shared<RectangleShape>(Vector2f(Background_Panel_Width, Background_Panel_Height));
				backgroundPanels[i][j]->setPosition(Current_Background_Panel_Initial_X_Position+(j-1)*Background_Panel_Width,Current_Background_Panel_Initial_Y_Position+(i-1)*Background_Panel_Height);
				backgroundPanels[i][j]->setTexture(&backgroundTexture);
			}
		currentBackgroundPanel = backgroundPanels[1][1];
		
	}
};