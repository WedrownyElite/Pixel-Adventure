#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

class MathFunctions {
public:
	float angleDifference(float angle_1, float angle_2);
	olc::vf2d GetWorldMousePos(olc::TileTransformedView& tv, olc::PixelGameEngine* pge);
	float PointTo(olc::vf2d pos1, olc::vf2d pos2);
};