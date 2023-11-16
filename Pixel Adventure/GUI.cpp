#include "GUI.h"
#include "MathFunctions.h"
#include "GlobalVars.h"

void GUI::Initialize(olc::PixelGameEngine* pge) {
    PauseScreen = std::make_unique<olc::Sprite>("./Sprites/PauseScreen.png");
    FullHeart = std::make_unique<olc::Sprite>("./Sprites/FullHeart.png");
    HalfHeart = std::make_unique<olc::Sprite>("./Sprites/HalfHeart.png");
    EmptyHeart = std::make_unique<olc::Sprite>("./Sprites/EmptyHeart.png");

    PauseScreenDecal = new olc::Decal(PauseScreen.get());
    FullHeartDecal = new olc::Decal(FullHeart.get());
    HalfHeartDecal = new olc::Decal(HalfHeart.get());
    EmptyHeartDecal = new olc::Decal(EmptyHeart.get());
}

void GUI::Hearts(olc::PixelGameEngine* pge, int CharacterHealth) {
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
void GUI::DrawDebugVariables(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, olc::vf2d PlayerPos, int CharacterHealth, bool PlayerCanAttack) {
	//PlayerPos
	std::string PlayerX = std::to_string(PlayerPos.x);
	std::string PlayerY = std::to_string(PlayerPos.y);

	pge->DrawStringDecal({ 10.0f, 10.0f }, "PlayerX", olc::WHITE, { 2.0f, 2.0f });
	pge->DrawStringDecal({ 140.0f, 10.0f }, PlayerX, olc::WHITE, { 2.0f, 2.0f });
	pge->DrawStringDecal({ 10.0f, 30.0f }, "PlayerY", olc::WHITE, { 2.0f, 2.0f });
	pge->DrawStringDecal({ 140.0f, 30.0f }, PlayerY, olc::WHITE, { 2.0f, 2.0f });

	//MousePos
	olc::vf2d MousePos = { MathFunctions::GetWorldMousePos(tv, pge) };
	std::string MouseX = std::to_string(MousePos.x);
	std::string MouseY = std::to_string(MousePos.y);

	pge->DrawStringDecal({ 10.0f, 60.0f }, "MouseX", olc::WHITE, { 2.0f, 2.0f });
	pge->DrawStringDecal({ 140.0f, 60.0f }, MouseX, olc::WHITE, { 2.0f, 2.0f });
	pge->DrawStringDecal({ 10.0f, 80.0f }, "MouseY", olc::WHITE, { 2.0f, 2.0f });
	pge->DrawStringDecal({ 140.0f, 80.0f }, MouseY, olc::WHITE, { 2.0f, 2.0f });

	std::string Health = std::to_string(CharacterHealth);

	pge->DrawStringDecal({ 10.0f, 120.0f }, "Health", olc::WHITE, { 2.0f, 2.0f });
	pge->DrawStringDecal({ 130.0f, 120.0f }, Health, olc::WHITE, { 2.0f, 2.0f });

	std::string Attack = std::to_string(PlayerCanAttack);

	pge->DrawStringDecal({ 10.0f, 160.0f }, "Attack", olc::WHITE, { 2.0f, 2.0f });
	pge->DrawStringDecal({ 130.0f, 160.0f }, Attack, olc::WHITE, { 2.0f, 2.0f });

	std::string FPS = std::to_string(pge->GetFPS());
	pge->DrawStringDecal({ 500.0f, 10.0f }, "FPS", olc::WHITE, { 2.0f, 2.0f });
	pge->DrawStringDecal({ 560.0f, 10.0f }, FPS, olc::WHITE, { 2.0f, 2.0f });
}
GlobalVars::GameStateEnum GUI::PauseScreenInput(olc::PixelGameEngine* pge, bool resume_hovered, bool options_hovered, bool quit_hovered) {
	if ((pge->GetMouse(0).bPressed && resume_hovered == true) || pge->GetKey(olc::Key::ESCAPE).bPressed) {
		GlobalVars::GameState[0] = GlobalVars::GameState[1];
	}
	if (pge->GetMouse(0).bPressed && options_hovered == true) {

	}
	if (pge->GetMouse(0).bPressed && quit_hovered == true) {
		GlobalVars::GameState[0] = GlobalVars::MENU;
	}
	return GlobalVars::GameState[0], GlobalVars::GameState[1];
}
void GUI::doPauseScreen(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, int CharacterHealth, olc::vf2d PlayerPos, bool PlayerCanAttack) {
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
	if (GlobalVars::GameState[1] == GlobalVars::DEBUG) {
		DrawDebugVariables(tv, pge, PlayerPos, CharacterHealth, PlayerCanAttack);
	}

	PauseScreenInput(pge, resume_hovered, options_hovered, quit_hovered);
}
