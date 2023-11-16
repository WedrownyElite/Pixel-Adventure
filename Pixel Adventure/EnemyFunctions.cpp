#include "EnemyFunctions.h"

float EnemyFunctions::ReturnDistTraveled(float DistTraveled, float fElapsedTime) {
	float DistThisFrame = KnockbackSpeed * fElapsedTime;
	DistTraveled += DistThisFrame;

	return DistTraveled;
}
int EnemyFunctions::ReturnHit(float DistTraveled, int Hit) {
	if (DistTraveled >= 2.0f) {
		Hit = 0;
	}
	return Hit;
}
olc::vf2d EnemyFunctions::Knockback(olc::vf2d PlayerPos, olc::vf2d EnemyPos, float fElapsedTime) {

	olc::vf2d dir = (PlayerPos - EnemyPos).norm();

	float DistThisFrame = KnockbackSpeed * fElapsedTime;

	EnemyPos -= dir * DistThisFrame;

	return EnemyPos;

}