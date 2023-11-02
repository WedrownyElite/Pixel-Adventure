#define OLC_PGE_APPLICATION
#define _USE_MATH_DEFINES
#include "olcPixelGameEngine.h"
#include <iostream>
#include <vector>
#include <cmath>

class Pixel_Adventure : public olc::PixelGameEngine {
public:
	//Sprites
	std::unique_ptr<olc::Sprite> FullHeart;
	std::unique_ptr<olc::Sprite> HalfHeart;
	std::unique_ptr<olc::Sprite> EmptyHeart;
	std::unique_ptr<olc::Sprite> ArcherRight;
	std::unique_ptr<olc::Sprite> ArcherLeft;
	//Decals
	olc::Decal* FullHeartDecal;
	olc::Decal* HalfHeartDecal;
	olc::Decal* EmptyHeartDecal;
	olc::Decal* ArcherRightDecal;
	olc::Decal* ArcherLeftDecal;
	//GameState
	enum GameStateEnum { DEBUG, GAME, PAUSED, EXIT, MENU };
	GameStateEnum GameState = MENU;
	//Main Menu
	int Selection = 0;
	//Character Variables
	olc::vf2d ArcherPos = { 300.0f, 300.0f };
	bool ArcherDir = true;
	int CharacterHealth = 6;
	Pixel_Adventure() {
		sAppName = "Pixel Adventure";
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
	void UserInput(float PlayerSpeed, float fElapsedTime) {
		if ((GetKey(olc::Key::LEFT).bHeld || (GetKey(olc::Key::A).bHeld)) && ArcherPos.x < 965 && ArcherPos.x > -8) {
			ArcherPos.x -= PlayerSpeed;
			ArcherDir = false;
		}
		if ((GetKey(olc::Key::RIGHT).bHeld || (GetKey(olc::Key::D).bHeld)) && ArcherPos.x < 965 && ArcherPos.x > -8) {
			ArcherPos.x += PlayerSpeed;
			ArcherDir = true;
		}
		if ((GetKey(olc::Key::UP).bHeld || (GetKey(olc::Key::W).bHeld)) && ArcherPos.y < 575 && ArcherPos.y > -5) {
			ArcherPos.y -= PlayerSpeed;
		}
		if ((GetKey(olc::Key::DOWN).bHeld || (GetKey(olc::Key::S).bHeld)) && ArcherPos.y < 575 && ArcherPos.y > -5) {
			ArcherPos.y += PlayerSpeed;
		}
		if (ArcherPos.x > 965) {
			ArcherPos.x = 964;
		}
		if (ArcherPos.x < -8) {
			ArcherPos.x = -7;
		}
		if (ArcherPos.y > 512) {
			ArcherPos.y = 511;
		}
		if (ArcherPos.y < -5) {
			ArcherPos.y = -4;
		}
		if (GetKey(olc::Key::ESCAPE).bPressed) {
			GameState = EXIT;
		}
		if (GetMouse(0).bPressed) {
			CharacterHealth--;
		}
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
			GameState = GAME;
		}
		if (Selection == 1 && GetKey(olc::Key::ENTER).bPressed) {
			GameState = DEBUG;
		}
		if (GetKey(olc::Key::ESCAPE).bPressed) {
			GameState = EXIT;
		}
	}
	void GameGameState(float fElapsedTime) {
		fElapsedTime = std::min(fElapsedTime, 0.16667f);
		float PlayerSpeed = 200 * fElapsedTime;

		UserInput(PlayerSpeed, fElapsedTime);

		//Draw Archer
		if (ArcherDir == true) {
			DrawDecal({ ArcherPos }, ArcherRightDecal, { 2.0f, 2.0f });
		}
		if (ArcherDir == false) {
			DrawDecal({ ArcherPos }, ArcherLeftDecal, { 2.0f, 2.0f });
		}
	}
	void DebugGameState(float fElapsedTime) {
		fElapsedTime = std::min(fElapsedTime, 0.16667f);
		float PlayerSpeed = 400 * fElapsedTime;

		UserInput(PlayerSpeed, fElapsedTime);
		DrawCharacterHealth();

		std::string ArcherX = std::to_string(ArcherPos.x);
		std::string ArcherY = std::to_string(ArcherPos.y);

		DrawStringDecal({ 10.0f, 10.0f }, ArcherX, olc::WHITE, { 2.0f, 2.0f });
		DrawStringDecal({ 10.0f, 30.0f }, ArcherY, olc::WHITE, { 2.0f, 2.0f });
		//Draw Archer
		if (ArcherDir == true) {
			DrawDecal({ ArcherPos }, ArcherRightDecal, { 2.0f, 2.0f });
		}
		if (ArcherDir == false) {
			DrawDecal({ ArcherPos }, ArcherLeftDecal, { 2.0f, 2.0f });
		}
	}
	bool OnUserUpdate(float fElapsedTime) override {
		if (GameState == MENU) {
			MainMenu();
			return true;
		}
		if (GameState == GAME) {
			GameGameState(fElapsedTime);
			return true;
		}
		if (GameState == DEBUG) {
			DebugGameState(fElapsedTime);
			return true;
		}
		if (GameState == EXIT) {
			return false;
		}
	}
private:
	bool OnUserCreate() override {

		//Sprites
		FullHeart = std::make_unique<olc::Sprite>("./Sprites/FullHeart.png");
		HalfHeart = std::make_unique<olc::Sprite>("./Sprites/HalfHeart.png");
		EmptyHeart = std::make_unique<olc::Sprite>("./Sprites/EmptyHeart.png");
		ArcherRight = std::make_unique<olc::Sprite>("./Sprites/ArcherRight.png");
		ArcherLeft = std::make_unique<olc::Sprite>("./Sprites/ArcherLeft.png");
		//Decals
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