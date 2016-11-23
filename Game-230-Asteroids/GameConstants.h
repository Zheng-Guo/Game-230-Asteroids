#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

const int Window_Width = 1000;
const int Window_Height = 1000;
const int Refresh_Frequency = 25;
const float Refresh_Interval = 1.0 / Refresh_Frequency;
const int Background_Panel_Width = 6000;
const int Background_Panel_Height = 6000;
const int Background_Center_Width = 4000;
const int Background_Center_Height = 4000;
const int Background_Center_X_Position = 1000;
const int Background_Center_Y_Position = 1000;
const char Background_Texture_File[] = "textures/background.jpg";
const int Current_Background_Panel_Initial_X_Position = -2500;
const int Current_Background_Panel_Initial_Y_Position = -2500;
const int Display_Center_Width = 600;
const int Display_Center_Height = 600;
const int Display_Center_X_Position = 200;
const int Display_Center_Y_Position = 200;
const int Background_Inner_Bound_Top = 200;
const int Background_Inner_Bound_Left = 200;
const int Background_Inner_Bound_Width = 600;
const int Background_Inner_Bound_Heigh = 600;
const float Background_Shift_Parallax_Coefficient = 0.1;
const float Spaceship_Size = 40;
const float Spaceship_Flame_Width = 25;
const float Spaceship_Flame_Height = 20;
const char Spaceship_Texture[] = "textures/VF31F.png";
const char Spaceship_Engine_Flame_Texture[] = "textures/VF31F Engine Flame.png";
const float Spaceship_Thrust = -0.5;
const float Spaceship_Full_Speed = -8;
const float Spaceship_Angular_Speed = -10;
const float Degree_To_Radian = 3.14159265 / 180;