#pragma once
#include "olcPixelGameEngine.h"


class EnemyFunctions {
public:
	static float JumpHeight;
	static float KnockbackSpeed;
	static float JumpSpeed;

	static float ReturnDistTraveled(float DistTraveled, float fElapsedTime);
	static int ReturnHit(float DistTraveled, int Hit);
	static olc::vf2d Knockback(olc::vf2d PlayerPos, olc::vf2d EnemyPos, float fElapsedTime);

};
