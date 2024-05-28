#pragma once
#include "olcPixelGameEngine.h"

class Map_Editor;

class cCell
{
public:
	cCell(float px, float py, std::string n);
	~cCell();


public:
	float px, py;


	std::string sName;

public:
	virtual void DrawSelf(olc::PixelGameEngine* gfx, float fofx, float fofy) {};

	static Map_Editor* g_engine;


protected:
	
};

class cSprite : public cCell
{
public:
	cSprite(float px, float py, float spx, float spy, float scX, float scY, int nLayer, olc::Decal* sprite);


public:

	void DrawSelf(olc::PixelGameEngine* gfx, float fofx, float fofy) override;


public:
	

	int collisionTipe;
	float spx, spy;
	float scX, scY;
	int nLayer;



protected:
	olc::Decal* m_pSprite;
};
