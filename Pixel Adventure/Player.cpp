#include "Player.h"
#include "GlobalVars.h"

#include "olcPGEX_Animator2D.h"

olcPGEX_Animator2D animator;

void Player::PlayAnimation(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, olc::vf2d PlayerPos, float fElapsedTime) {
	if (DrawAnim == false) {

		animator.UpdateAnimations(fElapsedTime);
		//Set PlayerPos to tv offset
		PlayerPos = MF.GetPlayerPos(tv, pge, PlayerPos);
		//Draw animation
		animator.DrawAnimationFrame(PlayerPos);
	}
}
void Player::Draw(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, float fElapsedTime) {
	//Draw Animation if player moving
	//if (PlayerMoving == true) {
	//	PlayAnimation(tv, pge, PlayerPos, fElapsedTime);
	//}
	//Draw Player if not moving
	if (ArcherDir == true && GlobalVars::PlayerMoving == false) {
		tv.DrawDecal({ PlayerPos.x - 1.0f, PlayerPos.y }, ShadowDecal, { 2.0f, 2.0f });
		tv.DrawDecal({ PlayerPos.x - 1.9f, PlayerPos.y - 2.0f }, PlayerRightDecal, { 4.0f, 4.0f });
	}
	if (ArcherDir == false && GlobalVars::PlayerMoving == false) {
		tv.DrawDecal({ PlayerPos.x - 1.0f, PlayerPos.y }, ShadowDecal, { 2.0f, 2.0f });
		tv.DrawDecal({ PlayerPos.x - 1.9f, PlayerPos.y - 2.0f }, PlayerLeftDecal, { 4.0f, 4.0f });
	}
}
olc::vf2d Player::Input(olc::PixelGameEngine* pge, float fElapsedTime) {
	PlayerSpeed = 8.0f * fElapsedTime;
	// Handle input and move the player
	if (pge->GetKey(olc::Key::LEFT).bHeld || (pge->GetKey(olc::Key::A).bHeld)) {
		PlayerPos.x -= PlayerSpeed;
		ArcherDir = false;
		DrawAnim = false;
		GlobalVars::PlayerMoving = true;
	}
	if (pge->GetKey(olc::Key::RIGHT).bHeld || (pge->GetKey(olc::Key::D).bHeld)) {
		PlayerPos.x += PlayerSpeed;
		ArcherDir = true;
		DrawAnim = true;
		GlobalVars::PlayerMoving = true;
	}
	if (pge->GetKey(olc::Key::UP).bHeld || (pge->GetKey(olc::Key::W).bHeld)) {
		PlayerPos.y -= PlayerSpeed;
		DrawAnim = ArcherDir;
	}
	if (pge->GetKey(olc::Key::DOWN).bHeld || (pge->GetKey(olc::Key::S).bHeld)) {
		PlayerPos.y += PlayerSpeed;
		DrawAnim = ArcherDir;
		GlobalVars::PlayerMoving = true;
	}
	//If player isn't moving
	if ((!pge->GetKey(olc::Key::DOWN).bHeld || (!pge->GetKey(olc::Key::S).bHeld))
		|| (!pge->GetKey(olc::Key::UP).bHeld || (!pge->GetKey(olc::Key::W).bHeld))
		|| (!pge->GetKey(olc::Key::RIGHT).bHeld || (!pge->GetKey(olc::Key::D).bHeld))
		|| (!pge->GetKey(olc::Key::LEFT).bHeld || (!pge->GetKey(olc::Key::A).bHeld))) {
		GlobalVars::PlayerMoving = false;
	}

	if (PlayerPos.x > 965) {
		PlayerPos.x = 964;
	}
	if (PlayerPos.x < -8) {
		PlayerPos.x = -7;
	}
	if (PlayerPos.y > 512) {
		PlayerPos.y = 511;
	}
	if (PlayerPos.y < -5) {
		PlayerPos.y = -4;
	}
	return PlayerPos;
}
bool Player::AttackInput(olc::PixelGameEngine* pge, float fElapsedTime) {
	if (GlobalVars::PlayerCanAttack == false) {
		if (AttackCooldown < 0.5f) {
			AttackCooldown += fElapsedTime;
		}
		if (AttackCooldown >= 0.5f) {
			GlobalVars::PlayerCanAttack = true;
			AttackCooldown = 0.0f;
		}
	}
	return GlobalVars::PlayerCanAttack;
}
GlobalVars::GameStateEnum Player::EscapeInput(olc::PixelGameEngine* pge) {
	if (pge->GetKey(olc::Key::ESCAPE).bPressed) {
		GlobalVars::GameState[1] = GlobalVars::GameState[0];
		GlobalVars::GameState[0] = GlobalVars::PAUSED;
	}
	return GlobalVars::GameState[1], GlobalVars::GameState[0];
}
int Player::HealthTest(olc::PixelGameEngine* pge) {
	if (pge->GetKey(olc::Key::O).bPressed) {
		CharacterHealth--;
	}
	return CharacterHealth;
}
void Player::Initialize(olc::PixelGameEngine* pge) {
	//Sprites
	PlayerRight = std::make_unique<olc::Sprite>("./Sprites/CharacterRightFacing.png");
	PlayerLeft = std::make_unique<olc::Sprite>("./Sprites/CharacterLeftFacing.png");
	PlayerDead = std::make_unique<olc::Sprite>("./Sprites/CharacterDeathPose.png");
	Shadow = std::make_unique<olc::Sprite>("./Sprites/Shadow.png");
	WalkLeftSS = std::make_unique<olc::Sprite>("./Sprites/CharacterLeftFacing-Sheet.png");
	//Decals
	PlayerRightDecal = new olc::Decal(PlayerRight.get());
	PlayerLeftDecal = new olc::Decal(PlayerLeft.get());
	PlayerDeadDecal = new olc::Decal(PlayerDead.get());
	ShadowDecal = new olc::Decal(Shadow.get());
	WalkLeftDecal = new olc::Decal(WalkLeftSS.get());

	//Animations
	animator.AddAnimation("Walk_Left", 0.6f, 6, WalkLeftDecal, { 0, 0 }, { 32, 32 }, { 0, 0 }, { 0, 0 }, true, false);
}