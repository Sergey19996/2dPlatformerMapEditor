#include "Map_Editor.h"
#include <fstream>


Map_Editor::Map_Editor()
{
	sAppName = "Pantrir MapEditor";
}

void Map_Editor::WorldToScreen(float fWorldX, float fWorldY, int& nScreenX, int& nScreenY)
{
	nScreenX = (int)((fWorldX - fOffsetX) * fScaleX);
	nScreenY = (int)((fWorldY - fOffsetY) * fScaleY);

}

void Map_Editor::ScreenToWorld(int nScreenX, int nScreenY, float& fWorldX, float& fWorldY)
{
	fWorldX = ((float)nScreenX / fScaleX) + fOffsetX;
	fWorldY = ((float)nScreenY / fScaleY) + fOffsetY;

}

void Map_Editor::DescribeRect()
{

	if (fEnSelRectX < 0 && fEnSelRectY < 0)
	{
		float helpX, helpY;

		helpX = fStSelRectX + fEnSelRectX;
		helpY = fStSelRectY + fEnSelRectY;



		fEnSelRectY = abs(fEnSelRectY);
		fEnSelRectX = abs(fEnSelRectX);

		fStSelRectX = helpX;
		fStSelRectY = helpY;
	}

	if (fEnSelRectX < 0)
	{
		float helpX;

		helpX = fStSelRectX + fEnSelRectX;




		fEnSelRectY = abs(fEnSelRectY);
		fEnSelRectX = abs(fEnSelRectX);

		fStSelRectX = helpX;

	}

	if (fEnSelRectY < 0)
	{
		float  helpY;


		helpY = fStSelRectY + fEnSelRectY;



		fEnSelRectY = abs(fEnSelRectY);
		fEnSelRectX = abs(fEnSelRectX);


		fStSelRectY = helpY;
	}

	saveenrecX = fEnSelRectX;
	saveenrecY = fEnSelRectY;


	savestrecX = fStSelRectX;
	savestrecY = fStSelRectY;


	fStSelRectX = 0;
	fStSelRectY = 0;
	fEnSelRectX = 0;
	fEnSelRectY = 0;
}

bool Map_Editor::OnUserCreate()
{



	// Initialise offset so 0,0 in world space is middle of the screen
	
	
	




	RPG_Assets::get().LoadSprites();               //Load sprites
	RPG_Assets::get().LoadMaps();

	cCell::g_engine = this;
	currentCell = nullptr;

	m_pCurrentMap = RPG_Assets::get().GetMap("Grids");
	m_StaticLevel = RPG_Assets::get().GetSprite("Lvl1Constructor");
	DynamicLayer = RPG_Assets::get().GetSprite("DynamicLayer");
	fCameraPosX = 1;
	fCameraPosY = 1;


	// Draw rectangles map

	nLayerBackground = CreateLayer(); //1
	nLayerZero = CreateLayer(); //0
	//SetDrawTarget(nLayerZero);
	/*Clear({ 77,95,74 });
	SetPixelMode(olc::Pixel::ALPHA);
	DrawRect(0, 0, ScreenWidth(), ScreenHeight(), { 77,95,74 });
	SetPixelMode(olc::Pixel::NORMAL);
	EnableLayer(nLayerZero, true);
	SetDrawTarget(nullptr);*/


	//nLayerOne = CreateLayer();
	//nLayerTwo = CreateLayer();

	return true;
}

bool Map_Editor::OnUserUpdate(float fElapsedTime)
{
	animsprite += fElapsedTime;

		//m_vecCells.erase(                              //check m_vecProjectiles on flag -bredundand and erase it
		//remove_if(m_vecCells.begin(), m_vecCells.end(),
		//	[](const cCell* d) 
		//	{
		//		return((cCell*)d)->bchange; 

		//	}), m_vecCells.end());





	
	inEditmode = (GetKey(olc::Key::TAB).bHeld);

	if (GetKey(olc::Key::TAB).bHeld)
	{			
		
		int Staticwidth = m_StaticLevel->sprite->width;
		int Staticheigh = m_StaticLevel->sprite->height;


		Clear(olc::BLANK);
		
		//FillRect(0, 0, (2048 / 100) * 32, (2048 / 100) * 32, { 35,71,69,255 / 3 });

		for (float x = 1; x < (Staticwidth / 64)+1 ; x++)
		{


			DrawLine(x * 64*0.3f, 64 * 0.3f, x * 64 * 0.3f, (2048+64)*0.3f, { 255,255,255,255 / 3 });


		}
		for (float y = 1; y < (Staticheigh / 64)+1; y++)
		{



			DrawLine(64 * 0.3f, y * 64 * 0.3f, (2048 + 64) *0.3f, y * 64 * 0.3f, { 255,255,255,255 / 3 });


		}


		DrawStaticFrame(64,64, Staticwidth+64, Staticheigh+64,0.3f); // frame for static texture

		DrawStaticFrame(35*64 , 4*64, 46 * 64, 5*64, 0.3f);   // frame for collision
		
		DrawStaticFrame(1*64, 35*64, 33*64, 39*64, 0.3f);  // frame for dynamic

		



		DrawString(0, 0, "Static");
		DrawString(35*64*0.3f, 3*64*0.3f, "Collision");
		DrawString(10*64*0.3f, 34 * 64*0.3f, "Dynamic");

		fMX = (float)GetMouseX() / (64*0.3f);
		fMY = (float)GetMouseY() / (64 * 0.3f);

		if (GetMouse(0).bPressed)

		{

			

			 fSelectedCellX = (int)fMX + 0.5f;
			 fSelectedCellY = (int)fMY + 0.5f;

			 fStSelRectX = (int)fMX + 0.5f;
			 fStSelRectY = (int)fMY + 0.5f;

		}


	
	
		if (GetMouse(0).bHeld)

		{
			fEnSelRectX = (int)fMX - fStSelRectX + 2;
			fEnSelRectY = (int)fMY - fStSelRectY + 2;


		}
		if (GetMouse(0).bReleased)
		{
			DescribeRect();

			CheckGrabbedObject(fSelectedCellX, fSelectedCellY);

		
		}
		

		//Collision variations
		DrawRect((35+0.3) * 19.2, (4+0.3) * 19.2, 19.2f/2, 19.2f/2, olc::RED);       //hard collision
		DrawTriangle((37 + 0.3) * 19.2, (4 + 0.7) * 19.2, (38 - 0.3) * 19.2, (4 + 0.7) * 19.2, (38 - 0.3) * 19.2, (4 + 0.3) * 19.2,  olc::RED);  //obsticle left
		DrawTriangle((39 + 0.3) * 19.2, (4 + 0.7) * 19.2, (40 - 0.3) * 19.2, (4 + 0.7) * 19.2, (39 + 0.3) * 19.2, (4 + 0.3) * 19.2, olc::RED);  //obsticle right

		DrawLine((41+0.5)*19.2, (4+0.3)*19.2, (41 + 0.5) * 19.2, (4 + 0.7) * 19.2, olc::RED);


		//collision spikes vertical
		DrawLine((43 + 0.1) * 19.2, (4 + 0.9) * 19.2, (43 + 0.9) * 19.2, (4 + 0.9) * 19.2, olc::RED);
		DrawLine((43 + 0.2) * 19.2, (4 + 0.2) * 19.2, (43 + 0.2) * 19.2, (4 + 0.9) * 19.2, olc::RED); 
		DrawLine((43 + 0.5) * 19.2, (4 + 0.2) * 19.2, (43 + 0.5) * 19.2, (4 + 0.9) * 19.2, olc::RED);
		DrawLine((43 + 0.8) * 19.2, (4 + 0.2) * 19.2, (43 + 0.8) * 19.2, (4 + 0.9) * 19.2, olc::RED);
		//collision pikes vertical


		//teleport
		DrawLine((45 + 0.2) * 19.2, (4 + 0.1) * 19.2, (45 + 0.8) * 19.2, (4 + 0.1) * 19.2, olc::RED);
		DrawLine((45 + 0.2) * 19.2, (4 + 0.1) * 19.2, (45 + 0.2) * 19.2, (4 + 0.9) * 19.2, olc::RED);
		DrawLine((45 + 0.8) * 19.2, (4 + 0.1) * 19.2, (45 + 0.8) * 19.2, (4 + 0.9) * 19.2, olc::RED);


		DrawRect((int)fSelectedCellX * 19.2, (int)fSelectedCellY * 19.2, 19.2f, 19.2f);

		DrawRect((int)fSelectedCellX * 19.2, (int)fSelectedCellY * 19.2, (fEnSelRectX - 0.5) * 19.2, (fEnSelRectY - 0.5) * 19.2, olc::RED);

		


		//SetDrawTarget(nLayerBackground);
	//	SetPixelMode(olc::Pixel::ALPHA);

		DrawPartialDecal({ 64*0.3f  ,64 * 0.3f }, m_StaticLevel, { (float)0,(float)0 }, { (float)2048,(float)2048 }, { 0.3,0.3 });


		DrawDynamicLayer(1*64, 35 * 64,0.3f, fElapsedTime);   // Here we drawing dynamic in tab


		for (int i = 0; i < 8; i++)
		{
			DrawLine(1 * 19.2 + (i * (256 * 0.3)), 35 * 64*0.3f, 1 * 19.2 + (i * (256 * 0.3)), 39 * 64*0.3f);
		}

		//DrawPartialDecal({2048*0.3f  ,0 }, RPG_Assets::get().GetSprite("DynamicLayer"), { (float)0,(float)0 }, { (float)256,(float)2048 }, { 0.3,0.3 });

	//	SetPixelMode(olc::Pixel::NORMAL);
	//	EnableLayer(nLayerBackground, true);
	//	SetDrawTarget(nullptr);


		switch (CollNumber)  // here we draw when moving mouse. near mouse
		{
		case 0:
					//represent extract mause area
		DrawPartialDecal({ (fMX - 0.5f ) * (float)19.2  ,(fMY - 0.5f ) * (float)19.2 }, m_StaticLevel, { (int)savestrecX * (float)64-64,(int)savestrecY * (float)64-64 }, { (float)nTileWidth * (int)saveenrecX,(float)nTileHeight * (int)saveenrecY }, { 0.5,0.5 });
			break;
		case 1:
			DrawRect((fMX + 0.3) * 19.2, (fMY + 0.3 + 1) * 19.2, 19.2f / 2, 19.2f / 2, olc::RED);       //hard collision
			break;
		case 2:
			DrawTriangle((fMX + 0.3) * 19.2, (fMY + 0.7 + 1) * 19.2, (fMX - 0.3) * 19.2, (fMY + 0.7 + 1) * 19.2, (fMX - 0.3) * 19.2, (fMY + 0.3 + 1) * 19.2, olc::RED);  //obsticle left
			break;
		case 3:
			DrawTriangle((fMX + 0.3) * 19.2, (fMY + 0.7+1) * 19.2, (fMX - 0.3) * 19.2, (fMY + 0.7 + 1) * 19.2, (fMX + 0.3) * 19.2, (fMY + 0.3 + 1) * 19.2, olc::RED);  //obsticle right
			break;
		case 4:
			DrawLine((fMX + 0.5) * 19.2, (fMY + 0.3+1) * 19.2, (fMX + 0.5) * 19.2, (fMY + 0.7+1) * 19.2, olc::RED);
			break;
		case 5:
			DrawLine((fMX + 0.1) * 19.2, (fMY + 0.9 + 1) * 19.2, (fMX + 0.9) * 19.2, (fMY + 0.9 + 1) * 19.2, olc::RED);
			DrawLine((fMX + 0.2) * 19.2, (fMY + 0.2 + 1) * 19.2, (fMX + 0.2) * 19.2, (fMY + 0.9 + 1) * 19.2, olc::RED);
			DrawLine((fMX + 0.5) * 19.2, (fMY + 0.2 + 1) * 19.2, (fMX + 0.5) * 19.2, (fMY + 0.9 + 1) * 19.2, olc::RED);
			DrawLine((fMX + 0.8) * 19.2, (fMY + 0.2 + 1) * 19.2, (fMX + 0.8) * 19.2, (fMY + 0.9 + 1) * 19.2, olc::RED);

			break;

		case 6:
			DrawLine((fMX + 0.2) * 19.2, (fMY + 0.1 + 1) * 19.2, (fMX + 0.8) * 19.2, (fMY + 0.1 + 1) * 19.2, olc::RED);
			DrawLine((fMX + 0.2) * 19.2, (fMY + 0.1 + 1) * 19.2, (fMX + 0.2) * 19.2, (fMY + 0.9 + 1) * 19.2, olc::RED);
			DrawLine((fMX + 0.8) * 19.2, (fMY + 0.1 + 1) * 19.2, (fMX + 0.8) * 19.2, (fMY + 0.9 + 1) * 19.2, olc::RED);

			break;
			//Dynamic Drawing near ,ouse
		case 10:
			DrawPartialDecal({ (fMX - 0.5f) * (float)19.2  ,(fMY - 0.5f) * (float)19.2 },DynamicLayer, { 0,0 }, { 256,256 }, { 0.5,0.5 });
			break;
		case 11:
			DrawPartialDecal({ (fMX - 0.5f) * (float)19.2  ,(fMY - 0.5f) * (float)19.2 }, DynamicLayer, { 0,1*256 }, { 256,256 }, { 0.5,0.5 });
			break;
		case 12:
			DrawPartialDecal({ (fMX - 0.5f) * (float)19.2  ,(fMY - 0.5f) * (float)19.2 }, DynamicLayer, { 0,2 * 256 }, { 256,256 }, { 0.5,0.5 });
			break;
		case 13:
			DrawPartialDecal({ (fMX - 0.5f) * (float)19.2  ,(fMY - 0.5f) * (float)19.2 }, DynamicLayer, { 0,3 * 256 }, { 256,256 }, { 0.5,0.5 });
			break;
		case 14:
			DrawPartialDecal({ (fMX - 0.5f) * (float)19.2  ,(fMY - 0.5f) * (float)19.2 }, DynamicLayer, { 0,4 * 256 }, { 256,256 }, { 0.5,0.5 });
			break;
		case 15:
			DrawPartialDecal({ (fMX - 0.5f) * (float)19.2  ,(fMY - 0.5f) * (float)19.2 }, DynamicLayer, { 0,5 * 256 }, { 256,256 }, { 0.5,0.5 });
			break;
		case 16:
			DrawPartialDecal({ (fMX - 0.5f) * (float)19.2  ,(fMY - 0.5f) * (float)19.2 }, DynamicLayer, { 0,6 * 256 }, { 256,256 }, { 0.5,0.5 });
			break;
		case 17:
			DrawPartialDecal({ (fMX - 0.5f) * (float)19.2  ,(fMY - 0.5f) * (float)19.2 }, DynamicLayer, { 0,7 * 256 }, { 256,256 }, { 0.5,0.5 });
			break;



		}

		
		
		//DrawPartialDecal({ 0  ,0 }, RPG_Assets::get().GetSprite("CellConstructor"), { (float)0,(float)0 }, { (float)2048,(float)2048 }, { 0.3,0.3 });

		return true;
	}

	if (!inEditmode)
	{
		// Just grab a copy of mouse coordinates for convenience
		float fMouseX = (float)GetMouseX() / 64 - fOffsetX;
		float fMouseY = (float)GetMouseY() / 64 - fOffsetY;
		float fmouseWX = (float)GetMouseX() / 64 + fOffsetX;
		float fmouseWY = (float)GetMouseY() / 64 + fOffsetY;

		// For panning, we need to capture the screen location when the user starts
		// to pan...
		if (GetMouse(0).bPressed)
		{
			fStartPanX = fMouseX;
			fStartPanY = fMouseY;


			//DrawRect(10, 675, 24, 74); // Frame for Zero layer
			if (GetMouseX() >= 10 && GetMouseY() >= 680 && GetMouseX() <= 10 + 24 && GetMouseY() <= 680 + 64)    //describe Zero active layer
			{
				currLayer = 0;
				std::cout << bseeThird << std::endl;
			};
			if (GetMouseX() >= 60 && GetMouseY() >= 680 && GetMouseX() <= 60 + 74 && GetMouseY() <= 680 + 64)    //describe first active layer
			{
				currLayer = 1;
				std::cout << bseeThird << std::endl;
			};
			if (GetMouseX() >= 154 && GetMouseY() >= 680 && GetMouseX() <= 154 + 74 && GetMouseY() <= 680 + 64)    //describe Second active layer
			{
				currLayer = 2;
			};
			if (GetMouseX() >= 243 && GetMouseY() >= 680 && GetMouseX() <= 243 + 74 && GetMouseY() <= 680 + 64)    //describe thirt active layer
			{
				currLayer = 3;
			};

			if (GetMouseX() >= 450 && GetMouseY() >= 680 && GetMouseX() <= 450 + 54 && GetMouseY() <= 680 + 64)    //describe four active layer
			{
				currLayer = 4;
			};


			//DrawCircle(22, 660, 12);   // circle when we off him
			if (GetMouseX() >= 22 - 12 && GetMouseY() >= 660 - 12 && GetMouseX() <= 22 + 12 && GetMouseY() <= 660 + 12)    //describe Zero hide point
			{
				bseeZero = !bseeZero;
			};
			if (GetMouseX() >= 92 - 12 && GetMouseY() >= 660 - 12 && GetMouseX() <= 92 + 12 && GetMouseY() <= 660 + 12)    //describe first hide point
			{
				bseeFirst = !bseeFirst;
			};

			if (GetMouseX() >= 186 - 12 && GetMouseY() >= 660 - 12 && GetMouseX() <= 186 + 12 && GetMouseY() <= 660 + 12)    //describe second hide point
			{
				bseeSecond = !bseeSecond;
			};
			if (GetMouseX() >= 280 - 12 && GetMouseY() >= 660 - 12 && GetMouseX() <= 280 + 12 && GetMouseY() <= 660 + 12)    //describe third hide point
			{
				bseeThird = !bseeThird;
			};
			if (GetMouseX() >= 477 - 12 && GetMouseY() >= 660 - 12 && GetMouseX() <= 477 + 12 && GetMouseY() <= 660 + 12)    //describe third hide point
			{
				bseeDynamic = !bseeDynamic;

			};


			FillRect(22, 675, 12, 12, olc::RED); // for deleting full layer Zero


			if (GetMouseX() >= 22 && GetMouseY() >= 675 && GetMouseX() <= 34 && GetMouseY() <= 675 + 12)    //describe Delete first active layer
				m_pCurrentMap->DeleteLayer(0);
			if (GetMouseX() >= 129 - 24 && GetMouseY() >= 675 && GetMouseX() <= 129 - 24+24 && GetMouseY() <= 675 + 24)    //describe Delete first active layer
				m_pCurrentMap->DeleteLayer(1);
			if (GetMouseX() >= 223 - 24 && GetMouseY() >= 675 && GetMouseX() <= 223 - 24 + 24 && GetMouseY() <= 675 + 24)    //describe Delete second active layer
				m_pCurrentMap->DeleteLayer(2);
			if (GetMouseX() >= 317 - 24 && GetMouseY() >= 675 && GetMouseX() <= 317 - 24 + 24 && GetMouseY() <= 675 + 24)    //describe third active layer
				m_pCurrentMap->DeleteLayer(3);

			

			
		

			if (GetMouseX() >= 504 - 24 && GetMouseY() >= 675 && GetMouseX() <= 504 - 24 + 24 && GetMouseY() <= 675 + 24)    //describe third active layer
				m_pCurrentMap->DeleteLayer(4);



		
			






			//Describe Load files
			if (GetMouseX() >= 360-5 && GetMouseY() >= 660-5 && GetMouseX() <= 430 - 5 && GetMouseY() <= 660-5+25)    //describe third hide point
			{
				m_pCurrentMap->LoadMap();
			}

			//Describe Save files
			if (GetMouseX() >= 360 - 5 && GetMouseY() >= 720 - 5 && GetMouseX() <= 430 - 5 && GetMouseY() <= 720 - 5 + 25)    //describe third hide point
			{
				m_pCurrentMap->SaveMap();
				//std::cout << m_pCurrentMap->SaveMap() << std::endl;
			}


			if (GetMouseX() >= ScreenWidth() - 132 && GetMouseY() >= ScreenHeight() - 60 && GetMouseX() <= ScreenWidth() - 30 && GetMouseY() <= ScreenHeight() - 20)    //describe Create LEVEL
			{

				m_pCurrentMap->CreateNew(SWidth, SHeight);
			}


			if (GetMouseX()>= ScreenWidth() - 140 && GetMouseY() >= ScreenHeight()-100 && GetMouseX() <= ScreenWidth()-86 && GetMouseY() <= ScreenHeight() -68)    //describe Create LEVEL Width
			{
				TextEntryEnable(true);


				bWriteWidth = true;

				std::cout << "Yooo1" << std::endl;

			//	std::cin >> SWidth;
			}
		
			if (GetMouseX() >= ScreenWidth() - 85 && GetMouseY() >= ScreenHeight() - 100 && GetMouseX() <= ScreenWidth() - 53 && GetMouseY() <= ScreenHeight() - 68)    //describe Create LEVEL Height
			{
				TextEntryEnable(true);
				std::cout << "Yooo2" << std::endl;

				
				bWriteWidth = false;

			//	std::cin >> SHeight;
			

			}

		}



		if (GetMouse(0).bHeld)          //Left Mouse                                        
		{
			fCameraPosX += (fMouseX - fStartPanX);
			fCameraPosY += (fMouseY - fStartPanY);

			//	// Start "new" pan for next epoch
			//	fStartPanX = fMouseX;
				//fStartPanY = fMouseY;

			



		}
		

		if (GetMouse(1).bPressed)       //Right mouse
		{
			fSelectedCellX = (int)fmouseWX + 0.5f;
			fSelectedCellY = (int)fmouseWY + 0.5f;



		}

		if (GetMouse(2).bPressed)          // Midlle mouse
		{


			fStSelRectX = (int)fmouseWX;
			fStSelRectY = (int)fmouseWY;
			
			
			saveenrecX = 1;
			saveenrecY = 1;
			

		}


		if (GetKey(olc::Key::SHIFT).bHeld)
		{
			addspeed = 10.0f;

		}
		if (GetKey(olc::Key::SHIFT).bReleased)
		{
			addspeed = 1.0f;

		}
		if (GetKey(olc::Key::ESCAPE).bReleased)
		{
			return false;
		}

		// ...change the scale as required...
		if (GetKey(olc::Key::Z).bHeld)
		{

			fixfScaleX *= 1.001f;
			fixfSacelY *= 1.001f;

			if ((int)fixfScaleX>0)
			{
			fScaleX = (int)fixfScaleX;
			fScaleY = (int)fixfSacelY;

			}
			else
			{
				fScaleX = fixfScaleX;
				fScaleY = fixfSacelY;
			}


		

		}

		if (GetKey(olc::Key::A).bHeld)
		{

			fCameraPosX -= 5 * fElapsedTime*addspeed;

		}


		

		if (GetKey(olc::Key::W).bHeld)
		{
			fCameraPosY -= 5 * fElapsedTime * addspeed;

		}
		if (GetKey(olc::Key::S).bHeld)
		{
			fCameraPosY += 5 * fElapsedTime * addspeed;

		}
		if (GetKey(olc::Key::D).bHeld)
		{
			fCameraPosX += 5 * fElapsedTime * addspeed;

		}

		if (GetKey(olc::Key::E).bHeld)
		{
			

			switch (currLayer)
			{

			case 0:

				m_pCurrentMap->SetIndexZeroLayer(fSelectedCellX, fSelectedCellY, 0);
				break;

			case 1:
				
			m_pCurrentMap->SetIndex(fSelectedCellX, fSelectedCellY, 0);
			break;
			
			case 2:
				m_pCurrentMap->SetIndexSecondLayer(fSelectedCellX, fSelectedCellY, 0);
				break;

			case 3:
				m_pCurrentMap->SetIndexThirdLayer(fSelectedCellX, fSelectedCellY, 0);
				break;


			case 4:
				for (int X =-2; X <2; X++)
				{
					for (int Y = -2; Y < 2; Y++)
					{


						m_pCurrentMap->SetDynamicLayer(fSelectedCellX+X, fSelectedCellY+Y, 0);


					}

				}

			}




			m_pCurrentMap->SetCollisionIndex(fSelectedCellX, fSelectedCellY, 0);


		}



		if (GetKey(olc::Key::X).bHeld)
		{
			fixfScaleX *= 0.999f;
			fixfSacelY *= 0.999f;



			if ((int)fixfScaleX > 0)
			{
				fScaleX = (int)fixfScaleX;
				fScaleY = (int)fixfSacelY;

			}
			else
			{
				fScaleX = fixfScaleX;
				fScaleY = fixfSacelY;
			}

		}
		if (GetKey(olc::Key::SPACE).bHeld)
		{
			/*for (auto& source : {&m_vecCells})
			{
				for (auto& object : * source)
				{
					object->px

				}
			}*/


			//cCell* g1 = new cSprite(fSelectedCellX - 0.5f, fSelectedCellY - 0.5f, (int)savestrecX * (float)64, (int)savestrecY * (float)64, (float)nTileWidth * (int)saveenrecX, (float)nTileHeight * (int)saveenrecY,1, RPG_Assets::get().GetSprite("Lvl1Constructor"));
		//	m_vecCells.push_back(g1);




			if (CollNumber == 0)    // Coll number - number which we selected in TAB collizion. 
			{


				switch (currLayer)
				{
				case 0:
					for (int y = 0; y < (int)saveenrecY; y++)    // for cycle describing rectangle
					{
						for (int x = 0; x < (int)saveenrecX; x++)
						{

							int number = ((int)savestrecY + y - 1) * 32 + ((int)savestrecX + x - 1);  // minus -1  we compinsate offset in tab drawing. we offset texture in 1x and 1 y in tab.
							// now we compinsate it 
							int fixX = fSelectedCellX + x;
							int fixY = fSelectedCellY + y;
							//									y * width + x   32 IT'S AMOUNT cells  OF 64/64 IN 2048    
							m_pCurrentMap->SetIndexZeroLayer(fixX, fixY, number);


						}
					}

					break;
				case 1:
					for (int y = 0; y < (int)saveenrecY; y++)    // for cycle describing rectangle
					{
						for (int x = 0; x < (int)saveenrecX; x++)
						{

							int number = ((int)savestrecY + y-1) * 32 + ((int)savestrecX + x-1);  // minus -1  we compinsate offset in tab drawing. we offset texture in 1x and 1 y in tab.
																								  // now we compinsate it 
							int fixX = fSelectedCellX + x;
							int fixY = fSelectedCellY + y;
							//									y * width + x   32 IT'S AMOUNT cells  OF 64/64 IN 2048    
							m_pCurrentMap->SetIndex(fixX, fixY, number);


						}
					}

					break;
				case 2:
					for (int y = 0; y < (int)saveenrecY; y++)
					{
						for (int x = 0; x < (int)saveenrecX; x++)
						{

							int number = ((int)savestrecY + y-1) * 32 + ((int)savestrecX + x-1);
							int fixX = fSelectedCellX + x;
							int fixY = fSelectedCellY + y;
							//									y * width + x   32 IT'S AMOUNT OF 64/64 IN 2048    
							m_pCurrentMap->SetIndexSecondLayer(fixX, fixY, number);


						}
					}
					break;
				case 3:
					for (int y = 0; y < (int)saveenrecY; y++)
					{
						for (int x = 0; x < (int)saveenrecX; x++)
						{

							int number = ((int)savestrecY + y-1) * 32 + ((int)savestrecX + x-1);
							int fixX = fSelectedCellX + x;
							int fixY = fSelectedCellY + y;
							//									y * width + x   32 IT'S AMOUNT OF 64/64 IN 2048    
							m_pCurrentMap->SetIndexThirdLayer(fixX, fixY, number);


						}
					}
					break;
				}

				
			}
			if (CollNumber > 0 && CollNumber <7)
			{
				  
						m_pCurrentMap->SetCollisionIndex(fSelectedCellX, fSelectedCellY, CollNumber);   // set collision draw

			}
			if (CollNumber > 9 && CollNumber < 18)
			{


				m_pCurrentMap->SetDynamicLayer(fSelectedCellX, fSelectedCellY, CollNumber);   // set collision draw

			}

		}

		//Draw Level
		int nTileWidth = 64;
		int nTileHeight = 64;

		int nVisibleTileX = (ScreenWidth() / nTileWidth) / fScaleX;
		int nVisibleTileY = (ScreenHeight() / nTileHeight)/ fScaleY;

		//Calculate Top-Leftmost visible tile
		fOffsetX = fCameraPosX - (float)nVisibleTileX / 2.0f;
		fOffsetY = fCameraPosY - (float)nVisibleTileY / 2.0f;


		//Clamp camera to game  boundaries
		if (fOffsetX < 0) fOffsetX = 0;
		if (fOffsetY < 0) fOffsetY = 0;
		if (fOffsetX > m_pCurrentMap->nWidth - nVisibleTileX)
		{
			fOffsetX = m_pCurrentMap->nWidth - nVisibleTileX;
		}
		if (fOffsetY > m_pCurrentMap->nHeight - nVisibleTileY)
		{
			fOffsetY = m_pCurrentMap->nHeight - nVisibleTileY;
		}
		if (fCameraPosX < 0)fCameraPosX = 0;
		if (fCameraPosY < 0) fCameraPosY = 0;
		if (fCameraPosX > m_pCurrentMap->nWidth)
		{
			fCameraPosX = m_pCurrentMap->nWidth;
		}
		if (fCameraPosY > m_pCurrentMap->nHeight)
		{
			fCameraPosY = m_pCurrentMap->nHeight;
		}




		//Get offsets for smooth movement
		float fTileOffsetX = ((fOffsetX - (int)fOffsetX) * nTileWidth) ;
		float fTileOffsetY = ((fOffsetY - (int)fOffsetY) * nTileHeight) ;


			
		Clear({ olc::BLANK });
	
		//SetDrawTarget(nLayerZero);
		
		
		//FillRect((fTileOffsetX)*fScaleX, (fTileOffsetY)*fScaleY , 74, 74);

		SetDrawTarget(nLayerBackground);
		SetPixelMode(olc::Pixel::ALPHA);
		DrawPartialDecal({ 0.0f, 0.0f }, RPG_Assets::get().GetSprite("BackGroundColor"), { 0,0 }, { (float)1980,(float)768 });
		SetPixelMode(olc::Pixel::NORMAL);
		EnableLayer(nLayerBackground, true);
		SetDrawTarget(nullptr);
		// Draw rectangles map
		for (int x = -1; x < nVisibleTileX + 1; x++)
		{
			for (int y = -1; y < nVisibleTileY + 1; y++)
			{


				int idx = m_pCurrentMap->GetIndex(x + fOffsetX, y + fOffsetY);
				int sy = idx / 10;

				
				DrawPartialDecal({ (x * 64 - fTileOffsetX) * fScaleX  ,(y * 64 - fTileOffsetY) * fScaleY }, m_pCurrentMap->pSprite, { (float)sy * 64,(float)sy * 64 }, { (float)nTileWidth,(float)nTileHeight }, { 1*fScaleX,1 *fScaleY});
				
			}

		}



		if (bseeThird == true)
		{



			for (int x = -1; x < nVisibleTileX + 1; x++)   //draw Third layer map 
			{
				for (int y = -1; y < nVisibleTileY + 1; y++)
				{


					int idx = m_pCurrentMap->GetIndexThirdLayer(x + fOffsetX, y + fOffsetY);

					if (idx!=0)
					{

					int sx = idx % 32;   // 32 cells - amount of cells in 2048/ 2048 texture - moduel give us X
					int sy = idx / 32;   // divide give's us y

					SetDrawTarget(nLayerBackground);
					SetPixelMode(olc::Pixel::ALPHA);
					DrawPartialDecal({ (x * 64 - fTileOffsetX)*fScaleX  ,(y * 64 - fTileOffsetY) * fScaleY }, m_StaticLevel, { (float)sx * 64,(float)sy * 64 }, { (float)nTileWidth,(float)nTileHeight }, { 1 * fScaleX,1 * fScaleY });
					SetPixelMode(olc::Pixel::NORMAL);
					EnableLayer(nLayerBackground, true);
					SetDrawTarget(nullptr);
					}
				}

			}
		}

		

		if (bseeSecond == true)
		{



			for (int x = -1; x < nVisibleTileX + 1; x++)   //draw Second layer map 
			{
				for (int y = -1; y < nVisibleTileY + 1; y++)
				{

					
						int idx = m_pCurrentMap->GetIndexSecondLayer(x + fOffsetX, y + fOffsetY);
						if (idx != 0)
						{
						int sx = idx % 32;   // 32 cells - amount of cells in 2048/ 2048 texture - moduel give us X
						int sy = idx / 32;   // divide give's us y

						SetDrawTarget(nLayerBackground);
						SetPixelMode(olc::Pixel::ALPHA);
						DrawPartialDecal({ (x * 64 - fTileOffsetX) * fScaleX ,(y * 64 - fTileOffsetY) * fScaleY }, m_StaticLevel, { (float)sx * 64,(float)sy * 64 }, { (float)nTileWidth,(float)nTileHeight }, { 1 * fScaleX,1 * fScaleY });
					SetPixelMode(olc::Pixel::NORMAL);
					EnableLayer(nLayerBackground, true);
					SetDrawTarget(nullptr);
					}
				}

			}





		}



		if (bseeDynamic == true)
		{

			//Dynamic Layer

			for (int x = -1; x < nVisibleTileX + 1; x++)   //draw Second layer map 
			{
				for (int y = -1; y < nVisibleTileY + 1; y++)
				{


					int idx = m_pCurrentMap->GetIndexDynamicLayer(x + fOffsetX, y + fOffsetY);

					if (idx != 0)
					{

						idx -= 10;
						int sx = idx % 8;   // 32 cells - amount of cells in 2048/ 2048 texture - moduel give us X
						int sy = idx;   // divide give's us y

						SetDrawTarget(nLayerBackground);
						SetPixelMode(olc::Pixel::ALPHA);
						DrawPartialDecal({ (x * 64 - fTileOffsetX) * fScaleX ,(y * 64 - fTileOffsetY) * fScaleY }, DynamicLayer, { (float)(frameIndex) * 256,(float)sy * 256 }, { (float)256,(float)265 }, { 1 ,1 });
						SetPixelMode(olc::Pixel::NORMAL);
						EnableLayer(nLayerBackground, true);
						SetDrawTarget(nullptr);
					}
				}

			}
		}
		//






		if (bseeFirst == true)
		{



			for (int x = -1; x < nVisibleTileX + 1; x++)   //draw first layer map 
			{
				for (int y = -1; y < nVisibleTileY + 1; y++)
				{


					int idx = m_pCurrentMap->GetIndexFirstLayer(x + fOffsetX, y + fOffsetY);
					if (idx != 0)
					{
						int sx = idx % 32;   // 32 cells - amount of cells in 2048/ 2048 texture - moduel give us X
						int sy = idx / 32;   // divide give's us y

						SetDrawTarget(nLayerBackground);
						SetPixelMode(olc::Pixel::ALPHA);
						DrawPartialDecal({ (x * 64 - fTileOffsetX) * fScaleX  ,(y * 64 - fTileOffsetY) * fScaleY }, m_StaticLevel, { (float)sx * 64,(float)sy * 64 }, { (float)nTileWidth,(float)nTileHeight }, { 1 * fScaleX,1 * fScaleY });
						SetPixelMode(olc::Pixel::NORMAL);
						EnableLayer(nLayerBackground, true);
						SetDrawTarget(nullptr);
					}
				}

			}
		}

		if (bseeZero == true)
		{



			for (int x = -1; x < nVisibleTileX + 1; x++)   //draw first layer map 
			{
				for (int y = -1; y < nVisibleTileY + 1; y++)
				{


					int idx = m_pCurrentMap->GetIndexZeroLayer(x + fOffsetX, y + fOffsetY);
					if (idx != 0)
					{
						int sx = idx % 32;   // 32 cells - amount of cells in 2048/ 2048 texture - moduel give us X
						int sy = idx / 32;   // divide give's us y

						SetDrawTarget(nLayerBackground);
						SetPixelMode(olc::Pixel::ALPHA);
						DrawPartialDecal({ (x * 64 - fTileOffsetX) * fScaleX  ,(y * 64 - fTileOffsetY) * fScaleY }, m_StaticLevel, { (float)sx * 64,(float)sy * 64 }, { (float)nTileWidth,(float)nTileHeight }, { 1 * fScaleX,1 * fScaleY });
						SetPixelMode(olc::Pixel::NORMAL);
						EnableLayer(nLayerBackground, true);
						SetDrawTarget(nullptr);
					}
				}

			}
		}

		
		for (int x = -1; x < nVisibleTileX + 1; x++)   //draw collision map
		{
			for (int y = -1; y < nVisibleTileY + 1; y++)
			{


				int idx = m_pCurrentMap->GetColliziionIndex(x + fOffsetX, y + fOffsetY);
				


				

				switch (idx)
				{
				case 1:
				DrawRect(((x + 0.3 ) * 64 - fTileOffsetX)* fScaleX, ((y + 0.3) * 64 - fTileOffsetY)* fScaleY, 19.2f / 2, 19.2f / 2, olc::RED);       //hard collision
					break;
				case 2:
				DrawTriangle(((x + 0.4 ) * 64 - fTileOffsetX)*fScaleX, ((y + 0.2 ) * 64 - fTileOffsetY) * fScaleY, ((x + 0.2 ) * 64 - fTileOffsetX) * fScaleX, ((y + 0.4 ) * 64 - fTileOffsetY) * fScaleY, ((x + 0.4 ) * 64 - fTileOffsetX) * fScaleX, ((y + 0.4) * 64 - fTileOffsetY) * fScaleY, olc::RED);  //obsticle left
					break;
				case 3:

				DrawTriangle(((x + 0.2 ) * 64 - fTileOffsetX)*fScaleX, ((y + 0.4 ) * 64 - fTileOffsetY) * fScaleY, ((x + 0.2 ) * 64 - fTileOffsetX) * fScaleX, ((y + 0.2 ) * 64 - fTileOffsetY) * fScaleY, ((x+ 0.4 ) * 64 - fTileOffsetX) * fScaleX, ((y + 0.4 ) * 64 - fTileOffsetY) * fScaleY, olc::RED);  //obsticle right

					break;
				case 4:

					DrawLine(((x + 0.35) * 64 - fTileOffsetX)* fScaleX, ((y + 0.2) * 64 - fTileOffsetY)* fScaleY, ((x + 0.35) * 64 - fTileOffsetX)* fScaleX, ((y + 0.4) * 64 - fTileOffsetY)* fScaleY, olc::RED);

					break;
				case 5:
					DrawLine(((x + 0.1 ) * 64 - fTileOffsetX) * fScaleX, ((y + 0.4 ) * 64 - fTileOffsetY) * fScaleY, ((x + 0.45 ) * 64 - fTileOffsetX) * fScaleX, ((y + 0.4 ) * 64 - fTileOffsetY) * fScaleY, olc::RED);
					DrawLine(((x + 0.14 ) * 64 - fTileOffsetX)* fScaleX, ((y + 0.4 ) * 64 - fTileOffsetY)* fScaleY, ((x + 0.14 ) * 64 - fTileOffsetX)* fScaleX, ((y + 0.2 ) * 64 - fTileOffsetY)* fScaleY, olc::RED);
					DrawLine(((x + 0.28 ) * 64 - fTileOffsetX)* fScaleX, ((y + 0.4 ) * 64 - fTileOffsetY)* fScaleY, ((x + 0.28) * 64 - fTileOffsetX)* fScaleX, ((y + 0.2) * 64 - fTileOffsetY)* fScaleY, olc::RED);
					DrawLine(((x + 0.42 ) * 64 - fTileOffsetX)* fScaleX, ((y + 0.4 ) * 64 - fTileOffsetY)* fScaleY, ((x + 0.42 ) * 64 - fTileOffsetX)* fScaleX, ((y + 0.2 ) * 64 - fTileOffsetY)* fScaleY, olc::RED);
					break;
				case 6:

					DrawLine(((x + 0.2) * 64 - fTileOffsetX)* fScaleX, ((y + 0.1 ) * 64 - fTileOffsetY)* fScaleY, ((x+ 0.4 ) * 64 - fTileOffsetX)* fScaleX, ((y + 0.1 ) * 64 - fTileOffsetY)* fScaleY, olc::RED);
					DrawLine(((x + 0.2 ) * 64 - fTileOffsetX)* fScaleX, ((y + 0.1 ) * 64 - fTileOffsetY)* fScaleY, ((x + 0.2 ) * 64 - fTileOffsetX)* fScaleX, ((y + 0.4 ) * 64 - fTileOffsetY)* fScaleY, olc::RED);
					DrawLine(((x + 0.4 ) * 64 - fTileOffsetX)* fScaleX, ((y + 0.1 ) * 64 - fTileOffsetY)* fScaleY, ((x + 0.4 ) * 64 - fTileOffsetX)* fScaleX, ((y + 0.4 ) * 64 - fTileOffsetY)* fScaleY, olc::RED);


					break;
				}


			}

		}
			
		std::cout << fSelectedCellX-0.5f << '\t' << fSelectedCellY-0.5f << std::endl;

		switch (CollNumber)    // here we drawing when click in cell out of tab mode
		{
		case 0:
			//represent extract mause area
			DrawPartialDecal({ (fSelectedCellX - 0.5f - fOffsetX) * 64 ,(fSelectedCellY - 0.5f - fOffsetY) * 64 }, m_StaticLevel, { (int)savestrecX * (float)64-64,(int)savestrecY * (float)64-64 }, { (float)nTileWidth * (int)saveenrecX,(float)nTileHeight * (int)saveenrecY }, { 1,1 });
			break;
		case 1:
			DrawRect((fSelectedCellX + 0.3 - fOffsetX) * 64, (fSelectedCellY + 0.3  - fOffsetY) * 64, 19.2f / 2, 19.2f / 2, olc::RED);       //hard collision
			break;
		case 2:
			DrawTriangle((fSelectedCellX + 0.4 - fOffsetX) * 64, (fSelectedCellY + 0.2 - fOffsetY) * 64, (fSelectedCellX + 0.2 - fOffsetX) * 64, (fSelectedCellY + 0.4 - fOffsetY) * 64, (fSelectedCellX + 0.4 - fOffsetX) * 64, (fSelectedCellY + 0.4 - fOffsetY) * 64, olc::RED);  //obsticle left
			break;
		case 3:
			DrawTriangle((fSelectedCellX + 0.2 - fOffsetX) * 64, (fSelectedCellY + 0.4 - fOffsetY) * 64, (fSelectedCellX + 0.2 - fOffsetX) * 64, (fSelectedCellY + 0.2 - fOffsetY) * 64, (fSelectedCellX + 0.4 - fOffsetX) * 64, (fSelectedCellY + 0.4 - fOffsetY) * 64, olc::RED);  //obsticle right
			break;
		case 4:
			DrawLine((fSelectedCellX + 0.35 - fOffsetX) * 64, (fSelectedCellY + 0.2 - fOffsetY) * 64, (fSelectedCellX + 0.35 - fOffsetX) * 64, (fSelectedCellY + 0.4 - fOffsetY) * 64, olc::RED);
			break;
		case 5:
			DrawLine((fSelectedCellX + 0.1 - fOffsetX) * 64, (fSelectedCellY + 0.4 - fOffsetY) * 64, (fSelectedCellX + 0.45 - fOffsetX) * 64, (fSelectedCellY + 0.4 - fOffsetY) * 64, olc::RED);
			DrawLine((fSelectedCellX + 0.14 - fOffsetX) * 64, (fSelectedCellY + 0.4 - fOffsetY) * 64, (fSelectedCellX + 0.14 - fOffsetX) * 64, (fSelectedCellY + 0.2 - fOffsetY) * 64, olc::RED);
			DrawLine((fSelectedCellX + 0.28 - fOffsetX) * 64, (fSelectedCellY + 0.4 - fOffsetY) * 64, (fSelectedCellX + 0.28 - fOffsetX) * 64, (fSelectedCellY + 0.2 - fOffsetY) * 64, olc::RED);
			DrawLine((fSelectedCellX + 0.42 - fOffsetX) * 64, (fSelectedCellY + 0.4 - fOffsetY) * 64, (fSelectedCellX + 0.42 - fOffsetX) * 64, (fSelectedCellY + 0.2 - fOffsetY) * 64, olc::RED);

			break;

		case 6:
			DrawLine((fSelectedCellX + 0.2 - fOffsetX) * 64, (fSelectedCellY + 0.1 - fOffsetY) * 64, (fSelectedCellX + 0.4 - fOffsetX) * 64, (fSelectedCellY + 0.1 - fOffsetY) * 64, olc::RED);
			DrawLine((fSelectedCellX + 0.2 - fOffsetX) * 64, (fSelectedCellY + 0.1 - fOffsetY) * 64, (fSelectedCellX + 0.2 - fOffsetX) * 64, (fSelectedCellY + 0.4 - fOffsetY) * 64, olc::RED);
			DrawLine((fSelectedCellX + 0.4 - fOffsetX) * 64, (fSelectedCellY + 0.1 - fOffsetY) * 64, (fSelectedCellX + 0.4 - fOffsetX) * 64, (fSelectedCellY + 0.4 - fOffsetY) * 64, olc::RED);

			break;

			//Dynamic Drawing near ,ouse
		case 10:
			DrawPartialDecal({ (fSelectedCellX - 0.5f - fOffsetX) * 64  ,(fSelectedCellY - 0.5f - fOffsetY) * 64 }, DynamicLayer, { 0,0 }, { 256,256 }, { 0.5,0.5 });
			break;
		case 11:
			DrawPartialDecal({ (fSelectedCellX - 0.5f - fOffsetX) * 64  ,(fSelectedCellY - 0.5f - fOffsetY) * 64 }, DynamicLayer, { 0,1 * 256 }, { 256,256 }, { 0.5,0.5 });
			break;
		case 12:
			DrawPartialDecal({ (fSelectedCellX - 0.5f - fOffsetX) * 64  ,(fSelectedCellY - 0.5f - fOffsetY) * 64 }, DynamicLayer, { 0,2 * 256 }, { 256,256 }, { 0.5,0.5 });
			break;
		case 13:
			DrawPartialDecal({ (fSelectedCellX - 0.5f - fOffsetX) * 64  ,(fSelectedCellY - 0.5f - fOffsetY) * 64 }, DynamicLayer, { 0,3 * 256 }, { 256,256 }, { 0.5,0.5 });
			break;
		case 14:
			DrawPartialDecal({ (fSelectedCellX - 0.5f - fOffsetX) * 64  ,(fSelectedCellY - 0.5f - fOffsetY) * 64 }, DynamicLayer, { 0,4 * 256 }, { 256,256 }, { 0.5,0.5 });
			break;
		case 15:
			DrawPartialDecal({ (fSelectedCellX - 0.5f - fOffsetX) * 64  ,(fSelectedCellY - 0.5f - fOffsetY) * 64 }, DynamicLayer, { 0,5 * 256 }, { 256,256 }, { 0.5,0.5 });
			break;
		case 16:
			DrawPartialDecal({ (fSelectedCellX - 0.5f - fOffsetX) * 64  ,(fSelectedCellY - 0.5f - fOffsetY) * 64 }, DynamicLayer, { 0,6 * 256 }, { 256,256 }, { 0.5,0.5 });
			break;
		case 17:
			DrawPartialDecal({ (fSelectedCellX - 0.5f - fOffsetX) * 64 ,(fSelectedCellY - 0.5f - fOffsetY) * 64 }, DynamicLayer, { 0,7 * 256 }, { 256,256 }, { 0.5,0.5 });
			break;





		}

		
		
		//SetDrawTarget(nLayerBackground);
		//SetPixelMode(olc::Pixel::ALPHA);     //Draw mouse seleceted slot
		//DrawPartialDecal({ (fSelectedCellX - 0.5f - fOffsetX) * 64 ,(fSelectedCellY - 0.5f - fOffsetY) * 64 }, RPG_Assets::get().GetSprite("Lvl1Constructor"), { (int)savestrecX *(float)64,(int)savestrecY*(float)64 }, {(float)nTileWidth*(int)saveenrecX,(float)nTileHeight*(int)saveenrecY }, { 1,1 });
		//SetPixelMode(olc::Pixel::NORMAL);
		//EnableLayer(nLayerBackground, true);
		//SetDrawTarget(nullptr);


		FillRect(ScreenWidth() - 140, ScreenHeight() - 97, 110, 45+32, { 67,68,85 });
		DrawRect(ScreenWidth() - 85, ScreenHeight() - 100, 110/2, 32);
		DrawRect(ScreenWidth() - 140, ScreenHeight() - 100, 110/2, 32);
		DrawString(ScreenWidth() - 132, ScreenHeight() - 60, std::string("Create"), olc::YELLOW, 2);
		DrawString(ScreenWidth() - 132, ScreenHeight() - 40, std::string("Level"), olc::YELLOW, 2);


		//
		if (IsTextEntryEnabled())
		{
			DrawString(0, 0, "Text entry Mode", olc::YELLOW);



			if (bWriteWidth)
			{

			FillRect({ TextEntryGetCursor()*16 + ScreenWidth() - 140,ScreenHeight() - 100 }, { 4,16 }, olc::RED);

			DrawString({ ScreenWidth() - 140, ScreenHeight() - 100 }, TextEntryGetString(), olc::WHITE, 2);
			DrawString(ScreenWidth() - 85, ScreenHeight() - 100, SHeight, olc::YELLOW, 2);

			}
			else
			{

				FillRect({ TextEntryGetCursor() * 16 + ScreenWidth() - 85,ScreenHeight() - 100 }, { 4,16 }, olc::RED);

				DrawString({ ScreenWidth() - 85, ScreenHeight() - 100 }, TextEntryGetString(), olc::WHITE, 2);

				DrawString(ScreenWidth() - 140, ScreenHeight() - 100, SWidth, olc::YELLOW, 2);
			}

		}
		else
		{

			DrawString(ScreenWidth() - 85, ScreenHeight() - 100, SHeight, olc::YELLOW, 2);
			DrawString(ScreenWidth() - 140, ScreenHeight() - 100, SWidth, olc::YELLOW, 2);
		}
		//

		DrawRect((fSelectedCellX - fOffsetX-0.5 ) * 64, (fSelectedCellY - fOffsetY-0.5 ) * 64, (float)nTileWidth* (int)saveenrecX, (float)nTileHeight* (int)saveenrecY, olc::RED);

	/*	for (auto& source : { &m_vecCells })
			for (auto& dyns : *source)
				dyns->DrawSelf(this, fOffsetX, fOffsetY);*/

		DrawString(ScreenWidth()-200, 0, std::string(std::to_string(fScaleX)), olc::WHITE, 2);
		DrawString(ScreenWidth() - 200, 50, std::string(std::to_string(fSelectedCellX - 0.5f)), olc::WHITE, 1);
		DrawString(ScreenWidth() - 100, 50, std::string(std::to_string(fSelectedCellY - 0.5f)), olc::WHITE, 1);

		FillRect(0, 640, ScreenWidth()/2, 128,{67,68,85});  // rectangle for layers

		switch (currLayer)
		{
		case 0:
			FillRect(10, 675, 24, 74, { 135,135,135 });
			break;

		case 1:
		FillRect(55, 675, 74, 74,{135,135,135});
			break;
		case 2:
		FillRect(149, 675, 74, 74, { 135,135,135 });

			break;
		case 3:
		FillRect(243, 675, 74, 74, { 135,135,135 });

			break;
		case 4:    // Dynamic layer. in general it's between first and second layer

			FillRect(450, 675, 54, 74, { 135,135,135 });
		}


		if (bseeZero)
		{
			FillCircle(22, 660, 10);     //circle when we extract 

		}

		if (bseeFirst)
		{
		FillCircle(92, 660, 10);     //circle when we extract 

		}
		if (bseeSecond)
		{
		FillCircle(186, 660, 10); //circle when we extract 

		}
		if (bseeThird)
		{
		FillCircle(280, 660, 10);     //circle when we extract 
		}
		if (bseeDynamic)
		{
			FillCircle(450 + 27, 660, 10);    // circle when we off dynamic layer
		}


		DrawRect(10, 675, 24, 74); // Frame for Zero layer
		DrawCircle(22, 660, 12);   // circle when we off him


		DrawRect(55, 675, 74, 74); // Frame for first layer
		DrawCircle(92, 660, 12);   // circle when we off him

		DrawRect(149, 675, 74, 74);  //Frame for Second layer
		DrawCircle(186, 660, 12); // circle when we off him

		DrawRect(243, 675, 74, 74); //Frame for Third layer
		DrawCircle(280, 660, 12);    // circle when we off him


		DrawRect(450, 675, 54, 74); // Frame for Dynamic layer
		DrawCircle(450+27, 660, 12);    // circle when we off dynamic layer



		FillRect(22, 675, 12, 12, olc::RED); // for deleting full layer Zero

		FillRect(129-24, 675, 24, 24,olc::RED); // for deleting full layer FIrt

		FillRect(223 - 24, 675, 24, 24, olc::RED); // for deleting full layer Second

		FillRect(317 - 24, 675, 24, 24, olc::RED); // for deleting full layer Second


		FillRect(504 - 24, 675, 24, 24, olc::RED); // for deleting full layer Dynamic

		DrawString(360, 660, std::string("Load"),olc::YELLOW, 2);
		DrawRect(360-5, 660-5, 70, 25, olc::YELLOW);
		DrawString(360, 720, std::string("Save"), olc::YELLOW, 2);
		DrawRect(360 - 5, 720 - 5, 70, 25, olc::YELLOW);
		
	}

	// Logic For Dynamic Layer
	if (animsprite > framespeed)
	{
		frameIndex++;
		animsprite = 0;

	}
	if (frameIndex >= 8)
	{
		frameIndex = 0;
	}
	//
		return true;
}

bool Map_Editor::CheckGrabbedObject(float fselectedCellX, float fselectedCelly)
{


	if ((int)fSelectedCellX < 34 && (int)fSelectedCellY < 34)   //return zero
	{
		CollNumber = 0;
		return true;

	}
	if ((int)fSelectedCellX == 35 && (int)fSelectedCellY == 4)        //one mean hard cell
	{
		CollNumber = 1;
		return true;
	}
	if ((int)fSelectedCellX == 37 && (int)fSelectedCellY == 4)     //obsticle left right
	{
		CollNumber = 2;
		return true;
	}
	if ((int)fSelectedCellX == 39 && (int)fSelectedCellY == 4)    //obsticle right lef
	{
		CollNumber = 3;
		return true;
	}
	if ((int)fSelectedCellX == 41 && (int)fSelectedCellY == 4)   // rope
	{
		CollNumber = 4;
		return true;
	}
	if ((int)fSelectedCellX == 43 && (int)fSelectedCellY == 4)   // spikes verical
	{
		CollNumber = 5;
		return true;
	}
	if ((int)fSelectedCellX == 45 && (int)fSelectedCellY == 4)   // teleport
	{
		CollNumber = 6;
		return true;
	}

	if ((int)fSelectedCellX >= 1 && (int)fSelectedCellY >= 35 && (int)fSelectedCellX <= 4 && (int)fSelectedCellY <= 38)   // Dynamic first slot
	{
		CollNumber = 10;
		return true;
	}
	if ((int)fSelectedCellX >= 5 && (int)fSelectedCellY >= 35 && (int)fSelectedCellX <= 8 && (int)fSelectedCellY <= 38)   // Dynamic Second slot
	{
		CollNumber = 11;
		return true;
	}
	if ((int)fSelectedCellX >= 9 && (int)fSelectedCellY >= 35 && (int)fSelectedCellX <= 12 && (int)fSelectedCellY <= 38)   // Dynamic third slot
	{
		CollNumber = 12;
		return true;
	}
	if ((int)fSelectedCellX >= 13 && (int)fSelectedCellY >= 35 && (int)fSelectedCellX <= 16 && (int)fSelectedCellY <= 38)   // Dynamic four slot
	{
		CollNumber = 13;
		return true;
	}
	if ((int)fSelectedCellX >= 17 && (int)fSelectedCellY >= 35 && (int)fSelectedCellX <= 20 && (int)fSelectedCellY <= 38)   // Dynamic five slot
	{
		CollNumber = 14;
		return true;
	}

	if ((int)fSelectedCellX >= 21 && (int)fSelectedCellY >= 35 && (int)fSelectedCellX <= 24 && (int)fSelectedCellY <= 38)   // Dynamic six slot
	{
		CollNumber = 15;
		return true;
	}
	if ((int)fSelectedCellX >= 25 && (int)fSelectedCellY >= 35 && (int)fSelectedCellX <= 28 && (int)fSelectedCellY <= 38)   // Dynamic seven slot
	{
		CollNumber = 16;
		return true;
	}
	if ((int)fSelectedCellX >= 29 && (int)fSelectedCellY >= 35 && (int)fSelectedCellX <= 32 && (int)fSelectedCellY <= 38)   // Dynamic eight slot
	{
		CollNumber = 17;
		return true;
	}
	return false;
}

void Map_Editor::DrawStaticFrame(int RecFirstX, int RecFirstY, int RecSecX,int RecSecY, float fscale) // here came full size in pixel (0,0,2048,2048)
{

	int spriteWidth = RPG_Assets::get().GetSprite("StaticFrame")->sprite->width;
	int spriteHeigh = RPG_Assets::get().GetSprite("StaticFrame")->sprite->height;

	olc::Decal* StaticFrame = RPG_Assets::get().GetSprite("StaticFrame");


	int FrameSocketX = spriteWidth / 3;
	int FrameSocketY = spriteHeigh / 3;


	float xRepitablesocketsX = (RecSecX - RecFirstX) / FrameSocketX ;  // how much times should repitable tile parts of staticframe
	float xRepitablesocketsY = (RecSecY - RecFirstY) / FrameSocketY ;


	int sparedPartsX = 42;
	int sparedPartsY = 42;

	if (xRepitablesocketsX <0)
	{
		xRepitablesocketsX = 0;
		sparedPartsX = 0;
	}
	if (xRepitablesocketsY <0)
	{
		xRepitablesocketsY = 0;
		sparedPartsY = 0;
	}

	
	


	int OffsetX = RecFirstX - 21;
	int offsetY = RecFirstY - 21;
	int OffsetFarX = RecSecX + 21;
	int OffsetFarY = RecSecY + 21;


	


	for (int x = 0; x < xRepitablesocketsX-2;x++)  // Draw repitable tileable parts of staticframe Xfirst frame to right corner
	{
		DrawPartialDecal({ ((float)OffsetX +FrameSocketX + (x*FrameSocketX))*fscale  ,(float)offsetY*fscale }, StaticFrame, { (float)64,(float)0 }, { (float)FrameSocketX,(float)FrameSocketY }, { fscale,fscale }); //Draw tilable part horizontal Up

		DrawPartialDecal({ ((float)OffsetX + FrameSocketX + (x * FrameSocketX)) * fscale  ,(float)(OffsetFarY -FrameSocketY) *fscale }, StaticFrame, { (float)64,(float)128 }, { (float)FrameSocketX,(float)FrameSocketY }, { fscale,fscale }); //Draw tilable part horizontal Down
	}



	for (int y = 0; y < xRepitablesocketsY-2; y++)  // Draw repitable tileable parts of staticframe yfirst frame to Down corners
	{
		DrawPartialDecal({ (float)OffsetX*fscale  ,((float)offsetY + FrameSocketY + (y * FrameSocketX)) * fscale }, StaticFrame, { (float)0,(float)64 }, { (float)FrameSocketX,(float)FrameSocketY }, { fscale,fscale }); //Draw tilable part horizontal Up

		DrawPartialDecal({ ((float)OffsetFarX - FrameSocketX) * fscale  ,((float)offsetY + FrameSocketY + (y * FrameSocketX)) * fscale }, StaticFrame, { (float)128,(float)64 }, { (float)FrameSocketX,(float)FrameSocketY }, { fscale,fscale }); //Draw tilable part horizontal Down
	}



	DrawPartialDecal({ (float)OffsetX *fscale  ,(float)offsetY *fscale}, StaticFrame, {(float)0,(float)0}, {(float)FrameSocketX,(float)FrameSocketY}, {fscale,fscale}); //Draw First LeftUp Angle



	DrawPartialDecal({ (float)(OffsetFarX - FrameSocketX- sparedPartsX) * fscale  ,(float)offsetY *fscale }, StaticFrame, { (float)128 - sparedPartsX ,(float)0 }, { (float)FrameSocketX+ sparedPartsX,(float)FrameSocketY }, { fscale,fscale }); //Draw First RightUp Angle

	DrawPartialDecal({ (float)OffsetX * fscale  ,(float)(OffsetFarY - FrameSocketY- sparedPartsX)*fscale }, StaticFrame, { (float)0,(float)128- sparedPartsY }, { (float)FrameSocketX,(float)FrameSocketY+ sparedPartsY }, { fscale,fscale }); //Draw First LeftDown Angle

	DrawPartialDecal({ (float)(OffsetFarX - FrameSocketX- sparedPartsX) * fscale  ,(float)(OffsetFarY - FrameSocketY- sparedPartsY) *fscale }, StaticFrame, { (float)128- sparedPartsX,(float)128- sparedPartsY }, { (float)FrameSocketX+ sparedPartsX,(float)FrameSocketY+ sparedPartsY }, { fscale,fscale }); //Draw First LeftRight Angle

}

void Map_Editor::DrawDynamicLayer(float px, float py, float fscale, float felapsedtime)
{
	

	int dyncount = 2048 / 256;


	for (int i = 0; i < dyncount; i++)
	{
		DrawPartialDecal({ px * 0.3f+(i*(256*fscale))  ,py * 0.3f}, DynamicLayer, {(float)frameIndex*256,(float)i * 256}, {(float)256,(float)256}, {0.3,0.3});
	}


	//DrawPartialDecal({ px * 0.3f  ,py }, DynamicLayer, { (float)0,(float)0 }, { (float)256,(float)2048 }, { 0.3,0.3 });

	


}
