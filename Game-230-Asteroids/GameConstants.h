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
const float Background_Shift_Parallax_Coefficient = 1;
const float Spaceship_Size = 40;
const float Spaceship_Flame_Width = 25;
const float Spaceship_Flame_Height = 20;
const char Spaceship_Texture[] = "textures/VF31F.png";
const char Spaceship_Engine_Flame_Texture[] = "textures/VF31F Engine Flame.png";
const char Spaceship_Explosion_Texture[] = "textures/spaceship_explosion.png";
const int Spaceship_Invinciblility_Duration = Refresh_Frequency * 3;
const int Explosion_Texture_Row_Number = 6;
const int Explosion_Texture_Column_Number = 8;
const int Explosion_Speed = Refresh_Frequency / 25;
const float Spaceship_Thrust = -0.5;
const float Spaceship_Full_Speed = -8;
const float Spaceship_Angular_Speed = -10;
const char Asteroid_Large_Texture[] = "textures/asteroid_large.png";
const char Asteroid_Medium_Texture[] = "textures/asteroid_medium.png";
const char Asteroid_Small_Texture[] = "textures/asteroid_small.png";
const float Asteroid_Large_Radius=60;
const float Asteroid_Medium_Radius = 25;
const float Asteroid_Small_Radius = 10;
const int Asteroid_Minimum_Speed = 1;
const int Asteroid_Maximum_Speed = 3;
const int Asteroid_Minimum_Angular_Velocity = 1;
const int Asteroid_Maximum_Angular_Velocity = 4;
const int Asteroid_Spawn_Initial_Angle_Range = 90;
const float Spawn_Bound_Top = -200;
const float Spawn_Bound_Left = -200;
const int Spawn_Bound_Height = 1400;
const int Spawn_Bound_Width = 1400;
const int Initial_Asteroid_Number = 40;
const int Asteroid_Number_Increment = 10;
const int Minimum_Spawn_Asteroid_Number = 10;
const int Asteroid_Mass_Large = 7;
const int Asteroid_Mass_Medium = 3;
const int Asteroid_Mass_Small = 1;
const float Epsilon = 0.00001;
const int Bucket_Grid_Row_Number = 7;
const int Bucket_Grid_Column_Number = 7;
const int Bucket_Grid_Width = 200;
const int Bucket_Grid_Height = 200;
const int Player_Initial_Life = 3;
const int Stat_Character_Size = 25;
const int Life_X_Position = 10;
const int Life_Y_Position = 10;
const int Score_X_Position = 10;
const int Score_Y_Position = 40;
const float Main_Menu_Foreground_X=50;
const float Main_Menu_Foreground_Y=100;
const float Main_Menu_Foreground_Width = 1275;
const float Main_Menu_Foreground_Height = 675;
const char Main_Menu_Foreground_Texture[] = "textures/VF31F_Flying.png";
const int Menu_Option_Character_Size = 50;
const int Menu_Option_X = 400;
const int Menu_Option_Initial_Y = 300;
const int Menu_Option_Interval = 200;
const int Menu_Instruction_X = 200;
const int Menu_Instruction_Y = 800;
const Vector2f Menu_Phase1_Velocity = Vector2f(30, 30);
const Vector2f Menu_Phase2_Velocity = Vector2f(-400, -200);
const float Level_Initial_View_X = 450;
const float Level_Initial_View_Y = 450;
const float Level_Initial_View_Width = 100;
const float Level_Initial_View_Height = 100;
const char Game_Over_Foreground_Texture[]= "textures/VF31F_Emblem.png";
const int Game_Over_Score_Character_Size = 80;
const int Game_Over_Score_X = 300;
const int Game_Over_Score_Y = 200;
const float Gun_Shot_Size = 10;
const char Gun_Shot_Texutre[] = "textures/VF31F_gunshot.png";
const int Gun_Shot_Number = 10;
const float Gun_Shot_Speed = 40;
const int Gun_Shot_Interval = Refresh_Frequency / 5;
const float PI = 3.14159265;
const float Degree_To_Radian = PI / 180;
enum AsteroidSize{Small,Medium,Large};
enum Interface{ MenuInterface,LevelInterface,GameoverInterface,Exit};
enum MenuOption{StartGame,Quit};