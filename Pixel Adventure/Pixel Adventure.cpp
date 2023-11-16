#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"
#include "olcUTIL_Camera2D.h"

#define GUI
#include "GUI.h"
#define GlobalVers
#include "GlobalVars.h"
#define MathFunctions
#include "MathFunctions.h"
#define Player
#include "Player.h"
#define EnemyFunctions
#include "EnemyFunctions.h"
#define Skeleton
#include "Skeleton.h"

std::vector<GameStateEnum> GameState;

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