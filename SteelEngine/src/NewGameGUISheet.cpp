#include "NewGameGUISheet.h"
#include "GUIMenu.h"
#include "CoolStuff.h"

namespace Steel
{
	NewGameGUISheet::NewGameGUISheet(irr::video::ITexture *newGameTexture)
	{
		m_newGameTexture = newGameTexture;
	}

	NewGameGUISheet::~NewGameGUISheet()
	{

	}

	void NewGameGUISheet::Init(int windowWidth, int windowHeight, GameMaster *game)
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

	void NewGameGUISheet::Draw(irr::gui::IGUIEnvironment *irrGUI)
	{
		//shorthand
		double winx = m_windowWidth;
		double winy = m_windowHeight;
		double winr = m_windowRatio;
		double bw = m_buttonWidth;
		double bh = m_buttonHeight;
		double bg = m_buttonDistance;

		//new game image because this engine apparently can't stretch font textures
		m_newGameImage = irrGUI->addImage(m_newGameTexture, irr::core::vector2d<irr::s32>((winx*0.5) - 250, 0));

		m_backButton = irrGUI->addButton(irr::core::rect<irr::s32>(bg, winy - (bh + bg), bg + bw, winy - bg), 0, 
											Steel::NewGameMenuID::M_NG_BACK_ID, L"Back", L"Return to previous menu");

		m_startGameButton = irrGUI->addButton(irr::core::rect<irr::s32>(winx - (bw + bg), winy - (bh + bg), winx - bg, winy - bg), 0, 
											Steel::NewGameMenuID::M_NG_STARTGAME_ID, L"Start", L"The adventure begins!");

		//buttons for game type - campaign or custom scenario
		m_tabControl = irrGUI->addTabControl(QuickRect(100, 200, winx - 100, winy - bg * 4), 0, true, true, 
												Steel::NewGameMenuID::M_NG_GAMETAB_ID);
		m_campaignTab = m_tabControl->addTab(L"Campaign", Steel::NewGameMenuID::M_NG_CHECKCAMPAIGN_ID);
		m_scenarioTab = m_tabControl->addTab(L"Scenario", Steel::NewGameMenuID::M_NG_CHECKSCENARIO_ID);
		m_campaignTabIndex = 0;
		m_scenarioTabIndex = 0;

		//campaign options
		//display a list of available campaigns and the levels in them
		m_campaignList = irrGUI->addListBox(QuickRect(25, 25, 350, 175), m_campaignTab, Steel::NewGameMenuID::M_NG_CAMPAIGNLIST_ID);
		m_campaignLevelList = irrGUI->addListBox(QuickRect(350 + bg, 25, 675, 325), m_campaignTab, Steel::NewGameMenuID::M_NG_CAMPAIGNLEVELLIST_ID);
		
		//add the available campaigns
		//these don't have a gamefile entry yet so just make a default one, but typically this would dynamically fill the level list
		m_campaignList->addItem(L"Default");

		for (int i = 0; i < m_gameMaster->GetLevelCount(); i++)
		{
			Steel::Level *l = m_gameMaster->GetLevelAtIndex(i);

			wchar_t lvlName[256] = { 0 };
			mbstowcs(lvlName, l->GetLevelName().c_str(), 255);

			m_campaignLevelList->addItem(lvlName);
		}

		//Scenario options
		//list box of locations
		m_scenarioLevelList = irrGUI->addListBox(QuickRect(25, 25, 175, 175), m_scenarioTab, Steel::NewGameMenuID::M_NG_SCENARIOLIST_ID);
		m_scenarioLevelList->addItem(L"Random");
		
		//regenerate random
		m_generateRandomButton = irrGUI->addButton(QuickRect(200, 100, 200 + bw, 100 + bh), m_scenarioTab, Steel::NewGameMenuID::M_NG_REGENRANDOM_ID, L"Regenerate Random", L"Create a new random map, discarding the current one");
		
		//min/max territories to generate randomly
		m_minmaxStaticText = irrGUI->addStaticText(L"Min/Max territories", QuickRect(170, 10, 330, 25), false, false, m_scenarioTab);
		m_minBox = irrGUI->addEditBox(L"2", QuickRect(200, 25, 250, 25 + bh), true, m_scenarioTab, Steel::NewGameMenuID::M_NG_MINTERRITORIES_ID);
		m_maxBox = irrGUI->addEditBox(L"6", QuickRect(260, 25, 310, 25 + bh), true, m_scenarioTab, Steel::NewGameMenuID::M_NG_MAXTERRITORIES_ID);
	}
}