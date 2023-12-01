#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "GlobalVars.h"
#include "EnemyFunctions.h"
#define ANIMATOR_IMPLEMENTATION
#include "olcPGEX_Animator2D.h"
#include "MathFunctions.h"

class Player {
public:

	//False == DrawLeft, True == DrawRight
	bool DrawAnim = false;
	//Sprites
	std::unique_ptr<olc::Sprite> PlayerRight;
	std::unique_ptr<olc::Sprite> PlayerLeft;
	std::unique_ptr<olc::Sprite> PlayerDead;
	std::unique_ptr<olc::Sprite> Shadow;
	std::unique_ptr<olc::Sprite> WalkLeftSS;

	//Decals
	olc::Decal* PlayerRightDecal;
	olc::Decal* PlayerLeftDecal;
	olc::Decal* PlayerDeadDecal;
	olc::Decal* ShadowDecal;
	olc::Decal* WalkLeftDecal;

	olc::vf2d PlayerPos{ 300.0f, 300.0f };
	olc::vf2d MousePos;
	float AttackCooldown = 0.0f;
	bool ArcherDir = true;
	int CharacterHealth = 6;
	float PlayerSpeed;

	olcPGEX_Animator2D animator;
	MathFunctions MF;

	void PlayAnimation(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, olc::vf2d PlayerPos, float fElapsedTime);
	void Draw(olc::TileTransformedView& tv);
	olc::vf2d Input(olc::PixelGameEngine* pge, float fElapsedTime);
	bool AttackInput(olc::PixelGameEngine* pge, float fElapsedTime);
	GlobalVars::GameStateEnum EscapeInput(olc::PixelGameEngine* pge);
	int HealthTest(olc::PixelGameEngine* pge);
	void Initialize(olc::PixelGameEngine* pge);
};