#include "Cell.h"
#include "Map_Editor.h"

Map_Editor* cCell::g_engine = nullptr;


cCell::cCell(float px, float py, std::string n)
{
	this->px = px;
	this->py = py;

	sName = n;
	
}

cCell::~cCell()
{
}





cSprite::cSprite (float px, float py, float spx, float spy, float scX, float scY, int nLayer, olc::Decal* sprite) :cCell( px, py, "Sprite")
{
	m_pSprite = sprite;
	this->px = px;
	this->py = py;
	this->spx = spx;
	this->spy = spy;
	this->scX = scX;
	this->scY = scY;
	this->nLayer = nLayer;
}

void cSprite::DrawSelf(olc::PixelGameEngine* gfx, float ox, float oy)
{


	gfx->SetDrawTarget(nLayer);
	gfx->SetPixelMode(olc::Pixel::ALPHA);
	gfx->DrawPartialDecal({ (px - ox) * 64.0f,(py - oy) * 64.0f }, m_pSprite, { spx,spy }, { scX,scY });
	gfx->SetPixelMode(olc::Pixel::NORMAL);
	gfx->EnableLayer(nLayer, true);
	gfx->SetDrawTarget(nullptr);

}
