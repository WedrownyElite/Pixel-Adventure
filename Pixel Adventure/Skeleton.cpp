#include "Skeleton.h"
#include "EnemyFunctions.h"
#include "MathFunctions.h"

void Skeleton::Hurt(olc::TileTransformedView& tv, olc::vf2d PlayerPos, int Container, float fElapsedTime) {
	if (SkeleHit[Container] == 1) {
		SkeleHit[Container] = EnemyFunctions::ReturnHit(DistTraveled[Container], SkeleHit[Container]);
		SkelePos[Container] = EnemyFunctions::Knockback(PlayerPos, SkelePos[Container], fElapsedTime);
		DistTraveled[Container] = EnemyFunctions::ReturnDistTraveled(DistTraveled[Container], fElapsedTime);
		if (SkeleRedTimer[Container] == 0) {
			tv.DrawDecal({ SkelePos[Container].x - 0.95f, SkelePos[Container].y + 0.1f }, ShadowDecal, { 1.8f, 1.8f });
			tv.DrawDecal({ SkelePos[Container].x - 1.0f, SkelePos[Container].y - 1.0f }, SkeleRightDecal, { 2.0f, 2.0f });
		}
	}
	//Reset Distance traveled
	if (SkeleHit[Container] == 0) {
		DistTraveled[Container] = 0.0f;
		JumpDist[Container] = 0.0f;
	}
	//Draw hurt skeleton for 10 frames
	if (SkeleRedTimer[Container] > 0) {
		tv.DrawDecal({ SkelePos[Container].x - 0.95f, SkelePos[Container].y + 0.1f }, ShadowDecal, { 1.8f, 1.8f });
		tv.DrawDecal({ SkelePos[Container].x - 1.20f, SkelePos[Container].y - 1.20f }, SkeleRightDecal, { 2.3f, 2.3f });
		tv.DrawDecal({ SkelePos[Container].x - 1.20f, SkelePos[Container].y - 1.20f }, SkeleRightHurtDecal, { 2.3f, 2.3f });
		SkeleRedTimer[Container]++;
		if (SkeleRedTimer[Container] == 21) {
			tv.DrawDecal({ SkelePos[Container].x - 0.95f, SkelePos[Container].y + 0.1f }, ShadowDecal, { 1.8f, 1.8f });
			tv.DrawDecal({ SkelePos[Container].x - 1.20f, SkelePos[Container].y - 1.20f }, SkeleRightDecal, { 2.3f, 2.3f });
			tv.DrawDecal({ SkelePos[Container].x - 1.20f, SkelePos[Container].y - 1.20f }, SkeleRightHurtDecal, { 2.3f, 2.3f });
			SkeleRedTimer[Container] = 0;
		}
	}
}
void Skeleton::Spawn() {
	if (SkelePos.size() < 1) {
		for (int k = 0; k < 1; k++) {
			SkelePos.push_back({ 305, 305 });
			SkeleHit.push_back(0);
			SkeleRedTimer.push_back(0);
			DistTraveled.push_back(0.0f);
			JumpBool.push_back(0);
			JumpDist.push_back(0.0f);
		}
	}
}
olc::vf2d Skeleton::ReturnClosestPos(olc::vf2d PlayerPos) {
	float ClosestSkeleDis = 1000.0f;
	olc::vf2d ClosestSkelePos;
	for (int k = 0; k < SkelePos.size(); k++) {

		float dx = SkelePos[k].x - PlayerPos.x;
		float dy = SkelePos[k].y - PlayerPos.y;

		float dis = sqrt(dx * dx + dy * dy);
		if (dis < ClosestSkeleDis) {
			ClosestSkeleDis = dis;
			ClosestSkelePos = SkelePos[k];
		}
	}
	return ClosestSkelePos;
}
void Skeleton::Draw(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, olc::vf2d PlayerPos, float PlayerSpeed, bool PlayerCanAttack, float fElapsedTime){
	Spawn();
	for (int k = 0; k < SkelePos.size(); k++) {
		olc::vf2d Skele(SkelePos[k].x - 0.5f, SkelePos[k].y - 0.85f);
		olc::vf2d UPlayerPos(PlayerPos.x - 0.4f, PlayerPos.y - 1.0f);

		//Collision
		if (Skele.x < UPlayerPos.x + PlayerSize.x
			&& Skele.x + SkeleSize.x > UPlayerPos.x
			&& Skele.y < UPlayerPos.y + PlayerSize.y
			&& Skele.y + SkeleSize.y > UPlayerPos.y) {
			//Direction of player
			olc::vf2d dir = (PlayerPos - SkelePos[k]).norm();
			SkelePos[k] += -dir * PlayerSpeed;
		}

		//Hit check
		olc::vf2d MousePos = { MathFunctions::GetWorldMousePos(tv, pge) };
		olc::vf2d PlayerDir = (-(PlayerPos - MousePos).norm());
		float angleTowards = MathFunctions::PointTo(PlayerPos, SkelePos[k]); //Calculate the angle towards a target.
		float angleDiff = MathFunctions::angleDifference(PlayerDir.polar().y, angleTowards); //Calculate the difference between the target and the angle.
		if (pge->GetMouse(0).bPressed && PlayerCanAttack == true) {
			PlayerCanAttack = false;
			if (
				(sqrt(pow(PlayerPos.x - SkelePos[k].x, 2) + pow(PlayerPos.y - SkelePos[k].y, 2)) < GlobalVars::maxDistance //Check to see if the target is in range (distance formula)
					&& abs(angleDiff) < GlobalVars::maxAngle)  //See if the target's angle is within the sweeping arc range.
				) {
				//&& pge->GetMouse(0).bPressed) {
				//Initiate hit
				JumpBool[k] = 1;
				SkeleRedTimer[k] = 1;
				SkeleHit[k] = 1;
				tv.DrawDecal({ SkelePos[k].x - 0.95f, SkelePos[k].y + 0.1f }, ShadowDecal, { 1.8f, 1.8f });
				tv.DrawDecal({ SkelePos[k].x - 1.20f, SkelePos[k].y - 1.20f }, SkeleRightDecal, { 2.3f, 2.3f });
				tv.DrawDecal({ SkelePos[k].x - 1.20f, SkelePos[k].y - 1.20f }, SkeleRightHurtDecal, { 2.3f, 2.3f });
			}
		}
		//Hit
		Hurt(tv, PlayerPos, k, fElapsedTime);
		//If not hit, draw normal skeleton
		if (SkeleRedTimer[k] == 0 && SkeleHit[k] == 0) {
			tv.DrawDecal({ SkelePos[k].x - 0.95f, SkelePos[k].y + 0.1f }, ShadowDecal, { 1.8f, 1.8f });
			tv.DrawDecal({ SkelePos[k].x - 1.0f, SkelePos[k].y - 1.0f }, SkeleRightDecal, { 2.0f, 2.0f });
		}
	}
}
void Skeleton::SkeleTest(olc::PixelGameEngine* pge, float fElapsedTime) {
	float SkeleSpeed = 8.0f * fElapsedTime;

	for (int k = 0; k < SkelePos.size(); k++) {
		if (pge->GetKey(olc::Key::I).bPressed) {
			SkelePos[k].y += 0.5f;
			SkelePos[k].x += 1.0f;
		}
		if (pge->GetKey(olc::Key::K).bPressed) {
			SkelePos[k].y += 0.01f;
		}
		if (pge->GetKey(olc::Key::L).bPressed) {
			SkelePos[k].x += 0.01f;
		}
		if (pge->GetKey(olc::Key::J).bPressed) {
			SkelePos[k].x -= 0.01f;
		}
	}
}
void Skeleton::Initialize() {
	//Sprites
	SkeleRight = std::make_unique<olc::Sprite>("./Sprites/SkeletonRight.png");
	SkeleRightHurt = std::make_unique<olc::Sprite>("./Sprites/SkeletonRightHurt.png");
	Shadow = std::make_unique<olc::Sprite>("./Sprites/Shadow.png");
	//Decals
	SkeleRightDecal = new olc::Decal(SkeleRight.get());
	SkeleRightHurtDecal = new olc::Decal(SkeleRightHurt.get());
	ShadowDecal = new olc::Decal(Shadow.get());
}