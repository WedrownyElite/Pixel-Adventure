#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "GlobalVars.h"

class GUI {
private:
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

public:
    void Initialize(olc::PixelGameEngine* pge);
    void Hearts(olc::PixelGameEngine* pge, int CharacterHealth);
    void DrawDebugVariables(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, olc::vf2d PlayerPos, int CharacterHealth, bool PlayerCanAttack, bool PlayerMoving);
    GlobalVars::GameStateEnum PauseScreenInput(olc::PixelGameEngine* pge, bool resume_hovered, bool options_hovered, bool quit_hovered);
    void doPauseScreen(olc::TileTransformedView& tv, olc::PixelGameEngine* pge, int CharacterHealth, olc::vf2d PlayerPos, bool PlayerCanAttack);
};
