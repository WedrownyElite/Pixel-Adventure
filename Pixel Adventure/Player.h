#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#define GlobalVars
#include "GlobalVars.h"
#define EnemyFunctions
#include "EnemyFunctions.h"

class Player {
private:

	//Sprites
	std::unique_ptr<olc::Sprite> PlayerRight;
	std::unique_ptr<olc::Sprite> PlayerLeft;
	std::unique_ptr<olc::Sprite> PlayerDead;
	std::unique_ptr<olc::Sprite> Shadow;

	//Decals
	olc::Decal* PlayerRightDecal;
	olc::Decal* PlayerLeftDecal;
	olc::Decal* PlayerDeadDecal;
	olc::Decal* ShadowDecal;

	olc::vf2d PlayerPos{ 300.0f, 300.0f };
	olc::vf2d MousePos;
	float AttackCooldown = 0.0f;
	bool ArcherDir = true;
	int CharacterHealth = 6;
	float PlayerSpeed;

public:
	void Draw(olc::TileTransformedView& tv);
	olc::vf2d Input(olc::PixelGameEngine* pge, float fElapsedTime);
	bool AttackInput(olc::PixelGameEngine* pge, float fElapsedTime);
	GameStateEnum EscapeInput(olc::PixelGameEngine* pge);
	int HealthTest(olc::PixelGameEngine* pge);
	void Initialize(olc::PixelGameEngine* pge);
};
