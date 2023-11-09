
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

class Pixel_Adventure : public olc::PixelGameEngine {
public:

	//Sprites
	std::unique_ptr<olc::Sprite> Grass;
	std::unique_ptr<olc::Sprite> PauseScreen;
	std::unique_ptr<olc::Sprite> FullHeart;
	std::unique_ptr<olc::Sprite> HalfHeart;
	std::unique_ptr<olc::Sprite> EmptyHeart;
	std::unique_ptr<olc::Sprite> ArcherRight;
	std::unique_ptr<olc::Sprite> ArcherLeft;
	//Decals
	olc::Decal* GrassDecal;
	olc::Decal* PauseScreenDecal;
	olc::Decal* FullHeartDecal;
	olc::Decal* HalfHeartDecal;
	olc::Decal* EmptyHeartDecal;
	olc::Decal* ArcherRightDecal;
	olc::Decal* ArcherLeftDecal;
	//GameState
	enum GameStateEnum { DEBUG, GAME, PAUSED, EXIT, MENU };
	std::vector<GameStateEnum> GameState;
	//Main Menu
	int Selection = 0;
	//Character Variables
	olc::vf2d PlayerPos = { 300.0f, 300.0f };
	bool ArcherDir = true;
	int CharacterHealth = 6;

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

	Pixel_Adventure() {
		sAppName = "Pixel Adventure";
	}

	//User inputs
	void UserInput(float PlayerSpeed, float fElapsedTime) {
		if ((GetKey(olc::Key::LEFT).bHeld || (GetKey(olc::Key::A).bHeld)) && PlayerPos.x < 965 && PlayerPos.x > -8) {
			PlayerPos.x -= PlayerSpeed;
			ArcherDir = false;
		}
		if ((GetKey(olc::Key::RIGHT).bHeld || (GetKey(olc::Key::D).bHeld)) && PlayerPos.x < 965 && PlayerPos.x > -8) {
			PlayerPos.x += PlayerSpeed;
			ArcherDir = true;
		}
		if ((GetKey(olc::Key::UP).bHeld || (GetKey(olc::Key::W).bHeld)) && PlayerPos.y < 575 && PlayerPos.y > -5) {
			PlayerPos.y -= PlayerSpeed;
		}
		if ((GetKey(olc::Key::DOWN).bHeld || (GetKey(olc::Key::S).bHeld)) && PlayerPos.y < 575 && PlayerPos.y > -5) {
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
		if (GetKey(olc::Key::ESCAPE).bPressed) {
			GameState[1] = GameState[0];
			GameState[0] = PAUSED;
		}
		if (GetMouse(0).bPressed) {
			CharacterHealth--;
		}
	}
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
	void DrawGrass() {
		for (int x = 0; x < 1024; x += 32) {
			for (int y = 0; y < 576; y += 32) {
				DrawDecal({ (float)x, (float)y }, GrassDecal);
			}
		}
	}
	void DrawCharacterHealth() {
		if (CharacterHealth == 6) {
			DrawDecal({ 10.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
			DrawDecal({ 47.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
			DrawDecal({ 84.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
		}
		if (CharacterHealth == 5) {
			DrawDecal({ 10.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
			DrawDecal({ 47.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
			DrawDecal({ 84.0f, 534.0f }, HalfHeartDecal, { 1.0f, 1.0f });
		}
		if (CharacterHealth == 4) {
			DrawDecal({ 10.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
			DrawDecal({ 47.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
			DrawDecal({ 84.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
		}
		if (CharacterHealth == 3) {
			DrawDecal({ 10.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
			DrawDecal({ 47.0f, 534.0f }, HalfHeartDecal, { 1.0f, 1.0f });
			DrawDecal({ 84.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
		}
		if (CharacterHealth == 2) {
			DrawDecal({ 10.0f, 534.0f }, FullHeartDecal, { 1.0f, 1.0f });
			DrawDecal({ 47.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
			DrawDecal({ 84.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
		}
		if (CharacterHealth == 1) {
			DrawDecal({ 10.0f, 534.0f }, HalfHeartDecal, { 1.0f, 1.0f });
			DrawDecal({ 47.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
			DrawDecal({ 84.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
		}
		if (CharacterHealth <= 0) {
			DrawDecal({ 10.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
			DrawDecal({ 47.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
			DrawDecal({ 84.0f, 534.0f }, EmptyHeartDecal, { 1.0f, 1.0f });
		}
	}
	void doPauseScreen() {

		const olc::vf2d scale = { 1.0f, 1.0f };
		//Resume variables
		const bool resume_hovered = (GetMouseX() >= 400 && GetMouseY() >= 158 && GetMouseX() <= 634 && GetMouseY() <= 194);
		const float resume_zoom = resume_hovered ? 5.5f : 5.0f;
		const float resume_XCoord = resume_hovered ? 411.0f : 420.0f;
		const float resume_YCoord = resume_hovered ? 168.0f : 170.0f;
		//Options variables
		const  bool options_hovered = (GetMouseX() >= 400 && GetMouseY() >= 250 && GetMouseX() <= 675 && GetMouseY() <= 285);
		const float options_zoom = options_hovered ? 5.5f : 5.0f;
		const float options_XCoord = options_hovered ? 388.0f : 400.0f;
		const float options_YCoord = options_hovered ? 258.0f : 260.0f;
		//Quit variables
		const bool quit_hovered = (GetMouseX() >= 460 && GetMouseY() >= 350 && GetMouseX() <= 620 && GetMouseY() <= 385);
		const float quit_zoom = quit_hovered ? 5.5 : 5.0f;
		const float quit_XCoord = quit_hovered ? 453.0f : 460.0f;
		const float quit_YCoord = quit_hovered ? 348.0f : 350.0f;

		//Draw background (W.I.P)
		//Draw Archer
		if (ArcherDir == true) {
			DrawDecal({ PlayerPos }, ArcherRightDecal, { 2.0f, 2.0f });
		}
		if (ArcherDir == false) {
			DrawDecal({ PlayerPos }, ArcherLeftDecal, { 2.0f, 2.0f });
		}

		//Draw hearts
		DrawCharacterHealth();

		//Draw pause opacity
		DrawDecal({ 0.0f, 0.0f }, PauseScreenDecal, { 1.0f, 1.0f });

		DrawStringDecal({ resume_XCoord, resume_YCoord }, "RESUME", olc::WHITE, scale * resume_zoom);
		DrawStringDecal({ options_XCoord, options_YCoord }, "OPTIONS", olc::WHITE, scale * options_zoom);
		DrawStringDecal({ quit_XCoord, quit_YCoord }, "QUIT", olc::WHITE, scale * quit_zoom);

		//Draw variables
		DrawDebugVariables();

		PauseScreenInputs(resume_hovered, options_hovered, quit_hovered);
	}
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
	void DrawDebugVariables() {
		//PlayerPos
		std::string ArcherX = std::to_string(PlayerPos.x);
		std::string ArcherY = std::to_string(PlayerPos.y);

		DrawStringDecal({ 10.0f, 10.0f }, "ArcherX", olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 140.0f, 10.0f }, ArcherX, olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 10.0f, 30.0f }, "ArcherY", olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 140.0f, 30.0f }, ArcherY, olc::WHITE, { 2.0f, 2.0f });

		//MousePos
		std::string MouseX = std::to_string(GetMouseX());
		std::string MouseY = std::to_string(GetMouseY());

		DrawStringDecal({ 10.0f, 60.0f }, "MouseX", olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 140.0f, 60.0f }, MouseX, olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 10.0f, 80.0f }, "MouseY", olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 140.0f, 80.0f }, MouseY, olc::WHITE, { 2.0f, 2.0f });
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
		fElapsedTime = std::min(fElapsedTime, 0.16667f);
		float PlayerSpeed = 200 * fElapsedTime;

		UserInput(PlayerSpeed, fElapsedTime);

		//Draw Archer
		if (ArcherDir == true) {
			tv.DrawDecal({ PlayerPos }, ArcherRightDecal, { 2.0f, 2.0f });
		}
		if (ArcherDir == false) {
			tv.DrawDecal({ PlayerPos }, ArcherLeftDecal, { 2.0f, 2.0f });
		}
	}
	void DebugGameState(float fElapsedTime) {
		//Speed
		fElapsedTime = std::min(fElapsedTime, 0.16667f);
		float PlayerSpeed = 8 * fElapsedTime;

		//Camera variables
		bool bOnScreen = camera.Update(fElapsedTime);
		tv.SetWorldOffset(camera.GetViewPosition());

		UserInput(PlayerSpeed, fElapsedTime);

		DrawBGCamera();

		//Draw Archer
		if (ArcherDir == true) {
			tv.DrawDecal({ PlayerPos }, ArcherRightDecal, { 2.0f, 2.0f });
		}
		if (ArcherDir == false) {
			tv.DrawDecal({ PlayerPos }, ArcherLeftDecal, { 2.0f, 2.0f });
		}

		//Draw variables
		DrawDebugVariables();

		//Draw Hearts
		DrawCharacterHealth();
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
			doPauseScreen();
		}
		if (GameState[0] == EXIT) {
			return false;
		}
	}
private:
	bool OnUserCreate() override {

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
		PauseScreen = std::make_unique<olc::Sprite>("./Sprites/PauseScreen.png");
		FullHeart = std::make_unique<olc::Sprite>("./Sprites/FullHeart.png");
		HalfHeart = std::make_unique<olc::Sprite>("./Sprites/HalfHeart.png");
		EmptyHeart = std::make_unique<olc::Sprite>("./Sprites/EmptyHeart.png");
		ArcherRight = std::make_unique<olc::Sprite>("./Sprites/ArcherRight.png");
		ArcherLeft = std::make_unique<olc::Sprite>("./Sprites/ArcherLeft.png");
		//Decals
		GrassDecal = new olc::Decal(Grass.get());
		PauseScreenDecal = new olc::Decal(PauseScreen.get());
		FullHeartDecal = new olc::Decal(FullHeart.get());
		HalfHeartDecal = new olc::Decal(HalfHeart.get());
		EmptyHeartDecal = new olc::Decal(EmptyHeart.get());
		ArcherRightDecal = new olc::Decal(ArcherRight.get());
		ArcherLeftDecal = new olc::Decal(ArcherLeft.get());
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