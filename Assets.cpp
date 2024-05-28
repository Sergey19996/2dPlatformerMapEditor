#include "Assets.h"
#include "Maps.h"



void RPG_Assets::LoadMaps()
{
	auto load = [&](cMap* m)
	{
		m_mapMaps[m->sName] = m;
	};

	load(new cMap_Grid());
}

RPG_Assets::~RPG_Assets()
{
}
RPG_Assets::RPG_Assets()
{
}


void RPG_Assets::LoadSprites()
{

	auto load = [&](std::string sName, std::string sFileName)
	{
		olc::Sprite* s = new olc::Sprite(sFileName);
		olc::Decal* d = new olc::Decal(s);
		m_mapSprites[sName] = d;     //map array <key,value>
	};

	load("GridSquere", "graphs/squere.png");
	load("Eye", "graphs/Eye.png");
	load("Lvl1Constructor", "graphs/ForestLvl1Constructor.png");
	load("CellConstructor", "graphs/Cellconstructor.png");
	load("DynamicLayer", "graphs/DynamicLayer.png");
	load("StaticFrame", "graphs/StaticFrame.png");
	load("BackGroundColor", "graphs/BackGroundColor.png");
}