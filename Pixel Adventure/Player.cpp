#include "Player.h"

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
	}
	if (pge->GetKey(olc::Key::RIGHT).bHeld || (pge->GetKey(olc::Key::D).bHeld)) {
		PlayerPos.x += PlayerSpeed;
		ArcherDir = true;
	}
	if (pge->GetKey(olc::Key::UP).bHeld || (pge->GetKey(olc::Key::W).bHeld)) {
		PlayerPos.y -= PlayerSpeed;
	}
	if (pge->GetKey(olc::Key::DOWN).bHeld || (pge->GetKey(olc::Key::S).bHeld)) {
		PlayerPos.y += PlayerSpeed;
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
	if (pge->GetMouse(0).bPressed && PlayerCanAttack == true) {
		PlayerCanAttack = false;
	}
	if (PlayerCanAttack == false) {
		if (AttackCooldown < 0.5f) {
			AttackCooldown += fElapsedTime;
		}
		if (AttackCooldown >= 0.5f) {
			PlayerCanAttack = true;
			AttackCooldown = 0.0f;
		}
	}
	return PlayerCanAttack;
}