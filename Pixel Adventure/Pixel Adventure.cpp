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

//GameState
enum GameStateEnum { DEBUG, GAME, PAUSED, EXIT, MENU };
std::vector<GameStateEnum> GameState;

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
	void DrawDebugVariables(olc::PixelGameEngine* pge, olc::vf2d PlayerPos, int CharacterHealth) {
		//PlayerPos
		std::string ArcherX = std::to_string(PlayerPos.x);
		std::string ArcherY = std::to_string(PlayerPos.y);

		pge->DrawStringDecal({ 10.0f, 10.0f }, "ArcherX", olc::WHITE, { 2.0f, 2.0f });
		pge->DrawStringDecal({ 140.0f, 10.0f }, ArcherX, olc::WHITE, { 2.0f, 2.0f });
		pge->DrawStringDecal({ 10.0f, 30.0f }, "ArcherY", olc::WHITE, { 2.0f, 2.0f });
		pge->DrawStringDecal({ 140.0f, 30.0f }, ArcherY, olc::WHITE, { 2.0f, 2.0f });

		//MousePos
		std::string MouseX = std::to_string(pge->GetMouseX());
		std::string MouseY = std::to_string(pge->GetMouseY());

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
	void doPauseScreen(olc::PixelGameEngine* pge, int CharacterHealth, olc::vf2d PlayerPos) {
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
			DrawDebugVariables(pge, PlayerPos, CharacterHealth);
		}

		PauseScreenInput(pge, resume_hovered, options_hovered, quit_hovered);
	}
};
class Player {
public:

	//Sprites
	std::unique_ptr<olc::Sprite> ArcherRight;
	std::unique_ptr<olc::Sprite> ArcherLeft;

	//Decals
	olc::Decal* ArcherRightDecal;
	olc::Decal* ArcherLeftDecal;

	olc::vf2d PlayerPos{ 300.0f, 300.0f };
	bool ArcherDir = true;
	int CharacterHealth = 6;

	void Draw(olc::TileTransformedView& tv) {
		//Draw Archer
		if (ArcherDir == true) {
			tv.DrawDecal({ PlayerPos }, ArcherRightDecal, { 2.0f, 2.0f });
		}
		if (ArcherDir == false) {
			tv.DrawDecal({ PlayerPos }, ArcherLeftDecal, { 2.0f, 2.0f });
		}
	}
	olc::vf2d Input(olc::PixelGameEngine* pge, float PlayerSpeed){
		if ((pge->GetKey(olc::Key::LEFT).bHeld || (pge->GetKey(olc::Key::A).bHeld)) && PlayerPos.x < 965 && PlayerPos.x > -8) {
			PlayerPos.x -= PlayerSpeed;
			ArcherDir = false;
		}
		if ((pge->GetKey(olc::Key::RIGHT).bHeld || (pge->GetKey(olc::Key::D).bHeld)) && PlayerPos.x < 965 && PlayerPos.x > -8) {
			PlayerPos.x += PlayerSpeed;
			ArcherDir = true;
		}
		if ((pge->GetKey(olc::Key::UP).bHeld || (pge->GetKey(olc::Key::W).bHeld)) && PlayerPos.y < 575 && PlayerPos.y > -5) {
			PlayerPos.y -= PlayerSpeed;
		}
		if ((pge->GetKey(olc::Key::DOWN).bHeld || (pge->GetKey(olc::Key::S).bHeld)) && PlayerPos.y < 575 && PlayerPos.y > -5) {
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
		//Decals
		ArcherRightDecal = new olc::Decal(ArcherRight.get());
		ArcherLeftDecal = new olc::Decal(ArcherLeft.get());
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
	olc::vf2d MousePos;
	//Arch variables
	float radius = 8.0f; // Radius of the arch (half of the desired length)

	olc::vf2d GoopPos = { 280.0f, 280.0f }; // Position of the Goop
	olc::vf2d GoopSize = { 16.0f, 16.0f }; // Size of the Goop
	bool ArchDrawn = false;

	Player Player;
	GUI GUI;

	Pixel_Adventure() {
		sAppName = "Pixel Adventure";
	}
	olc::vf2d GetWorldMousePos() {
		return GetMousePos() + tv.GetWorldOffset();
	}
	float PointTo(olc::vf2d pos1, olc::vf2d pos2) {
		return atan2(pos2.y - pos1.y, pos2.x - pos1.x);
	}
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
	void DrawPlayerArch() {
		// Get the mouse cursor position
		int mouseX = GetMouseX();
		int mouseY = GetMouseY();

		// Calculate the direction vector from the origin to the cursor position
		olc::vf2d dir = { static_cast<float>(mouseX) - PlayerPos.x, static_cast<float>(mouseY) - PlayerPos.y };

		// Calculate the angle between the cursor vector and the right vector (1, 0)
		float angle = atan2(dir.y, dir.x);

		// Draw the arch as two lines forming a 30-degree angle
		float halfAngle = 30.0f * (static_cast<float>(M_PI) / 180.0f); // 30 degrees in radians
		olc::vf2d p1 = PlayerPos + radius * olc::vf2d(cos(angle - halfAngle), sin(angle - halfAngle));
		olc::vf2d p2 = PlayerPos + radius * olc::vf2d(cos(angle + halfAngle), sin(angle + halfAngle));

		// Draw the lines forming the arch
		tv.DrawLineDecal({ PlayerPos.x, PlayerPos.y }, { p1.x, p1.y }, olc::WHITE);
		tv.DrawLineDecal({ PlayerPos.x, PlayerPos.y }, { p2.x, p2.y }, olc::WHITE);

		ArchDrawn = true;
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
		float PlayerSpeed = 8 * fElapsedTime;

		//Camera variables
		bool bOnScreen = camera.Update(fElapsedTime);
		tv.SetWorldOffset(camera.GetViewPosition());

		PlayerPos = Player.Input(this, PlayerSpeed);
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

		PlayerPos = Player.Input(this, PlayerSpeed);
		//CharacterHealth = Player.HealthTest(this);
		Player.EscapeInput(this);

		DrawBGCamera();

		//Draw Archer
		Player.Draw(tv);

		if (GetMouse(0).bPressed) {
			//DrawPlayerArch();
		}
		MousePos = { GetWorldMousePos() };
		olc::vf2d PlayerDir = (PlayerPos - MousePos).norm();
		float angleTowards = PointTo(PlayerPos, GoopPos); //Calculate the angle towards a target.
		float angleDiff = angleDifference(PlayerDir.polar().y, angleTowards); //Calculate the difference between the target and the angle.
		float maxDistance = 24; //How far away the object can be to still be in range.
		float maxAngle = M_PI / 4; //The total sweeping angle of the arch in either direction (PI/4 is 45 degrees in either direction, 90 degrees total).

		if (
			sqrt(pow(PlayerPos.x - GoopPos.x, 2) + pow(PlayerPos.y - GoopPos.y, 2)) < maxDistance //Check to see if the target is in range (distance formula)
			&& abs(angleDiff) < maxAngle  //See if the target's angle is within the sweeping arc range.
			&& GetMouse(0).bHeld) {
			//Target is in range.
			tv.FillRectDecal({ GoopPos }, { 2.0f, 2.0f }, olc::RED);
		}
		else {
			tv.FillRectDecal({ GoopPos }, { 2.0f, 2.0f }, olc::GREEN);//Target is out of range.
		}

		//Draw variables
		GUI.DrawDebugVariables(this, PlayerPos, CharacterHealth);

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
			GUI.doPauseScreen(this, CharacterHealth, PlayerPos);
		}
		if (GameState[0] == EXIT) {
			return false;
		}
	}
private:
	bool OnUserCreate() override {

		GameState.push_back(MENU);
		GameState.push_back(MENU);

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