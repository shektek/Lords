#include "MainMenuGUISheet.h"
#include "GUIMenu.h"

namespace Steel
{
	MainMenuGUISheet::MainMenuGUISheet(irr::video::ITexture *logoTexture)
	{
		m_logoTexture = logoTexture;
	}

	MainMenuGUISheet::~MainMenuGUISheet()
	{
		//irrlicht takes care of destroying GUI elements
	}

	void MainMenuGUISheet::Init(int windowWidth, int windowHeight, GameMaster *game)
	{
		m_gameMaster = game;
		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;
		m_windowRatio = m_windowWidth / m_windowHeight;
		double xa = m_windowWidth / 1000.00;
		double ya = m_windowHeight / 1000.00;
		m_buttonWidth = 160 * xa;
		m_buttonHeight = 50 * ya;
		m_buttonDistance = 40 * ya;
	}

	void MainMenuGUISheet::Draw(irr::gui::IGUIEnvironment *irrGUI)
	{
		//shorthand
		double winx = m_windowWidth;
		double winy = m_windowHeight;
		double winr = m_windowRatio;
		double bw = m_buttonWidth;
		double bh = m_buttonHeight;
		double bg = m_buttonDistance;

		//logo at top!
		m_logoImage = irrGUI->addImage(m_logoTexture, irr::core::vector2d<irr::s32>((winx*0.5) - 250, 0));
		//new game
		irrGUI->addButton(irr::core::rect<irr::s32>((winx*0.5) - (bw / 2), 260 + bh, (winx*0.5) + (bw / 2), 260 + (bh * 2)), 0, Steel::MainMenuID::M_MM_NEWGAME_ID, L"New Game", L"Begin a new conquest");
		//load game
		irrGUI->addButton(irr::core::rect<irr::s32>((winx*0.5) - (bw / 2), 260 + bh * 2 + bg, (winx*0.5) + (bw / 2), 260 + bh * 3 + bg), 0, Steel::MainMenuID::M_MM_LOADGAME_ID, L"Load Game", L"Continue a previous conquest");
		//options 
		irrGUI->addButton(irr::core::rect<irr::s32>((winx*0.5) - (bw / 2), 260 + bh * 3 + bg * 2, (winx*0.5) + (bw / 2), 260 + bh * 4 + bg * 2), 0, Steel::MainMenuID::M_MM_OPTIONS_ID, L"Options", L"Configure game settings");
		//quit
		irrGUI->addButton(irr::core::rect<irr::s32>((winx*0.5) - (bw / 2), 260 + bh * 4 + bg * 4, (winx*0.5) + (bw / 2), 260 + bh * 5 + bg * 4), 0, Steel::MainMenuID::M_MM_QUIT_ID, L"Quit", L"Return to the mundane");
	}
}