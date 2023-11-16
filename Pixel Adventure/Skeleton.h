#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"

#define EnemyFunctions
#include "EnemyFunctions.h"
#define MathFunctions
#include "MathFunctions.h"
#define GlobalVars
#include "GlobalVars.h"

class Skeleton {
private:
	//Skele variables
	std::vector<olc::vf2d> SkelePos;
	olc::vf2d SkeleSize{ 1.1f, 0.6f };
	//Hit variables
	std::vector<int> SkeleHit;
	std::vector<int> SkeleRedTimer;
	std::vector<float> DistTraveled;
	std::vector<int> JumpBool;
	std::vector<float> JumpDist;
	//Player
	olc::vf2d PlayerSize{ 0.7f, 0.8f };

	olc::vf2d Zero{ 0.0f, 0.0f };

	//Sprites
	std::unique_ptr<olc::Sprite> SkeleRight;
	std::unique_ptr<olc::Sprite> SkeleRightHurt;
	std::unique_ptr<olc::Sprite> Shadow;

	//Decals
	olc::Decal* SkeleRightDecal;
	olc::Decal* SkeleRightHurtDecal;
	olc::Decal* ShadowDecal;
public:
	void Hurt(olc::TileTransformedView& tv, olc::vf2d PlayerPos, int Container, float fElapsedTime);
	void Spawn();
	olc::vf2d ReturnClosestPos(olc::vf2d PlayerPos);
	void Draw(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, olc::vf2d PlayerPos, float PlayerSpeed, bool PlayerCanAttack, float fElapsedTime);
	void SkeleTest(olc::PixelGameEngine* pge, float fElapsedTime);
	void Initialize();
};