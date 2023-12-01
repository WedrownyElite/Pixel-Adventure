#include "MathFunctions.h"

float MathFunctions::angleDifference(float angle_1, float angle_2) {
	angle_1 = fmod(angle_1, 2 * 3.14159f);
	angle_2 = fmod(angle_2, 2 * 3.14159f);
	float angle_diff = angle_1 - angle_2;

	if (angle_diff > 3.14159f)
		angle_diff -= 2 * 3.14159f;
	else if (angle_diff < -3.14159f)
		angle_diff += 2 * 3.14159f;

	return -angle_diff;
}
olc::vf2d MathFunctions::GetWorldMousePos(olc::TileTransformedView& tv, olc::PixelGameEngine* pge) {
	return pge->GetMousePos() / 32 + tv.GetWorldOffset();
}
olc::vf2d MathFunctions::GetPlayerPos(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, olc::vf2d PlayerPos) {
	return PlayerPos / 32 + tv.GetWorldOffset();
}
float MathFunctions::PointTo(olc::vf2d pos1, olc::vf2d pos2) {
	return atan2(pos2.y - pos1.y, pos2.x - pos1.x);
}
