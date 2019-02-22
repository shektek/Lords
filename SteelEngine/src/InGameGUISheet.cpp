#include "InGameGUISheet.h"
#include "GUIMenu.h"

namespace Steel
{
	InGameGUISheet::InGameGUISheet()
	{

	}

	InGameGUISheet::~InGameGUISheet()
	{

	}

	void InGameGUISheet::Init(int windowWidth, int windowHeight, GameMaster *game)
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

	void InGameGUISheet::Draw(irr::gui::IGUIEnvironment *irrGUI)
	{
		//shorthand
		double winx = m_windowWidth;
		double winy = m_windowHeight;
		double winr = m_windowRatio;
		double bw = m_buttonWidth;
		double bh = m_buttonHeight;
		double bg = m_buttonDistance;

		m_endTurnButton = irrGUI->addButton(irr::core::rect<irr::s32>(winx - (bw + bg), winy - (bh + bg), winx - bg, winy - bg), 0,
			Steel::InGameMenuID::M_IG_ENDTURN, L"End turn", L"Pass to the next season");
	}
}