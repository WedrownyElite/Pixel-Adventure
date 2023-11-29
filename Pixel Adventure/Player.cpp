#include "Player.h"
#include "GlobalVars.h"

void Player::PlayAnimation(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, float fElapsedTime) {
	if (DrawAnim == false && pge->GetKey(olc::Key::A).bHeld) {
		WalkLeft.UpdateAnimations(fElapsedTime);
		
		pge->DrawDecal({ PlayerPos.x - 1.9f, PlayerPos.y - 2.0f }, WalkLeft.GetAnim("Walk_Left"), {4.0f, 4.0f});
	}
}
void Player::Draw(olc::TileTransformedView& tv) {
	//Draw Archer
	if (ArcherDir == true) {
		tv.DrawDecal({ PlayerPos.x - 1.0f, PlayerPos.y }, ShadowDecal, { 2.0f, 2.0f });
		tv.DrawDecal({ PlayerPos.x - 1.9f, PlayerPos.y - 2.0f }, PlayerRightDecal, { 4.0f, 4.0f });
	}
	if (ArcherDir == false) {
		tv.DrawDecal({ PlayerPos.x - 1.0f, PlayerPos.y }, ShadowDecal, { 2.0f, 2.0f });
		tv.DrawDecal({ PlayerPos.x - 1.9f, PlayerPos.y - 2.0f }, PlayerLeftDecal, { 4.0f, 4.0f });
	}
}
olc::vf2d Player::Input(olc::PixelGameEngine* pge, float fElapsedTime){
	PlayerSpeed = 8.0f * fElapsedTime;
	// Handle input and move the player
	if (pge->GetKey(olc::Key::LEFT).bHeld || (pge->GetKey(olc::Key::A).bHeld)) {
		PlayerPos.x -= PlayerSpeed;
		ArcherDir = false;
		DrawAnim = false;
	}
	if (pge->GetKey(olc::Key::RIGHT).bHeld || (pge->GetKey(olc::Key::D).bHeld)) {
		PlayerPos.x += PlayerSpeed;
		ArcherDir = true;
		DrawAnim = true;
	}
	if (pge->GetKey(olc::Key::UP).bHeld || (pge->GetKey(olc::Key::W).bHeld)) {
		PlayerPos.y -= PlayerSpeed;
		DrawAnim = ArcherDir;
	}
	if (pge->GetKey(olc::Key::DOWN).bHeld || (pge->GetKey(olc::Key::S).bHeld)) {
		PlayerPos.y += PlayerSpeed;
		DrawAnim = ArcherDir;
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
void Player::Initialize(olc::PixelGameEngine* pge, olcPGEX_ResourceManager &rm) {
	//Sprites
	PlayerRight = std::make_unique<olc::Sprite>("./Sprites/CharacterRightFacing.png");
	PlayerLeft = std::make_unique<olc::Sprite>("./Sprites/CharacterLeftFacing.png");
	PlayerDead = std::make_unique<olc::Sprite>("./Sprites/CharacterDeathPose.png");
	Shadow = std::make_unique<olc::Sprite>("./Sprites/Shadow.png");
	//Decals
	PlayerRightDecal = new olc::Decal(PlayerRight.get());
	PlayerLeftDecal = new olc::Decal(PlayerLeft.get());
	PlayerDeadDecal = new olc::Decal(PlayerDead.get());
	ShadowDecal = new olc::Decal(Shadow.get());

	//Animations
	WalkLeft.AddAnimation("Walk_Left", 0.6f, 6, rm.RM_Sprite("./Sprites/CharacterLeftFacing-Sheet.png"), { 0, 0 }, { 32, 32 }, { 0, 0 }, { 0, 0 }, true, false);
}