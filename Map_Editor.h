#pragma once
#include "olcPixelGameEngine.h"
#include "Assets.h"
#include "Maps.h"
#include "Cell.h"
#include <conio.h>


class Map_Editor : public olc::PixelGameEngine
{

public:
	Map_Editor();

private:

	float fOffsetX = 0.0f;
	float fOffsetY = 0.0f;

	float fCameraPosX = 0.0f;
	float fCameraPosY =0.0f;

	float addspeed = 1.0f;

	float fScaleX = 1.0f;
	float fScaleY = 1.0f;

	float fStartPanX = 0.0f;
	float fStartPanY = 0.0f;

	float fSelectedCellX = 0.0f;
	float fSelectedCellY = 0.0f;

	float fStSelRectX = 0.0f;
	float fStSelRectY = 0.0f;

	float fEnSelRectX = 0.0f;
	float fEnSelRectY = 0.0f;

	float savestrecX = 0.0f;
	float savestrecY = 0.0f;
	float saveenrecX = 0.0f;
	float saveenrecY = 0.0f;

	
	int nLayerBackground = 1;
	int nLayerZero = 0;
	int nLayerOne = 1;
	int nLayerTwo = 2;
	int nLayerThree = 3;
	
	float fMX = 0;
	float fMY = 0;

	float framespeed = 0.1f;
	float animsprite = 0.0f;
	int frameIndex = 0;

	cMap* m_pCurrentMap = nullptr;
	olc::Decal* m_StaticLevel = nullptr;
	olc::Decal* DynamicLayer = nullptr;

	olc::vi2d vTIleCursor = { 0,0 };
	olc::vi2d vTileSize = { 19.2f,19.2f };

	bool inEditmode = false;

	int CollNumber = 0;
	
	//Draw Level
	int nTileWidth = 64;
	int nTileHeight = 64;

	int nVisibleTileX = ScreenWidth() / nTileWidth;
	int nVisibleTileY = ScreenHeight() / nTileHeight;


	std::vector<cCell*> m_vecCells;    // Fixed

	cCell* currentCell;


	float fixfScaleX = 1.0f;
	float fixfSacelY= 1.0f;



	bool bseeThird = true;
	bool bseeFirst = true;
	bool bseeZero = true;
	bool bseeSecond = true;
	bool bseeDynamic = true;
	int currLayer = 1;

	bool bWriteWidth = false;

	std::string SWidth ="256";
	std::string SHeight = "64";
	
protected:


	// Convert coordinates from World Space --> Screen Space
	void WorldToScreen(float fWorldX, float fWorldY, int& nScreenX, int& nScreenY);
	

	// Convert coordinates from Screen Space --> World Space
	void ScreenToWorld(int nScreenX, int nScreenY, float& fWorldX, float& fWorldY);
	
	void DescribeRect();

	void OnTextEntryComplete(const std::string& sText) override
	{
		std::cout << "Text entry: " + sText << std::endl;

		if (bWriteWidth)
		{
			SWidth = sText;
		}
		else
		{
			SHeight = sText;
		}

	}


public:

	bool OnUserCreate() override;
	bool OnUserUpdate(float fElapsedTime) override;

	bool CheckGrabbedObject(float fselectedCellX, float fselectedCelly);


	void DrawStaticFrame(int recFirstX, int recFirstY, int RecSecX, int RecSecY, float fscale);
	void DrawDynamicLayer(float px, float py, float fscale, float felapsedtime);


};


