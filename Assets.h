#pragma once
#include "olcPixelGameEngine.h"
#include <map>

class cMap;
class RPG_Assets
{
public:
	static RPG_Assets& get()
	{

		static RPG_Assets me;
		return me;
	}

	RPG_Assets(RPG_Assets const&) = delete;           //we delete it becouse we don/t want have any copies of this 
	void operator =(RPG_Assets const&) = delete;



	olc::Decal* GetSprite(std::string name)
	{
		return m_mapSprites[name];       //filled it in Load Sprites

	}

	cMap* GetMap(std::string name)
	{
		return m_mapMaps[name];
	}

	void LoadSprites();
	void LoadMaps();

private:
	RPG_Assets();
	~RPG_Assets();


	std::map<std::string, olc::Decal*> m_mapSprites;
	std::map<std::string, cMap*> m_mapMaps;
};

