#pragma once
#include "olcPixelGameEngine.h"
#include "Assets.h"
class cMap
{

public:
	cMap();
	~cMap();


public:
	int nWidth;
	int nHeight;
	int* m_indices = nullptr;   // Grid


	int* m_zerolayer = nullptr; // Grid
	int* m_firstlayer = nullptr; // Grid
	int* m_secondlayer = nullptr; // Grid
	int* m_thirdlayer = nullptr; // Grid
	int* m_DynamicLayer = nullptr;




	int* m_Collision = nullptr;  //Collision


	std::string sName;
	olc::Decal* pSprite;           //<--Sprite



	bool Create(olc::Decal* sprite, std::string name);   //         	bool Create( std::string fileData, olc::Decal* sprite, std::string name);

	void CreateNew(std::string Width, std::string Heigh);

	int GetIndex(int x, int y);                      // get index from cellsmap

	int GetIndexFirstLayer(int x, int y);
	int GetIndexZeroLayer(int x, int y);
	int GetIndexSecondLayer(int x, int y);
	int GetIndexThirdLayer(int x, int y);
	int GetIndexDynamicLayer(int x, int y);

	int GetColliziionIndex(int x, int y);



	void SetIndexZeroLayer(int x, int y, int number);
	void SetIndex(int x, int y, int texnumber);  //set index first layer
	void SetIndexSecondLayer(int x, int y, int texnumber);  //set index first layer
	void SetIndexThirdLayer(int x, int y, int texnumber);  //set index first layer
	void SetCollisionIndex(int x, int y, int texnumber);
	void SetDynamicLayer(int x, int y, int texnumber);


	void DeleteLayer(int layer);


	bool LoadMap();
	void SaveMap();
};


class cMap_Grid : public cMap
{
public:
	cMap_Grid();
	

	//bool PopulateDynamics(vector<cDynamic*>& vecDyns) override;
	//bool OnInteraction(vector<cDynamic*>& vecDynobs, cDynamic* target, NATURE nature) override;
};
