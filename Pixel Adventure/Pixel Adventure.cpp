#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"
#include "olcUTIL_Camera2D.h"

#include "GUI.h"
#include "GlobalVars.h"
#include "MathFunctions.h"

std::vector<GameStateEnum> GameState;

//Classes
class EnemyFunctions {
private:
	float JumpHeight = 1.0f;
	float KnockbackSpeed = 12.0f;
	float JumpSpeed = 6.0f;

public:
	float ReturnDistTraveled(float DistTraveled, float fElapsedTime) {
		float DistThisFrame = KnockbackSpeed * fElapsedTime;
		DistTraveled += DistThisFrame;

		return DistTraveled;
	}
	int ReturnHit(float DistTraveled, int Hit) {
		if (DistTraveled >= 2.0f) {
			Hit = 0;
		}
		return Hit;
	}
	olc::vf2d Knockback(olc::vf2d PlayerPos, olc::vf2d EnemyPos, float fElapsedTime) {

		olc::vf2d dir = (PlayerPos - EnemyPos).norm();

		float DistThisFrame = KnockbackSpeed * fElapsedTime;

		EnemyPos -= dir * DistThisFrame;

		return EnemyPos;

	}
};
class Player {
public:

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

	EnemyFunctions Enemy;
	void Draw(olc::TileTransformedView& tv) {
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
	olc::vf2d Input(olc::PixelGameEngine* pge, float fElapsedTime) {
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
	bool AttackInput(olc::PixelGameEngine* pge, float fElapsedTime) {
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
	GameStateEnum EscapeInput(olc::PixelGameEngine* pge) {
		if (pge->GetKey(olc::Key::ESCAPE).bPressed) {
			GameState[1] = GameState[0];
			GameState[0] = PAUSED;
		}
		return GameState[1], GameState[0];
	}
	int HealthTest(olc::PixelGameEngine* pge) {
		if (pge->GetKey(olc::Key::O).bPressed) {
			CharacterHealth--;
		}
		return CharacterHealth;
	}
	void Initialize(olc::PixelGameEngine* pge) {
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
	}
};
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

	EnemyFunctions Enemy;
	MathFunctions MF;

	//Sprites
	std::unique_ptr<olc::Sprite> SkeleRight;
	std::unique_ptr<olc::Sprite> SkeleRightHurt;
	std::unique_ptr<olc::Sprite> Shadow;

	//Decals
	olc::Decal* SkeleRightDecal;
	olc::Decal* SkeleRightHurtDecal;
	olc::Decal* ShadowDecal;
public:
	void Hurt(olc::TileTransformedView& tv, olc::vf2d PlayerPos, int Container, float fElapsedTime) {
		if (SkeleHit[Container] == 1) {
			SkeleHit[Container] = Enemy.ReturnHit(DistTraveled[Container], SkeleHit[Container]);
			SkelePos[Container] = Enemy.Knockback(PlayerPos, SkelePos[Container], fElapsedTime);
			DistTraveled[Container] = Enemy.ReturnDistTraveled(DistTraveled[Container], fElapsedTime);
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
	void Spawn() {
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
	olc::vf2d ReturnClosestPos(olc::vf2d PlayerPos) {
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
	void Draw(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, olc::vf2d PlayerPos, float PlayerSpeed, bool PlayerCanAttack, float fElapsedTime) {
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
			olc::vf2d MousePos = { MF.GetWorldMousePos(tv, pge) };
			olc::vf2d PlayerDir = (-(PlayerPos - MousePos).norm());
			float angleTowards = MF.PointTo(PlayerPos, SkelePos[k]); //Calculate the angle towards a target.
			float angleDiff = MF.angleDifference(PlayerDir.polar().y, angleTowards); //Calculate the difference between the target and the angle.
			if (
				(sqrt(pow(PlayerPos.x - SkelePos[k].x, 2) + pow(PlayerPos.y - SkelePos[k].y, 2)) < maxDistance //Check to see if the target is in range (distance formula)
				&& abs(angleDiff) < maxAngle)  //See if the target's angle is within the sweeping arc range.
				&& PlayerCanAttack == true) {
				//&& pge->GetMouse(0).bPressed) {
				//Initiate hit
				JumpBool[k] = 1;
				SkeleRedTimer[k] = 1;
				SkeleHit[k] = 1;
				tv.DrawDecal({ SkelePos[k].x - 0.95f, SkelePos[k].y + 0.1f }, ShadowDecal, { 1.8f, 1.8f });
				tv.DrawDecal({ SkelePos[k].x - 1.20f, SkelePos[k].y - 1.20f }, SkeleRightDecal, { 2.3f, 2.3f });
				tv.DrawDecal({ SkelePos[k].x - 1.20f, SkelePos[k].y - 1.20f }, SkeleRightHurtDecal, { 2.3f, 2.3f });
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
	void SkeleTest(olc::PixelGameEngine* pge, float fElapsedTime) {
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
	void Initialize() {
		//Sprites
		SkeleRight = std::make_unique<olc::Sprite>("./Sprites/SkeletonRight.png");
		SkeleRightHurt = std::make_unique<olc::Sprite>("./Sprites/SkeletonRightHurt.png");
		Shadow = std::make_unique<olc::Sprite>("./Sprites/Shadow.png");
		//Decals
		SkeleRightDecal = new olc::Decal(SkeleRight.get());
		SkeleRightHurtDecal = new olc::Decal(SkeleRightHurt.get());
		ShadowDecal = new olc::Decal(Shadow.get());
	}
};
class Pixel_Adventure : public olc::PixelGameEngine {
public:

	//Sprites
	std::unique_ptr<olc::Sprite> Grass;
	//Decals
	olc::Decal* GrassDecal;
	//Main Menu
	int Selection = 0;

	//Camera variables
	// Transformed view object to make world offsetting simple
	olc::TileTransformedView tv;
	// Conveninet constants to define tile map world
	olc::vi2d m_vWorldSize = { 1000, 1000 };
	olc::vi2d m_vTileSize = { 32, 32 };
	// The camera!
	olc::utils::Camera2D camera;
	// The world map, stored as a 1D array
	std::vector<uint8_t> vWorldMap;
	//Player variables
	olc::vf2d PlayerPos = { 300, 300 };
	bool ArcherDir = true;
	int CharacterHealth = 6;
	//Arch variables
	float radius = 8.0f; // Radius of the arch (half of the desired length)

	olc::vf2d GoopPos = { 280.0f, 280.0f }; // Position of the Goop
	olc::vf2d GoopSize = { 16.0f, 16.0f }; // Size of the Goop
	bool ArchDrawn = false;

	Player Player;
	Skeleton Skeleton;
	GUI GUI;

	Pixel_Adventure() {
		sAppName = "Pixel Adventure";
	}
	//User inputs
	void PauseScreenInputs(bool resume_hovered, bool options_hovered, bool quit_hovered) {
		if (GetMouse(0).bPressed && resume_hovered == true) {
			GameState[0] = GameState[1];
		}
		if (GetMouse(0).bPressed && options_hovered == true) {

		}
		if (GetMouse(0).bPressed && quit_hovered == true) {
			GameState[0] = EXIT;
		}
	}
	//UI
	void MainMenu() {
		if (Selection == 0) {
			DrawStringDecal({ (float)ScreenWidth() / 2, (float)ScreenHeight() / 2 }, ">> Play", olc::WHITE, { 5.0, 5.0f });
			DrawStringDecal({ (float)ScreenWidth() / 2, (float)ScreenHeight() / 2 + 80 }, "Debug", olc::WHITE, { 5.0f, 5.0f });
		}
		if (Selection == 1) {
			DrawStringDecal({ (float)ScreenWidth() / 2, (float)ScreenHeight() / 2 }, "Play", olc::WHITE, { 5.0, 5.0f });
			DrawStringDecal({ (float)ScreenWidth() / 2, (float)ScreenHeight() / 2 + 80 }, ">> Debug", olc::WHITE, { 5.0f, 5.0f });
		}

		//UserInput
		if ((GetKey(olc::Key::UP).bPressed || GetKey(olc::Key::W).bPressed)) {
			Selection--;
		}
		if ((GetKey(olc::Key::DOWN).bPressed || GetKey(olc::Key::S).bPressed)) {
			Selection++;
		}
		if (Selection > 1) {
			Selection = 0;
		}
		if (Selection < 0) {
			Selection = 1;
		}
		if (Selection == 0 && GetKey(olc::Key::ENTER).bPressed) {
			GameState[0] = GAME;
		}
		if (Selection == 1 && GetKey(olc::Key::ENTER).bPressed) {
			GameState[0] = DEBUG;
		}
		if (GetKey(olc::Key::ESCAPE).bPressed) {
			GameState[0] = EXIT;
		}
	}
	void DrawBGCamera() {
		// Render "tile map", by getting visible tiles
		olc::vi2d vTileTL = tv.GetTopLeftTile().max({ 0,0 });
		olc::vi2d vTileBR = tv.GetBottomRightTile().min(m_vWorldSize);
		olc::vi2d vTile;

		for (vTile.y = vTileTL.y; vTile.y < vTileBR.y; vTile.y++)
			for (vTile.x = vTileTL.x; vTile.x < vTileBR.x; vTile.x++)
			{
				int idx = vTile.y * m_vWorldSize.x + vTile.x;

				if (vWorldMap[idx] == 0)
					tv.DrawDecal(vTile, GrassDecal, { 1.0f, 1.0f });

				if (vWorldMap[idx] == 1)
					tv.DrawDecal(vTile, GrassDecal, { 1.0f, 1.0f });
			}
	}
	//GameStates
	void GameGameState(float fElapsedTime) {
		//Speed
		fElapsedTime = std::min(fElapsedTime, 0.16667f);

		//Camera variables
		bool bOnScreen = camera.Update(fElapsedTime);
		tv.SetWorldOffset(camera.GetViewPosition());

		PlayerPos = Player.Input(this, fElapsedTime);
		CharacterHealth = Player.HealthTest(this);
		Player.EscapeInput(this);

		DrawBGCamera();

		//Draw Archer
		Player.Draw(tv);

		//Draw Hearts
		GUI.Hearts(this, CharacterHealth);
	}
	void DebugGameState(float fElapsedTime) {
		//Speed
		fElapsedTime = std::min(fElapsedTime, 0.16667f);
		float PlayerSpeed = 8 * fElapsedTime;

		//Camera variables
		bool bOnScreen = camera.Update(fElapsedTime);
		tv.SetWorldOffset(camera.GetViewPosition());

		PlayerPos = Player.Input(this, fElapsedTime);
		//CharacterHealth = Player.HealthTest(this);
		Player.EscapeInput(this);

		DrawBGCamera();

		Skeleton.SkeleTest(this, fElapsedTime);
		//Get ClosestSkelePos
		olc::vf2d ClosestSkelePos = Skeleton.ReturnClosestPos(PlayerPos);

		bool PlayerCanAttack = Player.AttackInput(this, fElapsedTime);

		//If Closest Skeleton is above player
		if (ClosestSkelePos.y <= PlayerPos.y) {
			//Enemies
			Skeleton.Draw(tv, this, PlayerPos, PlayerSpeed, PlayerCanAttack, fElapsedTime);

			//Draw Archer
			Player.Draw(tv);
		}

		//If Closest Skeleton is below player
		else {
			//Draw Archer
			Player.Draw(tv);

			//Enemies
			Skeleton.Draw(tv, this, PlayerPos, PlayerSpeed, PlayerCanAttack, fElapsedTime);
		}

		//HealthTest
		Player.HealthTest(this);

		//Draw variables
		GUI.DrawDebugVariables(tv, this, PlayerPos, CharacterHealth, PlayerCanAttack);

		//Draw Hearts
		GUI.Hearts(this, CharacterHealth);
	}
	bool OnUserUpdate(float fElapsedTime) override {
		if (GameState[0] == MENU) {
			MainMenu();
			return true;
		}
		if (GameState[0] == GAME) {
			GameGameState(fElapsedTime);
			return true;
		}
		if (GameState[0] == DEBUG) {
			DebugGameState(fElapsedTime);
			return true;
		}
		if (GameState[0] == PAUSED) {
			GUI.doPauseScreen(tv, this, CharacterHealth, PlayerPos, PlayerCanAttack);
		}
		if (GameState[0] == EXIT) {
			return false;
		}
	}
private:
	bool OnUserCreate() override {

		GameState.push_back(MENU);
		GameState.push_back(MENU);

		//Initialize Sprites for Skeletons
		Skeleton.Initialize();
		//Initialize player sprites/decals
		Player.Initialize(this);
		//Initialize GUI sprites/decals
		GUI.Initialize(this);

		// Construct transform view
		tv = olc::TileTransformedView(GetScreenSize(), m_vTileSize);

		// Construct Camera
		camera = olc::utils::Camera2D(GetScreenSize() / m_vTileSize, PlayerPos);

		// Configure Camera
		camera.SetTarget(PlayerPos);
		camera.SetMode(olc::utils::Camera2D::Mode::LazyFollow);
		camera.SetWorldBoundary({ 0.0f, 0.0f }, m_vWorldSize);
		camera.EnableWorldBoundary(false);

		// Create "tile map" world with just two tiles
		vWorldMap.resize(m_vWorldSize.x * m_vWorldSize.y);
		for (int i = 0; i < vWorldMap.size(); i++)
			vWorldMap[i] = ((rand() % 20) == 1) ? 1 : 0;

		GameState.push_back(MENU);
		GameState.push_back(MENU);



		//Sprites
		Grass = std::make_unique<olc::Sprite>("./Sprites/Grass.png");
		//Decals
		GrassDecal = new olc::Decal(Grass.get());
		return true;
	}
};

int main() {
	Pixel_Adventure demo;
	if (demo.Construct(1024, 576, 1, 1, true, true))
		demo.Start();
}

// To Do
//\ 1. Implement a "camera" https://community.onelonecoder.com/members/javidx9/Camera/
// 2. Implement knockback when Skeleton is hit
// 3. Implement a passive mob case (idle, wandering)
// 4. Implement an aggressive enemies attack case (Straight at player, around the player, running away, etc)