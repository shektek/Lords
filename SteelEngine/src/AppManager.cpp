#include "AppManager.h"
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

AppManager::AppManager()
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	irrDevice = nullptr;

	m_width = 1600;
	m_height = 900;
	m_vsync = true;
	m_fullscreen = false;
	m_bpp = 32;
	m_windowName = "SteelEngine";
}

AppManager::AppManager(bool debug)
{
	if(debug)
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
	}

	irrDevice = nullptr;

	m_width = 640;
	m_height = 480;
	m_vsync = false;
	m_fullscreen = false;
	m_bpp = 32;
	m_windowName = "SteelEngine";
}

AppManager::~AppManager()
{

}

void AppManager::CreateBox(const btVector3 &pos, const irr::core::vector3df &scale, btScalar mass)
{
	irr::scene::ISceneNode *cubenode = irrScene->addCubeSceneNode(1.0f);
	cubenode->setScale(scale);
	cubenode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	cubenode->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	cubenode->setMaterialTexture(0, irrDriver->getTexture("resources/textures/stones.jpg"));

	//initial transform
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(pos);

	btDefaultMotionState *motionstate = new btDefaultMotionState(trans);

	//shape itself
	btVector3 halfextents(scale.X*0.5f, scale.Y*0.5f, scale.Z*0.5f);
	btCollisionShape *shape = new btBoxShape(halfextents);

	//and the mass
	btVector3 localinertia;
	shape->calculateLocalInertia(mass, localinertia);

	btRigidBody *rigidbody = new btRigidBody(mass, motionstate, shape, localinertia);

	//set a pointer to the irrlicht node that refers to this, pretty handy
	rigidbody->setUserPointer((void*)cubenode);

	//now put it into the bullet world
	btWorld->addRigidBody(rigidbody);
	btObjects.push_back(rigidbody);
}

bool AppManager::InitIrrlicht()
{
	bool result = false;
	wchar_t str[256] = { 0 };

	irrDevice = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(m_width, m_height), m_bpp, m_fullscreen, true, m_vsync, nullptr);
	if (irrDevice)
	{
		MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, m_windowName.c_str(), m_windowName.size(), str, 256);

		irrDevice->setWindowCaption(str);

		irrDriver = irrDevice->getVideoDriver();
		irrScene = irrDevice->getSceneManager();
		irrTimer = irrDevice->getTimer();
		irrGUI = irrDevice->getGUIEnvironment();

		//set up gui stuff with defaults
		irrGUI->getSkin()->setFont(irrGUI->getFont("resources/fonts/cormorant_large.png"));
		m_textures["logo"] = irrDriver->getTexture("resources/textures/logo.png");
		m_textures["newgame"] = irrDriver->getTexture("resources/textures/newgame.png");
		m_cameras["default"] = irrScene->addCameraSceneNodeFPS();
		m_cameras["gui"] = irrScene->addCameraSceneNode();
		irrScene->setActiveCamera(m_cameras["default"]);

		//done, set the event receiver
		irrDevice->setEventReceiver(this);

		result = true;
	}

	return result;
}

bool AppManager::InitGame()
{
	bool result = false;

	m_GUIState = new Steel::GUIStateMachine(Steel::GUIState::G_MAINMENU);
	m_gameMaster = new Steel::GameMaster();
	if (m_gameMaster != nullptr)
	{
		m_gameMaster->peasantFactory = new PeasantFactory("resources/gen/peasant.txt");
		m_gameMaster->resourceFactory = new ResourceFactory();
		m_gameMaster->territoryFactory = new TerritoryFactory("resources/gen/territory.txt", m_gameMaster->peasantFactory);
		m_gameMaster->terrainGenerator = new Steel::TerrainGenerator();
		m_gameMaster->terrainFactory = new Steel::TerrainFactory();
		result = true;
	}

	return true;
}

bool AppManager::InitBullet(bool debug)
{
	bool result = false;

	//the axis sweep will have to actually reflect the world dimensions but w/e for now
	btBroadPhase = new btAxisSweep3(btVector3(-500, -500, -500), btVector3(500, 500, 500));
	btCollisionConfig = new btDefaultCollisionConfiguration();
	if (btCollisionConfig != nullptr)
	{
		btDispatcher = new btCollisionDispatcher(btCollisionConfig);
		btSolver = new btSequentialImpulseConstraintSolver();
		btWorld = new btDiscreteDynamicsWorld(btDispatcher, btBroadPhase, btSolver, btCollisionConfig);
		if (btWorld != nullptr)
		{
			btWorld->getSolverInfo().m_solverMode |= SOLVER_ENABLE_FRICTION_DIRECTION_CACHING;
			btWorld->getSolverInfo().m_numIterations = 5;
			btWorld->setGravity(btVector3(0, -9.8, 0));

			result = true;
		}
	}

	if (debug)
	{
		debugDraw = new DebugDraw(irrDevice);
		debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe |
		btIDebugDraw::DBG_DrawAabb |
		btIDebugDraw::DBG_DrawContactPoints |
		btIDebugDraw::DBG_DrawConstraints);
		btWorld->setDebugDrawer(debugDraw);
		debugMat.Lighting = false;
	}

	return true;
}

bool AppManager::Init()
{
	bool ret = false;
	
	if(InitIrrlicht())
	{
		//get main game file and start
		InitGame();

		//initialize bullet world
		InitBullet();

		//just testing...
		m_gameMaster->AddLevel("resources/levels/leveltest.lvl");
		m_gameMaster->SetCurrentLevel("resources/levels/leveltest.lvl");
		Steel::Terrain *tptr = m_gameMaster->GetCurrentLevel()->terrain;
		
		m_terrains[tptr->GetName()] = irrScene->addTerrainSceneNode(tptr->GetHeightmapFilename().c_str(), 0, -1, irr::core::vector3df(), irr::core::vector3df(), irr::core::vector3df(tptr->GetScale(), 1.0, tptr->GetScale()));
		
		m_terrains[tptr->GetName()]->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		m_terrains[tptr->GetName()]->setMaterialTexture(0, irrDriver->getTexture(tptr->GetDiffuseFilename().c_str()));

		//add a detail map
		/*if (tptr->GetDetailFilename().size() > 1)
		{
			m_terrains[tptr->GetName()]->setMaterialTexture(1, irrDriver->getTexture(tptr->GetDetailFilename().c_str()));
			m_terrains[tptr->GetName()]->setMaterialType(irr::video::EMT_DETAIL_MAP);
		}*/

		m_terrains[tptr->GetName()]->scaleTexture(1.0f, 20.0f);

		m_cameras["default"]->setPosition(irr::core::vector3df(-400, 1700, -400));
		m_cameras["gui"]->setPosition(m_cameras["default"]->getPosition());

		//add the test terrain to the bullet world
		m_terrains[tptr->GetName()]->updateAbsolutePosition();
		irr::core::vector3df irrPos = m_terrains[tptr->GetName()]->getPosition();
		btVector3 btPos(irrPos.X, irrPos.Y, irrPos.Z);
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(btPos);

		btDefaultMotionState *ms = new btDefaultMotionState(trans);

		btTriangleMesh *mtrimesh = new btTriangleMesh();
		irr::scene::CDynamicMeshBuffer *irrBuf;
		btVector3 vertices[3];
		irr::s32 j, k;
		irrBuf = new irr::scene::CDynamicMeshBuffer(irr::video::EVT_2TCOORDS, irr::video::EIT_32BIT);
		m_terrains[tptr->GetName()]->getMeshBufferForLOD(*irrBuf);

		irr::core::vector3df tscale = m_terrains[tptr->GetName()]->getScale();

		//now construct a triangle mesh
		const irr::u32 indexCount = irrBuf->getIndexCount();
		irr::video::S3DVertex2TCoords *mb_vertices = (irr::video::S3DVertex2TCoords*) irrBuf->getVertexBuffer().getData();
		irr::u32 *mb_indices = (irr::u32*)irrBuf->getIndices();
		for (j = 0; (irr::u32)j < indexCount; j += 3)
		{
			for (k = 0; k < 3; k++)
			{
				irr::s32 index = mb_indices[j + k];
				vertices[k] = btVector3(mb_vertices[index].Pos.X * tscale.X, mb_vertices[index].Pos.Y * tscale.Y, mb_vertices[index].Pos.Z * tscale.Z);
			}
			mtrimesh->addTriangle(vertices[0], vertices[1], vertices[2]);
		}

		irrBuf->drop();

		btBvhTriangleMeshShape *mshape = new btBvhTriangleMeshShape(mtrimesh, true);
		btRigidBody *rb = new btRigidBody(0, ms, mshape);
		rb->setUserPointer((void*)m_terrains[tptr->GetName()]);
		btWorld->addRigidBody(rb);
		btObjects.push_back(rb);

		m_running = true;
		ret = true;
	}

	return ret;
}

void AppManager::Run()
{
	irr::u32 now = irrTimer->getTime(), delta = 0;

	while(m_running)
	{
		delta = irrTimer->getTime() - now;
		now = irrTimer->getTime();

		m_running = irrDevice->run();

		UpdatePhysics(delta);

		irrDriver->beginScene();

		//temporary territory outline drawing code
		std::vector<Territory*> *tv = &m_gameMaster->levels[0]->territories;
		
		for (int i = 0; i < tv->size(); i++)
		{
			std::vector<Pos3> mapBounds = tv->at(i)->GetMapBounds();

			for (int j = 0; j < mapBounds.size(); j++)
			{
				float scale = 1.0;

				int x = mapBounds[j].x * scale;
				int z = mapBounds[j].z * scale;
				

				irr::core::vector3df f(x, mapBounds[j].y + 100, z);
				irr::core::vector3df t;

				if (j + 1 != mapBounds.size())
					t = irr::core::vector3df(mapBounds[j + 1].x * scale, mapBounds[j + 1].y + 100, mapBounds[j + 1].z * scale);
				else
					t = irr::core::vector3df(mapBounds[0].x * scale, mapBounds[0].y + 100, mapBounds[0].z * scale);

				irr::video::SMaterial m;
				m.Lighting = false;
				irrDriver->setMaterial(m);
				irrDriver->draw3DLine(f, t, irr::video::SColor(255, 255, 255, 255));
			}

		}

		irrScene->drawAll();
		
		/*irrDriver->setMaterial(debugMat);
		irrDriver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
		btWorld->debugDrawWorld();*/

		UpdateGUI();
		irrGUI->drawAll();

		irrDriver->endScene();
	}
}

bool AppManager::OnEvent(const irr::SEvent &event)
{

	bool ret = true;

	if (event.MouseInput.isLeftPressed())
	{
		Steel::Terrain *tptr = m_gameMaster->levels[0]->terrain;
		int r = (rand() % 140) - 71;
		btVector3 center(m_terrains[tptr->GetName()]->getTerrainCenter().X + r, m_terrains[tptr->GetName()]->getTerrainCenter().Y + 400, m_terrains[tptr->GetName()]->getTerrainCenter().Z + r);

		CreateBox(center, irr::core::vector3df(50.0, 50.0, 50.0), 10.0);
	}

	switch(event.EventType)
	{
		//very hacky
		//really needs an input handler of some kind to switch between actionsets
		case irr::EEVENT_TYPE::EET_KEY_INPUT_EVENT:
		{
			switch (event.KeyInput.Char)
			{
				case 'c':
				if (irrScene->getActiveCamera() == m_cameras["default"])
					irrScene->setActiveCamera(m_cameras["gui"]);
				else
					irrScene->setActiveCamera(m_cameras["default"]);
			}
		}
		break;

		case irr::EEVENT_TYPE::EET_GUI_EVENT:
		{
			//this tells us which specific element of the GUI was selected
			int eventId = event.GUIEvent.Caller->getID();
			
			switch(event.GUIEvent.EventType)
			{
				case irr::gui::EGUI_EVENT_TYPE::EGET_SCROLL_BAR_CHANGED:
				break;
				
				case irr::gui::EGUI_EVENT_TYPE::EGET_BUTTON_CLICKED:	
					//main menu buttons
					if (eventId == Steel::MainMenuID::M_MM_NEWGAME_ID)
						m_GUIState->ChangeState(Steel::GUIState::G_NEWGAME);
					else if (eventId == Steel::MainMenuID::M_MM_LOADGAME_ID)
						m_GUIState->ChangeState(Steel::GUIState::G_LOADGAME);
					else if (eventId == Steel::MainMenuID::M_MM_OPTIONS_ID)
						m_GUIState->ChangeState(Steel::GUIState::G_OPTIONSMENU);
					else if (eventId == Steel::MainMenuID::M_MM_QUIT_ID)
					{
						m_running = false;
						irrDevice->closeDevice();
					}

					//new game buttons
					if (eventId == Steel::NewGameMenuID::M_NG_BACK_ID)
						m_GUIState->Back();
					if (eventId == Steel::NewGameMenuID::M_NG_REGENRANDOM_ID)
					{
//						m_gameMaster->territories.clear();
						irr::gui::IGUIElement *br = irrGUI->getRootGUIElement()->getElementFromId(Steel::NewGameMenuID::M_NG_MINTERRITORIES_ID, true);
						irr::gui::IGUIElement *pr = irrGUI->getRootGUIElement()->getElementFromId(Steel::NewGameMenuID::M_NG_MAXTERRITORIES_ID, true);

						int min = _wtoi(br->getText());
						int max = _wtoi(pr->getText());

						int rnum = (rand() % (max+1-min)) + min;

						printf("Creating %d new territories...\n", rnum);

						for (int i = 0; i < rnum; i++)
						{
							//m_gameMaster->territories.push_back(m_gameMaster->territoryFactory->CreateNewRandomTerritory());
							//m_gameMaster->territories.back()->m_peasantFactory = m_gameMaster->peasantFactory;

							//printf("%s\n", m_gameMaster->territories[i]->GetName().c_str());
						}

						//now create a new terrain for it
						//TODO: scale according to territory count
						///*
						//delete[] m_gameMaster->terrain;
						//m_gameMaster->terrain = m_gameMaster->terrainGenerator->Generate(m_gameMaster->terrainWidth, m_gameMaster->terrainHeight);
						//irr::io::IReadFile *tfile = irrDevice->getFileSystem()->createMemoryReadFile(m_gameMaster->terrain, m_gameMaster->terrainWidth*m_gameMaster->terrainHeight, L"RandomHeightmap");
						//m_terrains["RandomHeightmap"] = irrScene->addTerrainSceneNode(NULL, 0, -1, irr::core::vector3df(0.f, 0.f, 0.f), irr::core::vector3df(0.f, 0.f, 0.f), irr::core::vector3df(40.f, 4.4f, 40.f),
						//	irr::video::SColor(0, 0, 0, 255), 5, irr::scene::ETPS_17, 4, true);
						//m_terrains["RandomHeightmap"]->loadHeightMapRAW(tfile);
						//*/

					}
				break;

				default:
					ret = false;
			}
		}
		break;

		default:
			ret = false;
	}

	//printf("%.2lf %.2lf %.2lf\n", m_cameras["default"]->getRotation().X, m_cameras["default"]->getRotation().Y, m_cameras["default"]->getRotation().Z);

	return ret;
}

void AppManager::UpdateGUI()
{
	if (m_GUIState->UpdateRequired())
	{
		//0,0  1,0
		//0,1  1,1

		double winx = m_width;
		double winy = m_height;
		double winr = m_width / m_height;
		double xa = m_width / 1000.00;
		double ya = m_height / 1000.00;
		double bw = 160 * xa;
		double bh = 50 * ya;
		double bg = 40 * ya;

		irrGUI->clear();

		//Everything in here only ever happens on state change
		m_GUIState->Tick();

		//now alter irrGUI with whatever objects are required
		Steel::GUIState cur = m_GUIState->GetCurrentState();
		switch (cur)
		{

		case Steel::GUIState::G_SPLASH:
			break;

		case Steel::GUIState::G_LOADING:
			break;

		case Steel::GUIState::G_MAINMENU:
			//logo at top!
			irrGUI->addImage(m_textures["logo"], irr::core::vector2d<irr::s32>((winx*0.5) - 250, 0));
			//new game
			irrGUI->addButton(irr::core::rect<irr::s32>((winx*0.5) - (bw/2), 260+bh, (winx*0.5) + (bw/2),260+(bh*2)), 0, Steel::MainMenuID::M_MM_NEWGAME_ID, L"New Game", L"Begin a new conquest");
			//load game
			irrGUI->addButton(irr::core::rect<irr::s32>((winx*0.5) - (bw / 2), 260+bh*2+bg, (winx*0.5) + (bw / 2), 260+bh*3+bg), 0, Steel::MainMenuID::M_MM_LOADGAME_ID, L"Load Game", L"Continue a previous conquest");
			//options 
			irrGUI->addButton(irr::core::rect<irr::s32>((winx*0.5) - (bw / 2), 260 + bh * 3 + bg * 2, (winx*0.5) + (bw / 2), 260 + bh * 4 + bg*2), 0, Steel::MainMenuID::M_MM_OPTIONS_ID, L"Options", L"Configure game settings");
			//quit
			irrGUI->addButton(irr::core::rect<irr::s32>((winx*0.5) - (bw / 2), 260 + bh * 4 + bg * 4, (winx*0.5) + (bw / 2), 260 + bh * 5 + bg * 4), 0, Steel::MainMenuID::M_MM_QUIT_ID, L"Quit", L"Return to the mundane");
			break;

		case Steel::GUIState::G_NEWGAME:
		{
			//new game image because this engine apparently can't stretch font textures
			irrGUI->addImage(m_textures["newgame"], irr::core::vector2d<irr::s32>((winx*0.5) - 250, 0));

			irrGUI->addButton(irr::core::rect<irr::s32>(bg, winy - (bh + bg), bg + bw, winy - bg), 0, Steel::NewGameMenuID::M_NG_BACK_ID, L"Back", L"Return to previous menu");

			irrGUI->addButton(irr::core::rect<irr::s32>(winx - (bw + bg), winy - (bh + bg), winx - bg, winy - bg), 0, Steel::NewGameMenuID::M_NG_STARTGAME_ID, L"Start", L"The adventure begins!");

			//buttons for game type - campaign or custom scenario
			irr::gui::IGUITabControl *tabctrl = irrGUI->addTabControl(QuickRect(100, 200, winx - 100, winy - bg * 4), 0, true, true, Steel::NewGameMenuID::M_NG_GAMETAB_ID);
			irr::gui::IGUITab *scntab = tabctrl->addTab(L"Scenario", Steel::NewGameMenuID::M_NG_CHECKSCENARIO_ID);
			irr::gui::IGUITab *cmptab = tabctrl->addTab(L"Campaign", Steel::NewGameMenuID::M_NG_CHECKCAMPAIGN_ID);

			//Scenario options
			//list box of locations
			irr::gui::IGUIListBox *maplist = irrGUI->addListBox(QuickRect(25, 25, 175, 175), scntab);
			maplist->addItem(L"Random");
			//regenerate random
			irrGUI->addButton(QuickRect(200, 100, 200 + bw, 100 + bh), scntab, Steel::NewGameMenuID::M_NG_REGENRANDOM_ID, L"Regenerate Random", L"Create a new random map, discarding the current one");
			//min/max territories to generate randomly
			irrGUI->addStaticText(L"Min/Max territories", QuickRect(170, 10, 330, 25), false, false, scntab);
			irrGUI->addEditBox(L"2", QuickRect(200, 25, 250, 25 + bh), true, scntab, Steel::NewGameMenuID::M_NG_MINTERRITORIES_ID);
			irrGUI->addEditBox(L"6", QuickRect(260, 25, 310, 25 + bh), true, scntab, Steel::NewGameMenuID::M_NG_MAXTERRITORIES_ID);
		}
			break;
		default:
			irrGUI->addButton(irr::core::rect<irr::s32>(m_width / 2 - 80, m_height / 2 - 15, m_width / 2 + 80, m_height / 2 + 15), 0, Steel::NewGameMenuID::M_NG_BACK_ID, L"Back", L"Return to previous menu");
			break;
			//no menu here!
		}
	}
}

void AppManager::UpdatePhysics(irr::u32 delta)
{
	btWorld->stepSimulation(delta * 0.001, 60);

	for (btRigidBody *rb : btObjects)
	{
		irr::scene::ISceneNode *node = static_cast<irr::scene::ISceneNode*>(rb->getUserPointer());

		if (node->getType() != irr::scene::ESNT_TERRAIN)
		{

			btVector3 point = rb->getCenterOfMassPosition();
			node->setPosition(irr::core::vector3df((irr::f32)point[0], (irr::f32)point[1], (irr::f32)point[2]));

			btVector3 eulers;
			QuaternionToEuler(rb->getOrientation(), eulers);
			node->setRotation(irr::core::vector3df(eulers[0], eulers[1], eulers[2]));
		}
	}
}

void AppManager::QuaternionToEuler(const btQuaternion &quat, btVector3 &euler)
{
	btScalar coords[4] = { quat.getW(), quat.getX(), quat.getY(), quat.getZ() };
	double squares[4] = { coords[0] * coords[0], coords[1] * coords[1], coords[2] * coords[2], coords[3] * coords[3] };

	euler.setX(atan2(2.0 * (coords[2] * coords[3] + coords[1] * coords[0]), -squares[1] - squares[2] + squares[3] + squares[0]));
	euler.setY(asin(-2.0 * (coords[1] * coords[3] - coords[2] * coords[0])));
	euler.setZ(atan2(2.0 * (coords[1] * coords[2] + coords[3] * coords[0]), squares[1] - squares[2] - squares[3] + squares[0]));
	euler *= irr::core::RADTODEG;
}

btRigidBody *AppManager::IrrMeshToBullet(const irr::scene::ISceneNode *node)
{
	irr::scene::ESCENE_NODE_TYPE type = node->getType();

	irr::scene::IAnimatedMeshSceneNode *meshnode = nullptr;
	btRigidBody *rb = nullptr;

	switch (type)
	{
	case irr::scene::ESNT_ANIMATED_MESH:
		meshnode = (irr::scene::IAnimatedMeshSceneNode*)node;
		break;

	default: break;
	}

	if (meshnode)
	{
		const irr::core::vector3df nodescale = meshnode->getScale();
		irr::scene::IMesh *mesh = meshnode->getMesh();
		const size_t bufsize = mesh->getMeshBufferCount();
		btVector3 pos(meshnode->getPosition().X, meshnode->getPosition().Y, meshnode->getPosition().Z);

		std::vector<irr::video::S3DVertex> verts;
		std::vector<int> inds;

		for (int i = 0; i < bufsize; i++)
		{
			irr::scene::IMeshBuffer *buffer = mesh->getMeshBuffer(i);
			const irr::video::E_VERTEX_TYPE vertexType = buffer->getVertexType();
			const irr::video::E_INDEX_TYPE indexType = buffer->getIndexType();
			const int numVerts = buffer->getVertexCount();
			const int numInds = buffer->getIndexCount();
			
			verts.resize(verts.size() + numVerts);
			inds.resize(inds.size() + numInds);

			void *vertices = buffer->getVertices();
			void *indices = buffer->getIndices();

			irr::video::S3DVertex *standard = reinterpret_cast<irr::video::S3DVertex*>(vertices);
			irr::video::S3DVertex2TCoords *two2coords = reinterpret_cast<irr::video::S3DVertex2TCoords*>(vertices);
			irr::video::S3DVertexTangents *tangents = reinterpret_cast<irr::video::S3DVertexTangents*>(vertices);

			irr::u16 *ind16 = reinterpret_cast<irr::u16*>(indices);
			irr::u32 *ind32 = reinterpret_cast<irr::u32*>(indices);

			for (int v = 0; v < numVerts; v++)
			{
				auto &vert = verts[v];
				
				switch (vertexType)
				{
					case irr::video::EVT_STANDARD:
					{
						const auto &irrv = standard[v];
						vert = irrv;
					}
					break;
					
					default:
					//unknown
						break;
				}
			}

			for (int n = 0; n < numInds; n++)
			{
				auto &index = inds[n];

				switch (indexType)
				{
					case irr::video::EIT_16BIT:
						index = ind16[n];
						break;

					case irr::video::EIT_32BIT:
						index = ind32[n];
						break;

					default:
						//unknown
						break;
				}
			}
		}

		if (!verts.empty() && !inds.empty())
		{
			const int numInds = inds.size();
			const int numTris = numInds / 3;

			btTriangleMesh *btmesh = new btTriangleMesh();

			for (int i = 0; i < numInds; i += 3)
			{
				btVector3 a(verts[inds[i + 0]].Pos.X, verts[inds[i + 0]].Pos.Y, verts[inds[i + 0]].Pos.Z);
				btVector3 b(verts[inds[i + 1]].Pos.X, verts[inds[i + 1]].Pos.Y, verts[inds[i + 1]].Pos.Z);
				btVector3 c(verts[inds[i + 2]].Pos.X, verts[inds[i + 2]].Pos.Y, verts[inds[i + 2]].Pos.Z);

				btmesh->addTriangle(a, b, c, true);
			}

			//add bullet info
			btTransform trans;
			trans.setIdentity();
			trans.setOrigin(pos);
			btDefaultMotionState *motionState = new btDefaultMotionState(trans);
			btCollisionShape *btShape = new btBvhTriangleMeshShape(btmesh, true);
			btShape->setMargin(0.05);
			btScalar mass = 0.0;
			rb = new btRigidBody(mass, motionState, btShape);
		}
	}

	return rb;
}

void AppManager::Destroy()
{
	for (btRigidBody *rb : btObjects)
	{
		irr::scene::ISceneNode *n = static_cast<irr::scene::ISceneNode*>(rb->getUserPointer());
		n->remove();
		btWorld->removeRigidBody(rb);
		delete rb->getMotionState();
		delete rb->getCollisionShape();
		delete rb;
		rb = nullptr;
	}

	btObjects.clear();

	delete btWorld;
	delete btSolver;
	delete btDispatcher;
	delete btBroadPhase;
	delete btCollisionConfig;

	irrDevice->drop();
}