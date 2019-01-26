#include "GUISheetManager.h"

namespace Steel
{
	GUISheetManager::GUISheetManager()
	{
		m_sheets.clear();
	}

	GUISheetManager::~GUISheetManager()
	{
		for (std::map<GUIState, AbstractGUISheet*>::iterator it = m_sheets.begin(); it != m_sheets.end(); it++)
		{
			delete it->second;
		}
	}

	void GUISheetManager::InitSheet(GUIState state, int windowWidth, int windowHeight, GameMaster *game)
	{
		if (m_sheets.find(state) != m_sheets.end())
			m_sheets[state]->Init(windowWidth, windowHeight, game);
	}

	AbstractGUISheet *GUISheetManager::GetSheet(GUIState state)
	{
		AbstractGUISheet *result = nullptr;

		if (m_sheets.find(state) != m_sheets.end())
			result = m_sheets[state];

		return result;
	}

	void GUISheetManager::AddSheet(GUIState state, AbstractGUISheet *newSheet)
	{
		if (m_sheets.find(state) == m_sheets.end())
			m_sheets[state] = newSheet;
	}

	void GUISheetManager::RemoveSheet(GUIState state)
	{
		if (m_sheets.find(state) != m_sheets.end())
			m_sheets.erase(state);
	}

	void GUISheetManager::DrawSheet(GUIState state, irr::gui::IGUIEnvironment *irrGUI)
	{
		if (m_sheets.find(state) != m_sheets.end())
			m_sheets[state]->Draw(irrGUI);
	}
}