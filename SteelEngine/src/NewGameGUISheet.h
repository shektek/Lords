#ifndef NEWGAMEGUISHEET_H
#define NEWGAMEGUISHEET_H

#include "AbstractGUISheet.h"
#include "GUIMenu.h"
#include <IGUIEnvironment.h>
#include <ITexture.h>
#include <IGUIImage.h>
#include <IGUIListBox.h>
#include <IGUITabControl.h>

namespace Steel
{
	class NewGameGUISheet : public AbstractGUISheet
	{
		private:
			irr::video::ITexture	*m_newGameTexture;
			irr::gui::IGUIImage		*m_newGameImage;
			irr::gui::IGUIButton	*m_backButton;
			irr::gui::IGUIButton	*m_startGameButton;
			
			irr::gui::IGUITabControl *m_tabControl;
			
			irr::gui::IGUITab		*m_campaignTab;
			int						m_campaignTabIndex;
			irr::gui::IGUIListBox	*m_campaignList;
			irr::gui::IGUIListBox	*m_campaignLevelList;

			irr::gui::IGUITab		*m_scenarioTab;
			int						m_scenarioTabIndex;
			irr::gui::IGUIListBox	*m_scenarioList;
			irr::gui::IGUIListBox	*m_scenarioLevelList;
			
			irr::gui::IGUIButton	*m_generateRandomButton;
			irr::gui::IGUIStaticText *m_minmaxStaticText;
			irr::gui::IGUIEditBox	*m_minBox;
			irr::gui::IGUIEditBox	*m_maxBox;

		public:
			NewGameGUISheet(irr::video::ITexture *newGameTexture);
			virtual ~NewGameGUISheet();

			void					Init(int windowWidth, int windowHeight, GameMaster *game);
			void					Draw(irr::gui::IGUIEnvironment *irrGUI);

			int						GetActiveTabIndex(){ return m_tabControl->getActiveTab(); }
			int						GetCampaignTabIndex() { return m_campaignTabIndex; }
			int						GetScenarioTabIndex() { return m_scenarioTabIndex; }
			irr::gui::IGUIListBox	*GetCampaignListBox() { return m_campaignList; }
			irr::gui::IGUIListBox	*GetCampaignLevelListBox() { return m_campaignLevelList; }
	};
}
#endif