#pragma once
#include "olcPixelGameEngine.h"


class EnemyFunctions {
private:
	float JumpHeight = 1.0f;
	float KnockbackSpeed = 12.0f;
	float JumpSpeed = 6.0f;
public:
	float ReturnDistTraveled(float DistTraveled, float fElapsedTime);
	int ReturnHit(float DistTraveled, int Hit);
	olc::vf2d Knockback(olc::vf2d PlayerPos, olc::vf2d EnemyPos, float fElapsedTime);

};
