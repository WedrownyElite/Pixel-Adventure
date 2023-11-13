#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"
#include "olcUTIL_Camera2D.h"
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

float maxDistance = 3; //How far away the object can be to still be in range.
float maxAngle = M_PI / 4; //The total sweeping angle of the arch in either direction (PI/4 is 45 degrees in either direction, 90 degrees total).

//GameState
enum GameStateEnum { DEBUG, GAME, PAUSED, EXIT, MENU };
std::vector<GameStateEnum> GameState;

//Math functions
float angleDifference(float angle_1, float angle_2)
{
	angle_1 = fmod(angle_1, 2 * 3.14159f);
	angle_2 = fmod(angle_2, 2 * 3.14159f);
	float angle_diff = angle_1 - angle_2;

	if (angle_diff > 3.14159f)
		angle_diff -= 2 * 3.14159f;
	else if (angle_diff < -3.14159f)
		angle_diff += 2 * 3.14159f;

	return -angle_diff;
}
olc::vf2d GetWorldMousePos(olc::TileTransformedView& tv, olc::PixelGameEngine* pge) {
	return pge->GetMousePos() / 32 + tv.GetWorldOffset();
}
float PointTo(olc::vf2d pos1, olc::vf2d pos2) {
	return atan2(pos2.y - pos1.y, pos2.x - pos1.x);
}

//Classes
class Player {
public:

	//Sprites
	std::unique_ptr<olc::Sprite> ArcherRight;
	std::unique_ptr<olc::Sprite> ArcherLeft;
	std::unique_ptr<olc::Sprite> Shadow;

	//Decals
	olc::Decal* ArcherRightDecal;
	olc::Decal* ArcherLeftDecal;
	olc::Decal* ShadowDecal;

	olc::vf2d PlayerPos{ 300.0f, 300.0f };
	olc::vf2d MousePos;
	bool ArcherDir = true;
	int CharacterHealth = 6;
	float PlayerSpeed;

	void Draw(olc::TileTransformedView& tv) {
		//Draw Archer
		if (ArcherDir == true) {
			tv.DrawDecal({ PlayerPos.x - 0.13f, PlayerPos.y + 0.9f }, ShadowDecal, { 2.0f, 2.0f });
			tv.DrawDecal({ PlayerPos }, ArcherRightDecal, { 2.0f, 2.0f });
		}
		if (ArcherDir == false) {
			tv.DrawDecal({ PlayerPos.x + 0.13f, PlayerPos.y + 0.9f }, ShadowDecal, { 2.0f, 2.0f });
			tv.DrawDecal({ PlayerPos }, ArcherLeftDecal, { 2.0f, 2.0f });
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
	GameStateEnum EscapeInput(olc::PixelGameEngine* pge) {
		if (pge->GetKey(olc::Key::ESCAPE).bPressed) {
			GameState[1] = GameState[0];
			GameState[0] = PAUSED;
		}
		return GameState[0], GameState[1];
	}
	int HealthTest(olc::PixelGameEngine* pge) {
		if (pge->GetMouse(0).bPressed) {
			CharacterHealth--;
		}
		return CharacterHealth;
	}
	void Initialize(olc::PixelGameEngine* pge) {
		//Sprites
		ArcherRight = std::make_unique<olc::Sprite>("./Sprites/ArcherRight.png");
		ArcherLeft = std::make_unique<olc::Sprite>("./Sprites/ArcherLeft.png");
		Shadow = std::make_unique<olc::Sprite>("./Sprites/Shadow.png");
		//Decals
		ArcherRightDecal = new olc::Decal(ArcherRight.get());
		ArcherLeftDecal = new olc::Decal(ArcherLeft.get());
		ShadowDecal = new olc::Decal(Shadow.get());
	}
};
class GUI {
public:
	//Sprites
	std::unique_ptr<olc::Sprite> PauseScreen;
	std::unique_ptr<olc::Sprite> FullHeart;
	std::unique_ptr<olc::Sprite> HalfHeart;
	std::unique_ptr<olc::Sprite> EmptyHeart;

	//Decals
	olc::Decal* PauseScreenDecal;
	olc::Decal* FullHeartDecal;
	olc::Decal* HalfHeartDecal;
	olc::Decal* EmptyHeartDecal;

	void Initialize(olc::PixelGameEngine* pge) {
		PauseScreen = std::make_unique<olc::Sprite>("./Sprites/PauseScreen.png");
		FullHeart = std::make_unique<olc::Sprite>("./Sprites/FullHeart.png");
		HalfHeart = std::make_unique<olc::Sprite>("./Sprites/HalfHeart.png");
		EmptyHeart = std::make_unique<olc::Sprite>("./Sprites/EmptyHeart.png");

		PauseScreenDecal = new olc::Decal(PauseScreen.get());
		FullHeartDecal = new olc::Decal(FullHeart.get());
		HalfHeartDecal = new olc::Decal(HalfHeart.get());
		EmptyHeartDecal = new olc::Decal(EmptyHeart.get());
	}
	void Hearts(olc::PixelGameEngine* pge, int CharacterHealth) {
		if (CharacterHealth == 6) {
			pge->DrawDecal({ 10.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
			pge->DrawDecal({ 47.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
			pge->DrawDecal({ 84.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
		}
		if (CharacterHealth == 5) {
			pge->DrawDecal({ 10.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
			pge->DrawDecal({ 47.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
			pge->DrawDecal({ 84.0f, 534.0f }, HalfHeartDecal, { 1.0f, 1.0f });
		}
		if (CharacterHealth == 4) {
			pge->DrawDecal({ 10.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
			pge->DrawDecal({ 47.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
			pge->DrawDecal({ 84.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
		}
		if (CharacterHealth == 3) {
			pge->DrawDecal({ 10.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
			pge->DrawDecal({ 47.0f, 534.0f }, HalfHeartDecal, { 1.0f, 1.0f });
			pge->DrawDecal({ 84.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
		}
		if (CharacterHealth == 2) {
			pge->DrawDecal({ 10.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
			pge->DrawDecal({ 47.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
			pge->DrawDecal({ 84.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
		}
		if (CharacterHealth == 1) {
			pge->DrawDecal({ 10.0f, 534.0f }, HalfHeartDecal, { 1.0f, 1.0f });
			pge->DrawDecal({ 47.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
			pge->DrawDecal({ 84.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
		}
		if (CharacterHealth <= 0) {
			pge->DrawDecal({ 10.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
			pge->DrawDecal({ 47.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
			pge->DrawDecal({ 84.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
		}
	}
	void DrawDebugVariables(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, olc::vf2d PlayerPos, int CharacterHealth) {
		//PlayerPos
		std::string PlayerX = std::to_string(PlayerPos.x);
		std::string PlayerY = std::to_string(PlayerPos.y);

		pge->DrawStringDecal({ 10.0f, 10.0f }, "PlayerX", olc::WHITE, { 2.0f, 2.0f });
		pge->DrawStringDecal({ 140.0f, 10.0f }, PlayerX, olc::WHITE, { 2.0f, 2.0f });
		pge->DrawStringDecal({ 10.0f, 30.0f }, "PlayerY", olc::WHITE, { 2.0f, 2.0f });
		pge->DrawStringDecal({ 140.0f, 30.0f }, PlayerY, olc::WHITE, { 2.0f, 2.0f });

		//MousePos
		olc::vf2d MousePos = { GetWorldMousePos(tv, pge) };
		std::string MouseX = std::to_string(MousePos.x);
		std::string MouseY = std::to_string(MousePos.y);

		pge->DrawStringDecal({ 10.0f, 60.0f }, "MouseX", olc::WHITE, { 2.0f, 2.0f });
		pge->DrawStringDecal({ 140.0f, 60.0f }, MouseX, olc::WHITE, { 2.0f, 2.0f });
		pge->DrawStringDecal({ 10.0f, 80.0f }, "MouseY", olc::WHITE, { 2.0f, 2.0f });
		pge->DrawStringDecal({ 140.0f, 80.0f }, MouseY, olc::WHITE, { 2.0f, 2.0f });

		std::string Health = std::to_string(CharacterHealth);

		pge->DrawStringDecal({ 10.0f, 120.0f }, "Health", olc::WHITE, { 2.0f, 2.0f });
		pge->DrawStringDecal({ 130.0f, 120.0f }, Health, olc::WHITE, { 2.0f, 2.0f });
	}
	GameStateEnum PauseScreenInput(olc::PixelGameEngine* pge, bool resume_hovered, bool options_hovered, bool quit_hovered) {
		if ((pge->GetMouse(0).bPressed && resume_hovered == true) || pge->GetKey(olc::Key::ESCAPE).bPressed) {
			GameState[0] = GameState[1];
		}
		if (pge->GetMouse(0).bPressed && options_hovered == true) {

		}
		if (pge->GetMouse(0).bPressed && quit_hovered == true) {
			GameState[0] = MENU;
		}
		return GameState[0], GameState[1];
	}
	void doPauseScreen(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, int CharacterHealth, olc::vf2d PlayerPos) {
		const olc::vf2d scale = { 1.0f, 1.0f };
		//Resume variables
		const bool resume_hovered = (pge->GetMouseX() >= 400 && pge->GetMouseY() >= 158 && pge->GetMouseX() <= 634 && pge->GetMouseY() <= 194);
		const float resume_zoom = resume_hovered ? 5.5f : 5.0f;
		const float resume_XCoord = resume_hovered ? 411.0f : 420.0f;
		const float resume_YCoord = resume_hovered ? 168.0f : 170.0f;
		//Options variables
		const  bool options_hovered = (pge->GetMouseX() >= 400 && pge->GetMouseY() >= 250 && pge->GetMouseX() <= 675 && pge->GetMouseY() <= 285);
		const float options_zoom = options_hovered ? 5.5f : 5.0f;
		const float options_XCoord = options_hovered ? 388.0f : 400.0f;
		const float options_YCoord = options_hovered ? 258.0f : 260.0f;
		//Quit variables
		const bool quit_hovered = (pge->GetMouseX() >= 460 && pge->GetMouseY() >= 350 && pge->GetMouseX() <= 620 && pge->GetMouseY() <= 385);
		const float quit_zoom = quit_hovered ? 5.5 : 5.0f;
		const float quit_XCoord = quit_hovered ? 453.0f : 460.0f;
		const float quit_YCoord = quit_hovered ? 348.0f : 350.0f;

		//Draw background (W.I.P)

		//Draw hearts
		Hearts(pge, CharacterHealth);

		//Draw pause opacity
			pge->DrawDecal({ 0.0f, 0.0f }, PauseScreenDecal, { 1.0f, 1.0f });

		pge->DrawStringDecal({ resume_XCoord, resume_YCoord }, "RESUME", olc::WHITE, scale * resume_zoom);
		pge->DrawStringDecal({ options_XCoord, options_YCoord }, "OPTIONS", olc::WHITE, scale * options_zoom);
		pge->DrawStringDecal({ quit_XCoord, quit_YCoord }, "QUIT", olc::WHITE, scale * quit_zoom);

		//Draw variables
		if (GameState[1] == DEBUG) {
			DrawDebugVariables(tv, pge, PlayerPos, CharacterHealth);
		}

		PauseScreenInput(pge, resume_hovered, options_hovered, quit_hovered);
	}
};
class Skeleton {
public:
	//Skele variables
	std::vector<olc::vf2d> SkelePos;
	olc::vf2d SkeleSize{ 1.1f, 1.7f };
	//Player
	olc::vf2d PlayerSize{ 1.1f, 1.7f };

	olc::vf2d Zero{ 0.0f, 0.0f };
	
	//Sprites
	std::unique_ptr<olc::Sprite> SkeleRight;
	std::unique_ptr<olc::Sprite> SkeleRightHurt;
	std::unique_ptr<olc::Sprite> Shadow;

	//Decals
	olc::Decal* SkeleRightDecal;
	olc::Decal* SkeleRightHurtDecal;
	olc::Decal* ShadowDecal;
	void Spawn() {
		if (SkelePos.size() < 1) {
			SkelePos.push_back({ 305, 305 });
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
	void Draw(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, olc::vf2d PlayerPos, float PlayerSpeed) {
		Spawn();
		for (int k = 0; k < SkelePos.size(); k++) {
			olc::vi2d Skele(SkelePos[k].x + 0.5, SkelePos[k].y + 0.1f);
			olc::vi2d UPlayerPos(PlayerPos.x + 0.4f, PlayerPos.y + 0.11f);

			olc::vf2d dir = (PlayerPos - SkelePos[k]).norm();

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
			olc::vf2d MousePos = { GetWorldMousePos(tv, pge) };
			olc::vf2d PlayerDir = (-(PlayerPos - MousePos).norm());
			float angleTowards = PointTo(PlayerPos, SkelePos[k]); //Calculate the angle towards a target.
			float angleDiff = angleDifference(PlayerDir.polar().y, angleTowards); //Calculate the difference between the target and the angle.
			if (
				sqrt(pow(PlayerPos.x - SkelePos[k].x, 2) + pow(PlayerPos.y - SkelePos[k].y, 2)) < maxDistance //Check to see if the target is in range (distance formula)
				&& abs(angleDiff) < maxAngle  //See if the target's angle is within the sweeping arc range.
				&& pge->GetMouse(0).bHeld) {
				tv.DrawDecal({ SkelePos[k].x + 0.05f, SkelePos[k].y + 1.1f }, ShadowDecal, { 2.0f, 2.0f });
				tv.DrawDecal({ SkelePos[k].x - 0.15f, SkelePos[k].y - 0.15f }, SkeleRightDecal, {2.3f, 2.3f});
				tv.DrawDecal({ SkelePos[k].x - 0.15f, SkelePos[k].y - 0.15f }, SkeleRightHurtDecal, { 2.3f, 2.3f });
			}
			else {
				tv.DrawDecal({ SkelePos[k].x + 0.05f, SkelePos[k].y + 1.1f }, ShadowDecal, { 1.8f, 1.8f });
				tv.DrawDecal({ SkelePos[k] }, SkeleRightDecal, { 2.0f, 2.0f });
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

		//Get ClosestSkelePos
		olc::vf2d ClosestSkelePos = Skeleton.ReturnClosestPos(PlayerPos);

		//If Closest Skeleton is above player
		if (ClosestSkelePos.y <= PlayerPos.y) {
			//Enemies
			Skeleton.Draw(tv, this, PlayerPos, PlayerSpeed);

			//Draw Archer
			Player.Draw(tv);
		}

		//If Closest Skeleton is below player
		else {
			//Draw Archer
			Player.Draw(tv);

			//Enemies
			Skeleton.Draw(tv, this, PlayerPos, PlayerSpeed);
		}

		//Draw variables
		GUI.DrawDebugVariables(tv, this, PlayerPos, CharacterHealth);

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
			GUI.doPauseScreen(tv, this, CharacterHealth, PlayerPos);
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
// 1. Implement a "camera" https://community.onelonecoder.com/members/javidx9/Camera/
// 2. Implement a passive mob case (idle, wandering)
// 3. Implement an aggressive enemies attack case (Straight at player, around the player, running away, etc)