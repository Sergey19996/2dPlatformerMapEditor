#include "Maps.h"
#include "Assets.h"
#include <fstream>
cMap::cMap()
{
	pSprite = nullptr;
	nWidth = 0;
	nHeight = 0;
	m_indices = nullptr;
	m_firstlayer = nullptr;
	m_secondlayer = nullptr;
	m_thirdlayer = nullptr;
	m_DynamicLayer = nullptr;
}

cMap::~cMap()
{
	delete[] m_indices;
}

int cMap::GetIndex(int x, int y)       //just for cells 
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return m_indices[y * nWidth + x];
	else
		return 0;


}

int cMap::GetIndexFirstLayer(int x, int y)       //just for cells 
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return m_firstlayer[y * nWidth + x];
	else
		return 0;


}

int cMap::GetIndexZeroLayer(int x, int y)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return m_zerolayer[y * nWidth + x];
	else
		return 0;
}

int cMap::GetIndexSecondLayer(int x, int y)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return m_secondlayer[y * nWidth + x];
	else
		return 0;

	return 0;
}

int cMap::GetIndexThirdLayer(int x, int y)
{

	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return m_thirdlayer[y * nWidth + x];
	else
		return 0;
	return 0;
}

int cMap::GetIndexDynamicLayer(int x, int y)
{

	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return m_DynamicLayer[y * nWidth + x];
	else
		return 0;
	return 0;
}

int cMap::GetColliziionIndex(int x, int y)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)
		return m_Collision[y * nWidth + x];
	else
		return 0;
}
void cMap::SetIndexZeroLayer(int x, int y, int number)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)

		m_zerolayer[y * nWidth + x] = number;

}
void cMap::SetIndex(int x, int y, int number)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)

		m_firstlayer[y * nWidth + x] = number;

}
void cMap::SetIndexSecondLayer(int x, int y, int texnumber)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)

		m_secondlayer[y * nWidth + x] = texnumber;
}
void cMap::SetIndexThirdLayer(int x, int y, int texnumber)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)

		m_thirdlayer[y * nWidth + x] = texnumber;
}





void cMap::SetCollisionIndex(int x, int y, int number)
{
	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)

		m_Collision[y * nWidth + x] = number;

}
void cMap::SetDynamicLayer(int x, int y, int texnumber)
{

	if (x >= 0 && x < nWidth && y >= 0 && y < nHeight)

		m_DynamicLayer[y * nWidth + x] = texnumber;

}
void cMap::DeleteLayer(int layer)
{

	switch (layer)
	{

	case 0:
		delete[] m_zerolayer;
		m_zerolayer = new int[nWidth * nHeight];
		for (int i = 0; i < nWidth * nHeight; i++)
		{

			m_zerolayer[i] = 0;                               //create default map for first layer 

		}
		break;
	case 1:
		delete[] m_firstlayer;
		m_firstlayer = new int[nWidth * nHeight];
		for (int i = 0; i < nWidth * nHeight; i++)
		{

			m_firstlayer[i] = 0;                               //create default map for first layer 

		}




		break;
	case 2:
		delete[] m_secondlayer;
		m_secondlayer = new int[nWidth * nHeight];
		for (int i = 0; i < nWidth * nHeight; i++)
		{

			m_secondlayer[i] = 0;                               //create default map for first layer 

		}

		break;
	case 3:
		delete[] m_thirdlayer;
		m_thirdlayer = new int[nWidth * nHeight];
		for (int i = 0; i < nWidth * nHeight; i++)
		{

			m_thirdlayer[i] = 0;                               //create default map for first layer 

		}

		break;
	case 4:
		delete[] m_DynamicLayer;
		m_DynamicLayer = new int[nWidth * nHeight];
		for (int i = 0; i < nWidth * nHeight; i++)
		{

			m_DynamicLayer[i] = 0;                               //create default map for first layer 

		}

		break;
	}



}
bool cMap::LoadMap()
{
	std::ifstream data;
	data.open("save/LevelMap.txt");

	if (data.is_open())
	{
		
		

		data >> nWidth;
		data >> nHeight;
		
		for (int i = 0; i < nWidth * nHeight; i++)
		{
			
			data >> m_Collision[i];
			data >> m_firstlayer[i];
			data >> m_secondlayer[i];
			data >> m_thirdlayer[i];
			

		}
		for (int i = 0; i < nWidth * nHeight; i++) // becouse we allready did few locations... neew use other one loop run
		{
		data >> m_DynamicLayer[i];
		data >> m_zerolayer[i];



		}


		std::cout << "Load succeed"<< std::endl;
		data.close();
		return true;
	}
	data.close();
	return false;


}
void cMap::SaveMap()
{
	std::ofstream data;
	data.open("save/LevelMap.txt");
	if (data.is_open())
	{
		data << "\n";
		data << nWidth;
		data << "\t";
		data << nHeight;
		data << "\n";
		for (int i = 0; i < nWidth * nHeight; i++)
		{
			

			data << m_Collision[i];
			data << " ";
			data << m_firstlayer[i];
			data << " ";
			data << m_secondlayer[i];
			data << " ";
			data << m_thirdlayer[i];
			data << "\n";
		}

		for (int i = 0; i < nWidth * nHeight; i++)
		{
			data << m_DynamicLayer[i];
			data << " ";
			data << m_zerolayer[i];


			data << "\n";
		}

	 std::cout << "save Succeed" << std::endl;
	
	}
	else
	{
		std::cout << "save Denied" << std::endl;
	}
	data.close();

}
bool cMap::Create(olc::Decal* sprite, std::string name)
{

	nWidth = 256;
	nHeight = 64;
	pSprite = sprite;
	sName = name;

	m_indices = new int[nWidth * nHeight];

	m_zerolayer = new int[nWidth * nHeight];
	m_firstlayer = new int[nWidth * nHeight];
	m_secondlayer = new int[nWidth * nHeight];
	m_thirdlayer = new int[nWidth * nHeight];
	m_DynamicLayer = new int[nWidth * nHeight];

	m_Collision = new int[nWidth * nHeight];


	for (int i = 0; i < nWidth*nHeight; i++)
	{
		m_indices[i] = 1;          // Fill cells 
		
		m_firstlayer[i] = 0;                               //create default map for first layer 
		m_zerolayer[i] = 0;
		m_DynamicLayer[i] = 0;   //fill third layer                              //create default map fordynamic layer 
		m_secondlayer[i] = 0;   //fill second layer                              //create default map for first layer 
		m_thirdlayer[i] = 0;   //fill third layer                              //create default map for first layer 
		m_Collision[i] = 0;   //fill collision                              //create default map for first layer 


	}

	

	return true;
}

void cMap::CreateNew(std::string Width, std::string Heigh)
{
	delete[] m_indices, m_firstlayer,m_zerolayer, m_secondlayer, m_thirdlayer, m_Collision, m_DynamicLayer;

	nWidth =stoi(Width);
	nHeight= stoi(Heigh);


	m_indices = new int[nWidth * nHeight];

	m_zerolayer = new int[nWidth * nHeight];
	m_firstlayer = new int[nWidth * nHeight];
	m_secondlayer = new int[nWidth * nHeight];
	m_thirdlayer = new int[nWidth * nHeight];
	m_DynamicLayer = new int[nWidth * nHeight];

	m_Collision = new int[nWidth * nHeight];


	for (int i = 0; i < nWidth * nHeight; i++)
	{
		m_indices[i] = 1;          // Fill cells 
		m_zerolayer[i] = 0;
		m_firstlayer[i] = 0;                               //create default map for first layer 
		m_Collision[i] = 0;   //fill collision                              //create default map for first layer 
		m_secondlayer[i] = 0;   //fill second layer                              //create default map for first layer 
		m_thirdlayer[i] = 0;   //fill third layer                              //create default map for first layer 
		m_DynamicLayer[i] = 0;   //fill third layer                              //create default map fordynamic layer 

	}

}

cMap_Grid::cMap_Grid()
{
	Create(RPG_Assets::get().GetSprite("GridSquere"), "Grids");


}
