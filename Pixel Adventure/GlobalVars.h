#pragma once
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

enum GameStateEnum { DEBUG, GAME, PAUSED, EXIT, MENU };
bool PlayerCanAttack = true;
float maxDistance = 2.0f; //How far away the object can be to still be in range.
float maxAngle = M_PI / 4; //The total sweeping angle of the arch in either direction (PI/4 is 45 degrees in either direction, 90 degrees total).