#include "InGameGUISheet.h"

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

	}
}