#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include "CoolStuff.h"
//engine
#include "irrlicht.h"
#include "btBulletDynamicsCommon.h"
#include "GUIStateMachine.h"
#include "GUIMenu.h"
#include "GUISheetManager.h"
//game
#include "GameMaster.h"

#pragma comment(lib, "..\\..\\irrlicht-1.8.3\\lib\\Win32-visualstudio\\irrlicht.lib")
#ifdef _DEBUG
#pragma comment(lib, "..\\..\\bullet3-2.86.1\\bin\\BulletDynamics_vs2010_debug.lib")
#pragma comment(lib, "..\\..\\bullet3-2.86.1\\bin\\BulletCollision_vs2010_debug.lib")
#pragma comment(lib, "..\\..\\bullet3-2.86.1\\bin\\LinearMath_vs2010_debug.lib")
#endif

#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")


class DebugDraw : public btIDebugDraw
{

public:

	DebugDraw();

	DebugDraw(irr::IrrlichtDevice* const device) :
		mode(DBG_NoDebug), driver(device->getVideoDriver()), logger(device->getLogger())
	{

	}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		//workaround to bullet's inconsistent debug colors which are either from 0.0 - 1.0 or from 0.0 - 255.0
		irr::video::SColor newColor(255, (irr::u32)color[0], (irr::u32)color[1], (irr::u32)color[2]);
		if (color[0] <= 1.0 && color[0] > 0.0)
			newColor.setRed((irr::u32)(color[0] * 255.0));
		if (color[1] <= 1.0 && color[1] > 0.0)
			newColor.setGreen((irr::u32)(color[1] * 255.0));
		if (color[2] <= 1.0 && color[2] > 0.0)
			newColor.setBlue((irr::u32)(color[2] * 255.0));

		this->driver->draw3DLine(
			irr::core::vector3df(from[0], from[1], from[2]),
			irr::core::vector3df(to[0], to[1], to[2]),
			newColor);
	}

	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
	{
		static const irr::video::SColor CONTACTPOINT_COLOR(255, 255, 255, 0); //bullet's are black :(

		//   this->drawLine(PointOnB, PointOnB + normalOnB*distance, CONTACTPOINT_COLOR);

		const btVector3 to(PointOnB + normalOnB*distance);

		this->driver->draw3DLine(
			irr::core::vector3df(PointOnB[0], PointOnB[1], PointOnB[2]),
			irr::core::vector3df(to[0], to[1], to[2]),
			CONTACTPOINT_COLOR);
	}

	void reportErrorWarning(const char* text)
	{
		this->logger->log(text, irr::ELL_ERROR);
	}

	void draw3dText(const btVector3& location, const char* text) { }

	void setDebugMode(int mode) { this->mode = mode; }

	int getDebugMode() const { return this->mode; }

private:

	int mode;

	irr::video::IVideoDriver* const driver;

	irr::ILogger* logger;
};

class AppManager : public irr::IEventReceiver
{
	protected:
		unsigned short		m_bpp;
		unsigned short		m_width;
		unsigned short		m_height;
		bool				m_vsync;
		bool				m_fullscreen;
		std::string			m_windowName;

		//input
		irr::s32			m_lastMouseX;
		irr::s32			m_lastMouseY;

		irr::core::recti	m_nonScrollBounds;
		irr::core::recti	m_scrollBoundaries[4];
		irr::s32			m_scrollBoundarySize;

		double				m_scrollSpeed;
		irr::s32			m_lastScrollX;
		irr::s32			m_lastScrollY;
		
		irr::s32			m_defaultCameraHeight;
		irr::s32			m_defaultCameraTargetHeight;
		irr::s32			m_currentCameraHeight;
		irr::s32			m_currentCameraTargetHeight;
		double				m_cameraZoomAmount;

		bool				m_isScrolling;

		//loop control
		bool				m_running;

		//irrlicht-specific objects
		irr::IrrlichtDevice *irrDevice;
		irr::video::IVideoDriver *irrDriver;
		irr::scene::ISceneManager *irrScene;
		irr::gui::IGUIEnvironment *irrGUI;
		irr::ITimer *irrTimer;

		//Bullet-specific objects
		btDiscreteDynamicsWorld *btWorld;
		irr::core::list<btRigidBody*> btObjects;
		btBroadphaseInterface *btBroadPhase;
		btDefaultCollisionConfiguration *btCollisionConfig;
		btCollisionDispatcher *btDispatcher;
		btSequentialImpulseConstraintSolver *btSolver;

		DebugDraw *debugDraw;
		irr::video::SMaterial debugMat;

		//Steel-specific objects
		Steel::GUIStateMachine*	m_GUIState;
		Steel::GUISheetManager* m_GUISheetManager;
		Steel::GameMaster*		m_gameMaster;

		//resources
		std::map<std::string, irr::scene::ICameraSceneNode*> m_cameras;
		std::map<std::string, irr::video::ITexture*>	m_textures;
		std::map<std::string, irr::scene::ITerrainSceneNode*> m_terrains;	//probably only 1
		std::map<std::string, irr::scene::ILightSceneNode *> m_lights;
		irr::scene::ILightSceneNode *m_activeLight;

		irr::video::SColor	m_summerTint;
		irr::video::SColor	m_autumnTint;
		irr::video::SColor	m_winterTint;
		irr::video::SColor	m_springTint;

		//private functions
		bool InitIrrlicht();
		bool InitGame();
		bool InitBullet(bool debug = false);

		void MatchLightingToSeason(Season season);
		void DoPassiveEvents();

		//debug stuff
		void CreateBox(const btVector3 &pos, const irr::core::vector3df &scale, btScalar mass);

		virtual bool OnEvent(const irr::SEvent &event);

		void		UpdateGUI();
		void		UpdatePhysics(irr::u32 delta);

		void		QuaternionToEuler(const btQuaternion &quat, btVector3 &euler);
		btRigidBody *IrrMeshToBullet(const irr::scene::ISceneNode *node);

	public:
		AppManager();
		AppManager(bool debug);
		~AppManager();

		bool Init();
		void Run();
		void Destroy();
};

#endif